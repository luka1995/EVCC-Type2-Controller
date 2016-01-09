/*----------------------------------------------------------------------------
 *      Type 2 Controller LPC824
 *			Copyright 2015
 *			Luka Penger
 *			Avant Car d.o.o.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include <LPC82x.h>

#include "cmsis_os.h"
#include <GPIO_LPC82x.h>
#include <ADC_LPC82x.h>
#include <IAP_LPC82x.h>
#include <WWDT_LPC82x.h>
#include "SETTINGS.h"
#include "EV_UART.h"
#include "EV_PWM.h"
#include "EV_ADC.h"
#include "EV_STATE_MACHINE.h"
#include "EV_OUTPUTS.h"
#include "EV_INPUTS.h"

/*****************************************************************************
 * Definitions
 ****************************************************************************/

#define FIRMWARE_VERSION												"1.0"

#define DEFAULT_MODULE_ADDRESS									0
#define MODULE_ADDRESS_MAX_NUMBER								9999

#define ADC_SAMPLE_INTERVAL											100 // ms / when pwm = false
#define STATE_MACHINE_INTERVAL									50 // ms

/*****************************************************************************
 * Public variables
 ****************************************************************************/

extern SETTINGS_Struct settingsData;

extern EvModuleMode selectedModuleMode;

extern char EV_UART_Buffer[EV_UART_BUFFER_SIZE];
extern int EV_UART_BufferLength;
extern bool EV_UART_DataReceived;

extern float selectedMaximumCurrent;
extern bool pwmRunning;
extern float evPwmDutyCycle;

extern bool thresholdCrossed, sequenceComplete;
extern bool groundError;
extern int pilotPositiveADCValue; // 0 - 4095
extern int pilotNegativeADCValue; // 0 - 4095
extern float pilotPositiveVoltage; // 0.0V - 12.0V
extern float pilotNegativeVoltage; // 0.0V - -12.0V
extern bool adcVoltageMeasured;

extern void (*currentStateMachine)(void);
extern int evStateMachineWaitMs;
extern bool manualStartChargingFlag;

int ledWaitMs = 0;

/*****************************************************************************
 * Watchdog Timer
 ****************************************************************************/

void WDT_IRQHandler(void) {
	uint32_t wdtStatus = WWDT_GetStatus(LPC_WWDT);
	
	/* The chip will reset before this happens, but if the WDT doesn't
	   have WWDT_WDMOD_WDRESET enabled, this will hit once */
	if (wdtStatus & WWDT_WDMOD_WDTOF) {
		/* A watchdog feed didn't occur prior to window timeout */
		WWDT_UnsetOption(LPC_WWDT, WWDT_WDMOD_WDEN);	/* Stop WDT */
		WWDT_ClearStatusFlag(LPC_WWDT, WWDT_WDMOD_WDTOF);
		WWDT_Start(LPC_WWDT);	/* Needs restart */
	}

	/* Handle warning interrupt */
	if (wdtStatus & WWDT_WDMOD_WDINT) {
		/* A watchdog feed didn't occur prior to warning timeout */
		WWDT_ClearStatusFlag(LPC_WWDT, WWDT_WDMOD_WDINT);
		WWDT_Feed(LPC_WWDT);
	}
}

void Watchdog_Init(void) {
	uint32_t wdtFreq;
	
	/* Freq = 0.6Mhz, divided by 64. WDT_OSC should be 9.375khz */
	Clock_SetWDTOSC(WDTLFO_OSC_0_60, 64);
	
	SYSCTL_PowerUp(SYSCTL_SLPWAKE_WDTOSC_PD);

	wdtFreq = (Clock_GetWDTOSCRate() / 4);
	
	WWDT_Init(LPC_WWDT);

	/* Set watchdog feed time constant to approximately 2s
	   Set watchdog warning time to 512 ticks after feed time constant
	   Set watchdog window time to 3s */
	WWDT_SetTimeOut(LPC_WWDT, wdtFreq * 2);
	WWDT_SetWarning(LPC_WWDT, 512);
	WWDT_SetWindow(LPC_WWDT, wdtFreq * 3);

	/* Configure WWDT to reset on timeout */
	WWDT_SetOption(LPC_WWDT, WWDT_WDMOD_WDRESET);

	/* Clear watchdog warning and timeout interrupts */
	WWDT_ClearStatusFlag(LPC_WWDT, WWDT_WDMOD_WDTOF | WWDT_WDMOD_WDINT);

	/* Clear and enable watchdog interrupt */
	NVIC_ClearPendingIRQ(WDT_IRQn);
	NVIC_EnableIRQ(WDT_IRQn);

	/* Start watchdog */
	WWDT_Start(LPC_WWDT);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/


/*****************************************************************************
 * Private functions
 ****************************************************************************/

void init(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	/*
	1 = 1s
	10 = 100ms
	100 = 10ms
	1000 = 1ms*/
	SysTick_Config(SystemCoreClock / 1000);
	
	// WWDT Init
	
	Watchdog_Init();
	
	// EV Init
	
	EV_OUTPUTS_Init();
	
	EV_INPUTS_Init();

	EV_UART_Init();
	
	EV_PWM_Init();
	
	EV_ADC_Init();
	
	// Settings load
	
	SETTINGS_Load();
	
	// Default state machine
	
	EV_State_A1();
}

/*****************************************************************************
 * Main
 ****************************************************************************/

int main(void) {
	//
	// Variables
	//
	char *evUartModuleAddressString = NULL, *evUartFunctionString = NULL, *evUartValueString = NULL;
	int evUartModuleAddressSpaceIndex = 0, evUartFunctionSpaceIndex = 0;
	int evUartModuleAddressNumber = 0, evUartFunctionNumber = 0, evUartValueNumber = 0;
	bool evUartShowCommandError = false;
	int sysTickAdcSampleValue = 0, sysTickStateMachineValue = 0;
	
	//
	// Init
	//
 	init();
	
	// Send firmware version
	
	EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, 2);
	EV_UART_SendString(" V"FIRMWARE_VERSION);
	EV_UART_SendString("\r\n");
	
	// Set maximum current
	
	selectedMaximumCurrent = settingsData.selectedIdefault;
	EV_PWM_SetDutyCycle(selectedMaximumCurrent);
	
	//
	// While
	//
	while (1) {
		/*****************************************************************************
		* ADC Voltage Read
		****************************************************************************/
		
		if (sequenceComplete == true) {
			sequenceComplete = false; // Clear flag
			
			// Read CP Voltage
			
			EV_ADC_ReadCPVoltage();
	  }
		
		/*****************************************************************************
		* SysTick Value Listener
		****************************************************************************/

		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
			// ADC Sequencer
			
			if (pwmRunning == false) {
				if (sysTickAdcSampleValue >= ADC_SAMPLE_INTERVAL) {
					sysTickAdcSampleValue = 0; // Clear flag

					ADC_StartSequencer(LPC_ADC, ADC_SEQA_IDX);
				} else {
					sysTickAdcSampleValue++;
				}
			}
			
			// EV State Machine
			
			if (adcVoltageMeasured && sysTickStateMachineValue >= STATE_MACHINE_INTERVAL) {
				sysTickStateMachineValue = 0; // Clear flag

				// State machine
			
				EV_State_Machine();
			} else {
				sysTickStateMachineValue++;
			}
			
			if (evStateMachineWaitMs > 0) {
				evStateMachineWaitMs--;
			}
			
			// LED States
			
			if (ledWaitMs == 0) {
				if (currentStateMachine == EV_State_A1 || currentStateMachine == EV_State_A2) {
					ledWaitMs = 1000; // Only for disable setting led no 1ms
					
					EV_OUTPUTS_SetLED2(false);
					EV_OUTPUTS_SetLED1(false);
				} else if (currentStateMachine == EV_State_B1 || currentStateMachine == EV_State_B2) {
					ledWaitMs = 1000; // Only for disable setting led no 1ms
					
					EV_OUTPUTS_SetLED2(false);
					EV_OUTPUTS_SetLED1(true);
				} else if (currentStateMachine == EV_State_C1 || currentStateMachine == EV_State_C2) {
					ledWaitMs = 1000; // Only for disable setting led no 1ms
					
					EV_OUTPUTS_SetLED2(true);
					EV_OUTPUTS_SetLED1(false);
				} else if (currentStateMachine == EV_State_D1 || currentStateMachine == EV_State_D2) {
					ledWaitMs = 1000;
					
					EV_OUTPUTS_SetLED1(false);
					
					if (EV_OUTPUTS_GetStateLED2()) {
						EV_OUTPUTS_SetLED2(false);
					} else {
						EV_OUTPUTS_SetLED2(true);
					}
				} else if (currentStateMachine == EV_State_E || currentStateMachine == EV_State_F) {
					ledWaitMs = 2000;
					
					if (EV_OUTPUTS_GetStateLED1()) {
						EV_OUTPUTS_SetLED2(false);
						EV_OUTPUTS_SetLED1(false);
					} else {
						EV_OUTPUTS_SetLED2(true);
						EV_OUTPUTS_SetLED1(true);
					}
				}
			} else {
				ledWaitMs--;
			}
		}
		
		/*****************************************************************************
		* EV UART Listener
		****************************************************************************/
		
		if (EV_UART_DataReceived == true && (UART_GetStatus(EV_UART) & UART_STAT_RXIDLE) != 0 && (UART_GetStatus(EV_UART) & UART_STAT_RXRDY) == 0) {
			EV_UART_DataReceived = false;

			// Search MODULE ADDRESS space index
			evUartModuleAddressSpaceIndex = EV_UART_LocateCharacter(EV_UART_Buffer, EV_UART_BufferLength, 0x20, 0);
			
			if (evUartModuleAddressSpaceIndex > 0) {
				evUartModuleAddressString = (char *)malloc(evUartModuleAddressSpaceIndex);
				strncpy(evUartModuleAddressString, EV_UART_Buffer, evUartModuleAddressSpaceIndex);
				
				if (EV_UART_BufferIsNumber(evUartModuleAddressString, evUartModuleAddressSpaceIndex) == true) {
					evUartModuleAddressNumber = atoi(evUartModuleAddressString);
					
					// Free pointer
					if (evUartModuleAddressString != NULL) { free(evUartModuleAddressString); evUartModuleAddressString = NULL; }
					
					// Check module address if correct
					if (evUartModuleAddressNumber == DEFAULT_MODULE_ADDRESS || evUartModuleAddressNumber == settingsData.selectedModuleAddress) {
						// MODULE ADDRESS IS CORRECT

						// Search FUNCTION space index
						evUartFunctionSpaceIndex = EV_UART_LocateCharacter(EV_UART_Buffer, EV_UART_BufferLength, 0x20, 1);
						
						if (evUartFunctionSpaceIndex > 0) {
							evUartFunctionString = (char *)malloc(evUartFunctionSpaceIndex - (evUartModuleAddressSpaceIndex + 1));
							strncpy(evUartFunctionString, EV_UART_Buffer + (evUartModuleAddressSpaceIndex + 1), (evUartFunctionSpaceIndex - (evUartModuleAddressSpaceIndex + 1)));
							
							// COMMAND WITH VALUE
							if (EV_UART_BufferIsNumber(evUartFunctionString, (evUartFunctionSpaceIndex - (evUartModuleAddressSpaceIndex + 1))) == true) {
								evUartFunctionNumber = atoi(evUartFunctionString);
								
								// Free pointer
								if (evUartFunctionString != NULL) { free(evUartFunctionString); evUartFunctionString = NULL; }
								
								// Search VALUE
								evUartValueString = (char *)malloc(EV_UART_BufferLength - (evUartFunctionSpaceIndex + 1));
								strncpy(evUartValueString, EV_UART_Buffer + (evUartFunctionSpaceIndex + 1), (EV_UART_BufferLength - (evUartFunctionSpaceIndex + 1)));
							
								if (EV_UART_BufferIsNumber(evUartValueString, (EV_UART_BufferLength - (evUartFunctionSpaceIndex + 1))) == true) {
									evUartValueNumber = atoi(evUartValueString);
								
									// Free pointer
									if (evUartValueString != NULL) { free(evUartValueString); evUartValueString = NULL; }
	
									// Functions
									
									switch (evUartFunctionNumber) {
										case kEvUartCommandSetAddressDevice: { // SET ADDRESS OF DEVICE
											if (evUartValueNumber >= 1 && evUartValueNumber >= MODULE_ADDRESS_MAX_NUMBER) {
												settingsData.selectedModuleAddress = evUartValueNumber;
												
												SETTINGS_Save();
												
												EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
												EV_UART_SendString("\r\n");
											} else { // VALUE NOT CORRECT
												evUartShowCommandError = true;
											}
											break;
										}
										case kEvUartCommandSetIcMax: { // SET Ic MAX
											if (evUartValueNumber >= 0 && evUartValueNumber <= (EV_PWM_MAXIMUM_DUTY_CICLE * 10)) {
												switch (evUartValueNumber) {
													case 0: { // 6A
														EV_PWM_SetDutyCycle(EV_CURRENT_PWM_DUTY_6A);
														break;
													}
													case 1: { // 10A
														EV_PWM_SetDutyCycle(EV_CURRENT_PWM_DUTY_10A);
														break;
													}
													case 2: { // 13A
														EV_PWM_SetDutyCycle(EV_CURRENT_PWM_DUTY_13A);
														break;
													}
													case 3: { // 16A
														EV_PWM_SetDutyCycle(EV_CURRENT_PWM_DUTY_16A);
														break;
													}
													case 4: { // 20A
														EV_PWM_SetDutyCycle(EV_CURRENT_PWM_DUTY_20A);
														break;
													}
													case 5: { // 32A
														EV_PWM_SetDutyCycle(EV_CURRENT_PWM_DUTY_32A);
														break;
													}
													case 6: { // 40A
														EV_PWM_SetDutyCycle(EV_CURRENT_PWM_DUTY_40A);
														break;
													}
													case 7: { // 50A
														EV_PWM_SetDutyCycle(EV_CURRENT_PWM_DUTY_50A);
														break;
													}
													case 8: { // 60A
														EV_PWM_SetDutyCycle(EV_CURRENT_PWM_DUTY_60A);
														break;
													}
													case 9: { // 70A
														EV_PWM_SetDutyCycle(EV_CURRENT_PWM_DUTY_70A);
														break;
													}
													case 10: { // 80A
														EV_PWM_SetDutyCycle(EV_CURRENT_PWM_DUTY_80A);
														break;
													}
													case 11: { // Idefault
														EV_PWM_SetDutyCycle(settingsData.selectedIdefault);
														break;
													}
													default: {
														EV_PWM_SetDutyCycle((float)((float)evUartValueNumber / 10));
														break;
													}
												}
												
												EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
												EV_UART_SendString("\r\n");
											} else { // VALUE NOT CORRECT
												evUartShowCommandError = true;
											}
											break;
										}
										case kEvUartCommandSetIDefault: { // SET Idefault
											if (evUartValueNumber >= 0 && evUartValueNumber <= (EV_PWM_MAXIMUM_DUTY_CICLE * 10)) {
												switch (evUartValueNumber) {
													case 0: { // 6A
														settingsData.selectedIdefault = EV_CURRENT_PWM_DUTY_6A;
														break;
													}
													case 1: { // 10A
														settingsData.selectedIdefault = EV_CURRENT_PWM_DUTY_10A;
														break;
													}
													case 2: { // 13A
														settingsData.selectedIdefault = EV_CURRENT_PWM_DUTY_13A;
														break;
													}
													case 3: { // 16A
														settingsData.selectedIdefault = EV_CURRENT_PWM_DUTY_16A;
														break;
													}
													case 4: { // 20A
														settingsData.selectedIdefault = EV_CURRENT_PWM_DUTY_20A;
														break;
													}
													case 5: { // 32A
														settingsData.selectedIdefault = EV_CURRENT_PWM_DUTY_32A;
														break;
													}
													case 6: { // 40A
														settingsData.selectedIdefault = EV_CURRENT_PWM_DUTY_40A;
														break;
													}
													case 7: { // 50A
														settingsData.selectedIdefault = EV_CURRENT_PWM_DUTY_50A;
														break;
													}
													case 8: { // 60A
														settingsData.selectedIdefault = EV_CURRENT_PWM_DUTY_60A;
														break;
													}
													case 9: { // 70A
														settingsData.selectedIdefault = EV_CURRENT_PWM_DUTY_70A;
														break;
													}
													case 10: { // 80A
														settingsData.selectedIdefault = EV_CURRENT_PWM_DUTY_80A;
														break;
													}
													default: {
														settingsData.selectedIdefault = (float)((float)evUartValueNumber / 10);
														break;
													}
												}
												
												SETTINGS_Save();
												
												EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
												EV_UART_SendString("\r\n");
											} else { // VALUE NOT CORRECT
												evUartShowCommandError = true;
											}
											break;
										}
										default: { // THIS FUNCTION DOESNT EXISTS
											evUartShowCommandError = true;
											break;
										}
									}
								} else { // VALUE NOT CORRECT
									evUartShowCommandError = true;
								}
							} else { // FUNCTION NOT CORRECT
								evUartShowCommandError = true;
							}
						} else {
							evUartFunctionString = (char *)malloc(EV_UART_BufferLength - (evUartModuleAddressSpaceIndex + 1));
							strncpy(evUartFunctionString, EV_UART_Buffer + (evUartModuleAddressSpaceIndex + 1), (EV_UART_BufferLength - (evUartModuleAddressSpaceIndex + 1)));
							
							// COMMAND WITHOUT VALUE
							if (EV_UART_BufferIsNumber(evUartFunctionString, (EV_UART_BufferLength - (evUartModuleAddressSpaceIndex + 1))) == true) {
								evUartFunctionNumber = atoi(evUartFunctionString);
								
								// Free pointer
								if (evUartFunctionString != NULL) { free(evUartFunctionString); evUartFunctionString = NULL; }
	
								// Functions
								
								switch (evUartFunctionNumber) {
									case kEvUartCommandResetDevice: { // RESET DEVICE
										NVIC_SystemReset();
										break;
									}
									case kEvUartCommandLoadFactorySettings: { // LOAD FACTORY SETTINGS AND RESET DEVICE
										SETTINGS_LoadFactorySettings();

										NVIC_SystemReset();
										break;
									}
									case kEvUartCommandGetVersionFirmware: { // GET VERSION OF FIRMWARE
										EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
										EV_UART_SendString(" V"FIRMWARE_VERSION);
										EV_UART_SendString("\r\n");
										break;
									}
									case kEvUartCommandGetAddressDevice: { // GET ADDRESS OF DEVICE
										EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
										EV_UART_SendStringFormat(" %d", settingsData.selectedModuleAddress);
										EV_UART_SendString("\r\n");
										break;
									}
									case kEvUartCommandGetStateMachineMode: { // GET STATE MACHINE MODE
										EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
										
										if (currentStateMachine == EV_State_A1) {
											EV_UART_SendStringFormat(" 0");
										} else if (currentStateMachine == EV_State_A2) {
											EV_UART_SendStringFormat(" 1");
										} else if (currentStateMachine == EV_State_B1) {
											EV_UART_SendStringFormat(" 2");
										} else if (currentStateMachine == EV_State_B2) {
											EV_UART_SendStringFormat(" 3");
										} else if (currentStateMachine == EV_State_C1) {
											EV_UART_SendStringFormat(" 4");
										} else if (currentStateMachine == EV_State_C2) {
											EV_UART_SendStringFormat(" 5");
										} else if (currentStateMachine == EV_State_D1) {
											EV_UART_SendStringFormat(" 6");
										} else if (currentStateMachine == EV_State_D2) {
											EV_UART_SendStringFormat(" 7");
										} else if (currentStateMachine == EV_State_E) {
											EV_UART_SendStringFormat(" 8");
										} else if (currentStateMachine == EV_State_F) {
											EV_UART_SendStringFormat(" 9");
										}
										
										EV_UART_SendString("\r\n");
										break;
									}
									case kEvUartCommandSwitchModeToManual: { // SWITCH MODE TO MANUAL (Stop charging)
										if (selectedModuleMode != kEvModuleModeManual) {
											selectedModuleMode = kEvModuleModeManual;
											
											if (currentStateMachine == EV_State_C2) {
												EV_State_C1();
											} else if (currentStateMachine == EV_State_D2) {
												EV_State_D1();
											}
											
											EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
											EV_UART_SendString("\r\n");
										} else {
											evUartShowCommandError = true;
										}
										break;
									}
									case kEvUartCommandSwitchModeToAuto: { // SWITCH MODE TO AUTO (Stop charging)
										if (selectedModuleMode != kEvModuleModeAuto) {
											selectedModuleMode = kEvModuleModeAuto;
											
											if (currentStateMachine == EV_State_C2) {
												EV_State_C1();
											} else if (currentStateMachine == EV_State_D2) {
												EV_State_D1();
											}
											
											EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
											EV_UART_SendString("\r\n");
										} else {
											evUartShowCommandError = true;
										}
										break;
									}
									case kEvUartCommandGetStatusOutput: { // GET STATUS OF OUTPUT
										EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
										EV_UART_SendStringFormat(" %d", ((EV_OUTPUTS_GetStateShut() << 4) + (EV_OUTPUTS_GetStateVentilation() << 3) + (EV_OUTPUTS_GetStatePower() << 2) + (EV_OUTPUTS_GetStateLED2() << 1) + (EV_OUTPUTS_GetStateLED1() << 0)));
										EV_UART_SendString("\r\n");
										break;
									}
									case kEvUartCommandGetStatusInput: { // GET STATUS OF INPUT
										EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
										EV_UART_SendStringFormat(" %d", ((EV_INPUTS_GetStateSwitchLockDevice() << 1) + (EV_INPUTS_GetStateSwitchStartCharging() << 0)));
										EV_UART_SendString("\r\n");
										break;
									}
									case kEvUartCommandGetADCValueCPPositive: { // GET ADC-value of pos. Ucp
										EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
										EV_UART_SendStringFormat(" %d", pilotPositiveADCValue);
										EV_UART_SendString("\r\n");
										break;
									}
									case kEvUartCommandGetADCValueCPNegative: { // GET ADC-value of neg. Ucp
										EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
										EV_UART_SendStringFormat(" %d", (0xFFF - pilotNegativeADCValue));
										EV_UART_SendString("\r\n");
										break;
									}
									case kEvUartCommandGetCPPositive: { // Get Ucp pos. voltage
										EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
										EV_UART_SendStringFormat(" %.2f", pilotPositiveVoltage);
										EV_UART_SendString("\r\n");
										break;
									}
									case kEvUartCommandGetCPNegative: { // Get Ucp neg. voltage
										if (pwmRunning) {
											EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
											EV_UART_SendStringFormat(" %.2f", pilotNegativeVoltage);
											EV_UART_SendString("\r\n");
										} else {
											EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
											EV_UART_SendString(" PWM DISABLED");
											EV_UART_SendString("\r\n");
										}
										break;
									}
									case kEvUartCommandGetADCValueCS: { // GET ADC-value of Ucs
										break;
									}
									case kEvUartCommandGetIcPWMDutyCycle: { // GET Ic - PWM duty cycle
										EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
										EV_UART_SendStringFormat(" %d", (int)(evPwmDutyCycle * 10));
										EV_UART_SendString("\r\n");
										break;
									}
									case kEvUartCommandGetIDefault: { // Get Idefault
										EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
										EV_UART_SendStringFormat(" %d", (int)((float)settingsData.selectedIdefault * 10));
										EV_UART_SendString("\r\n");
										break;
									}
									case kEvUartCommandSwitchPWMOn: { // Switch PWM on
										if (selectedModuleMode == kEvModuleModeManual) {
											if (!pwmRunning) EV_PWM_Start();

											EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
											EV_UART_SendString("\r\n");
										} else {
											evUartShowCommandError = true;
										}
										break;
									}
									case kEvUartCommandSwitchPWMOff: { // Switch PWM off
										if (selectedModuleMode == kEvModuleModeManual) {
											if (pwmRunning) EV_PWM_Stop();

											EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
											EV_UART_SendString("\r\n");
										} else {
											evUartShowCommandError = true;
										}
										break;
									}
									case kEvUartCommandGetPWMState: { // Get PWM State
										if (pwmRunning == true) {
											EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
											EV_UART_SendString(" 1");
											EV_UART_SendString("\r\n");
										} else {
											EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
											EV_UART_SendString(" 0");
											EV_UART_SendString("\r\n");
										}
										break;
									}
									case kEvUartCommandStartCharging: { // Start charging
										if (selectedModuleMode == kEvModuleModeManual) {
											if (currentStateMachine == EV_State_B1 || currentStateMachine == EV_State_B2) {
												manualStartChargingFlag = true;
												
												EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
												EV_UART_SendString("\r\n");
											} else if (currentStateMachine == EV_State_C1 || currentStateMachine == EV_State_D2) {
												if (currentStateMachine == EV_State_C1) {
													EV_State_C2();
												} else if (currentStateMachine == EV_State_D1) {
													EV_State_D2();
												}
												
												EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
												EV_UART_SendString("\r\n");
											} else {
												evUartShowCommandError = true;
											}
										} else {
											evUartShowCommandError = true;
										}
										break;
									}
									case kEvUartCommandStopCharging: { // Stop charging
										if (selectedModuleMode == kEvModuleModeManual) {
											if (currentStateMachine == EV_State_C2 || currentStateMachine == EV_State_D2) {
												if (currentStateMachine == EV_State_C2) {
													EV_State_C1();
												} else if (currentStateMachine == EV_State_D2) {
													EV_State_D1();
												}
												
												EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
												EV_UART_SendString("\r\n");
											} else {
												evUartShowCommandError = true;
											}
										} else {
											evUartShowCommandError = true;
										}
										break;
									}
									default: { // THIS FUNCTION DOESNT EXISTS
										evUartShowCommandError = true;
										break;
									}
								}
							} else { // FUNCTION NOT CORRECT
								evUartShowCommandError = true;
							}
						}
					} else {
						// MODULE ADDRESS NOT CORRECT
					}
				} else {
					// MODULE ADDRESS NOT CORRECT
				}
			} else { // COMMAND ERROR
				evUartShowCommandError = true;
			}
			
			if (evUartShowCommandError == true) {
				evUartShowCommandError = false;
				
				EV_UART_SendStringFormat("%c%d %02d", EV_UART_SEND_CHARACTER, settingsData.selectedModuleAddress, evUartFunctionNumber);
				EV_UART_SendString(" ERROR");
				EV_UART_SendString("\r\n");
			}
			
			// Set to default
			evUartModuleAddressSpaceIndex = 0; evUartFunctionSpaceIndex = 0;
			evUartModuleAddressNumber = 0; evUartFunctionNumber = 0, evUartValueNumber = 0;
			
			// Free pointer
			if (evUartModuleAddressString != NULL) { free(evUartModuleAddressString); evUartModuleAddressString = NULL; }
			if (evUartFunctionString != NULL) { free(evUartFunctionString); evUartFunctionString = NULL; }
			if (evUartValueString != NULL) { free(evUartValueString); evUartValueString = NULL; }
		}
	}
}
