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
#include "SETTINGS.h"
#include "EV_MOTOR.h"
#include "EV_UART.h"
#include "EV_PWM.h"
#include "EV_ADC.h"
#include "EV_STATE_MACHINE.h"

/*****************************************************************************
 * Definitions
 ****************************************************************************/

#define FIRMWARE_VERSION												"1.0"

#define DEFAULT_MODULE_ADDRESS									0
#define MODULE_ADDRESS_MAX_NUMBER								8

#define ADC_SAMPLE_INTERVAL											10 // ms
#define STATE_MACHINE_INTERVAL									100 // ms

#define LED_GREEN_PIN 													19
#define LED_RED_PIN 														18

typedef enum {
	kEvModuleModeA, // <- PLUG AND CHARGE
	kEvModuleModeM // <- MANUAL
} EvModuleMode;

/*****************************************************************************
 * Public variables
 ****************************************************************************/

char selectedModuleAddress = 0;
EvModuleMode selectedModuleMode = kEvModuleModeA;

extern char EV_UART_Buffer[EV_UART_BUFFER_SIZE];
extern int EV_UART_BufferLength;
extern bool EV_UART_DataReceived;

extern bool pwmRunning;
extern float evPwmDutyCycle;

extern bool thresholdCrossed, sequenceComplete;
extern bool groundError;
extern float pilotPositiveVoltage; // 0.0V - 12.0V
extern float pilotNegativeVoltage; // 0.0V - -12.0V
//extern void (*stateMachine)();

/*****************************************************************************
 * Private variables
 ****************************************************************************/


/*****************************************************************************
 * Thread definitions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/
unsigned int currectDelayTick = 0;

void delayMs(unsigned int ms) {
	int i = 0;
	currectDelayTick = 0;
	
	while (currectDelayTick < ms);
}

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
	
	// GPIO Init
	
	GPIO_Init(LPC_GPIO_PORT);
	
	// LED Init
	
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, LED_GREEN_PIN); // LED Green -> Output
	GPIO_SetPinDIROutput(LPC_GPIO_PORT, LED_RED_PIN); // LED Red -> Output
	
	GPIO_SetPinState(LPC_GPIO_PORT, LED_GREEN_PIN, true); // LED Green -> OFF
	GPIO_SetPinState(LPC_GPIO_PORT, LED_RED_PIN, true); // LED Red -> OFF
	
	// EV Init
	
	EV_MOTOR_Init();
	
	EV_UART_Init();
	
	EV_PWM_Init();
	
	EV_ADC_Init();
}

/*****************************************************************************
 * Main
 ****************************************************************************/

int main(void) {
	//
	// Variables
	//
	char *evUartModuleAddressString, *evUartFunctionString, *evUartValueString;
	int evUartModuleAddressSpaceIndex = 0, evUartFunctionSpaceIndex = 0;
	int evUartModuleAddressNumber = 0, evUartFunctionNumber = 0, evUartValueNumber = 0;
	bool evUartShowCommandError = false;
	int sysTickAdcSampleValue = 0, sysTickStateMachineValue = 0;
	
	//
	// Init
	//
 	init();
	
	//
	// While
	//
	
	while (1) {
		/*****************************************************************************
		* EV UART Listener
		****************************************************************************/
		/*
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
					if (evUartModuleAddressString) free(evUartModuleAddressString);
					
					// Check module address if correct
					if (evUartModuleAddressNumber == DEFAULT_MODULE_ADDRESS || evUartModuleAddressNumber == selectedModuleAddress) {
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
								if (evUartFunctionString) free(evUartFunctionString);
								
								// Search VALUE
								evUartValueString = (char *)malloc(EV_UART_BufferLength - (evUartFunctionSpaceIndex + 1));
								strncpy(evUartValueString, EV_UART_Buffer + (evUartFunctionSpaceIndex + 1), (EV_UART_BufferLength - (evUartFunctionSpaceIndex + 1)));
							
								if (EV_UART_BufferIsNumber(evUartValueString, (EV_UART_BufferLength - (evUartFunctionSpaceIndex + 1))) == true) {
									evUartValueNumber = atoi(evUartValueString);
								
									// Free pointer
									if (evUartValueString) free(evUartValueString);
	
									// Functions
									
									switch (evUartFunctionNumber) {
										case 0: { // RESET DEVICE
											if (evUartValueNumber == 1111) {
												
											} else {
												evUartShowCommandError = true;
											}
											break;
										}
										case 5: { // SET OUTPUT
											if (selectedModuleMode == kEvModuleModeM) {
												
												
											} else {
												evUartShowCommandError = true;
											}
											break;
										}
										case 6: { // CLEAR OUTPUT
											if (selectedModuleMode == kEvModuleModeM) {
												
												
											} else {
												evUartShowCommandError = true;
											}
											break;
										}
										case 12: { // SET Ic MAX
											
											break;
										}
										case 15: { // SET I DEFAULT
											
											break;
										}
										case 22: { // SET MODULE ADDRESS
											
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
								if (evUartFunctionString) free(evUartFunctionString);
	
								// Functions
								
								switch (evUartFunctionNumber) {
									case 1: { // GET FIRMWARE VERSION
										EV_UART_SendStringFormat("%c%01d %02d", EV_UART_SEND_CHARACTER, selectedModuleAddress, evUartFunctionNumber);
										EV_UART_SendString(" V"FIRMWARE_VERSION);
										EV_UART_SendString("\r\n");							
										break;
									}
									case 2: { // GET MODE
										
										
										break;
									}
									case 3: { // SWITCH MODE TO MANUAL
										if (selectedModuleMode == kEvModuleModeA) {
											// Change mode to M
											selectedModuleMode = kEvModuleModeM;
											
											EV_UART_SendStringFormat("%c%01d %02d", EV_UART_SEND_CHARACTER, selectedModuleAddress, evUartFunctionNumber);
											EV_UART_SendString("\r\n");
										} else { // MODE IS ALREADY SET TO M
											evUartShowCommandError = true;
										}
										break;
									}
									case 4: { // GET STATUS OF OUTPUT
										break;
									}
									case 7: { // GET ADC VALUE OF POSITIVE Ucp
										
										break;
									}
									case 8: { // GET ADC VALUE OF NEGATIVE Ucp
										
										break;
									}
									case 9: { // GET ADC VALUE OF Ucs
										
										break;
									}
									case 10: { // GET STATUS OF INPUT
										
										break;
									}
									case 11: { // GET Ic
										
										break;
									}
									case 13: { // SWITCH PWM ON
										if (selectedModuleMode == kEvModuleModeM) {
											EV_ADC_ClearCalculationValues();
											
											EV_PWM_Start();
											
											EV_UART_SendStringFormat("%c%01d %02d", EV_UART_SEND_CHARACTER, selectedModuleAddress, evUartFunctionNumber);
											EV_UART_SendString("\r\n");
										} else {
											evUartShowCommandError = true;
										}
										break;
									}
									case 14: { // SWITCH PWM OFF
										if (selectedModuleMode == kEvModuleModeM) {
											EV_ADC_ClearCalculationValues();
											EV_PWM_Stop();
											
											EV_UART_SendStringFormat("%c%01d %02d", EV_UART_SEND_CHARACTER, selectedModuleAddress, evUartFunctionNumber);
											EV_UART_SendString("\r\n");
										} else {
											evUartShowCommandError = true;
										}
										break;
									}
									case 23: { // GET ADDRESS OF DEVICE
										
										break;
									}
									case 25: { // STOP CHARING/LEAVE MANUAL (jump to A)
										
										break;
									}
									case 26: { // Get Idefault
										
										break;
									}
									case 27: { // Set bBreakCharge-flag
										
										break;
									}
									case 28: { // Clear bBreakCharge-flag
										
										break;
									}
									case 29: { // GET bBreakCharge-flag
										
										break;
									}
									case 30: { // JUMP TO A'
										
										break;
									}
									case 31: { // RESERT/JUMP TO OFF (enabled onlz in A')
										
										break;
									}
									case 40: { // GET DEVICE ID
										
										EV_UART_SendStringFormat("%c%01d %02d", EV_UART_SEND_CHARACTER, selectedModuleAddress, evUartFunctionNumber);
										EV_UART_SendStringFormat("%d", SYSCTL_GetDeviceID());
										EV_UART_SendString("\r\n");
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
				
				EV_UART_SendString("CMD ERROR\r\n");
			}
			
			// Set to default
			evUartModuleAddressSpaceIndex = 0; evUartFunctionSpaceIndex = 0;
			evUartModuleAddressNumber = 0; evUartFunctionNumber = 0, evUartValueNumber = 0;
			
			// Free pointer
			if (evUartModuleAddressString) free(evUartModuleAddressString);
			if (evUartFunctionString) free(evUartFunctionString);
			if (evUartValueString) free(evUartValueString);
		}*/
		
		/*****************************************************************************
		* SysTick Value Listener
		****************************************************************************/

		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
			// Delay ms tick
			
			//currectDelayTick++;
			
			// ADC Sequencer
			/*
			if (pwmRunning == false) {
				if (sysTickAdcSampleValue >= ADC_SAMPLE_INTERVAL) {
					sysTickAdcSampleValue = 0; // Clear flag
					
					ADC_StartSequencer(LPC_ADC, ADC_SEQA_IDX);
				} else {
					sysTickAdcSampleValue++;
				}
			}
			*/
			// State Machine
			/*
			if (sysTickStateMachineValue >= STATE_MACHINE_INTERVAL) {
				sysTickStateMachineValue = 0; // Clear flag
					
				// State machine
			
				EV_State_Machine();
			} else {
				sysTickStateMachineValue++;
			}*/
		}

		/*****************************************************************************
		* ADC Voltage Read
		****************************************************************************/
		
		if (sequenceComplete == true) {
			sequenceComplete = false; // Clear flag
			
			// Read CP Voltage
			
			EV_ADC_ReadCPVoltage();
	  }
		
		/*****************************************************************************
		* Ground Error
		****************************************************************************/
/*
		if (groundError == true) {
			groundError = false;
			
			// DISABLE ALL OUTPUTS !!!
			
			
		}*/

	}
}
