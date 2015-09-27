/****************************************************************************
	Luka Penger 2015
	SWM LPC82x
****************************************************************************/

#include <SWM_LPC82x.h>

#define PINASSIGN_IDX(movable)  (((movable) >> 4))
#define PINSHIFT(movable)       (((movable) & 0xF) << 3)

void SWM_Init(void) {
	Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);
}

void SWM_DeInit(void) {
	Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);
}

void SWM_MovablePinAssign(CHIP_SWM_PIN_MOVABLE_T movable, uint8_t assign) {
	uint32_t temp;
	int pinshift = PINSHIFT(movable), regIndex = PINASSIGN_IDX(movable);

	switch (regIndex) {
		case 0: {
			temp = LPC_SWM->PINASSIGN0 & (~(0xFF << pinshift));
			LPC_SWM->PINASSIGN0 = temp | (assign << pinshift);
			break;
		}
		case 1: {
			temp = LPC_SWM->PINASSIGN1 & (~(0xFF << pinshift));
			LPC_SWM->PINASSIGN1 = temp | (assign << pinshift);
			break;
		}
		case 2: {
			temp = LPC_SWM->PINASSIGN2 & (~(0xFF << pinshift));
			LPC_SWM->PINASSIGN2 = temp | (assign << pinshift);
			break;
		}
		case 3: {
			temp = LPC_SWM->PINASSIGN3 & (~(0xFF << pinshift));
			LPC_SWM->PINASSIGN3 = temp | (assign << pinshift);
			break;
		}
		case 4: {
			temp = LPC_SWM->PINASSIGN4 & (~(0xFF << pinshift));
			LPC_SWM->PINASSIGN4 = temp | (assign << pinshift);
			break;
		}
		case 5: {
			temp = LPC_SWM->PINASSIGN5 & (~(0xFF << pinshift));
			LPC_SWM->PINASSIGN5 = temp | (assign << pinshift);
			break;
		}
		case 6: {
			temp = LPC_SWM->PINASSIGN6 & (~(0xFF << pinshift));
			LPC_SWM->PINASSIGN6 = temp | (assign << pinshift);
			break;
		}
		case 7: {
			temp = LPC_SWM->PINASSIGN7 & (~(0xFF << pinshift));
			LPC_SWM->PINASSIGN7 = temp | (assign << pinshift);
			break;
		}
		case 8: {
			temp = LPC_SWM->PINASSIGN8 & (~(0xFF << pinshift));
			LPC_SWM->PINASSIGN8 = temp | (assign << pinshift);
			break;
		}
		case 9: {
			temp = LPC_SWM->PINASSIGN9 & (~(0xFF << pinshift));
			LPC_SWM->PINASSIGN9 = temp | (assign << pinshift);
			break;
		}
		case 10: {
			temp = LPC_SWM->PINASSIGN10 & (~(0xFF << pinshift));
			LPC_SWM->PINASSIGN10 = temp | (assign << pinshift);
			break;
		}
		case 11: {
			temp = LPC_SWM->PINASSIGN11 & (~(0xFF << pinshift));
			LPC_SWM->PINASSIGN11 = temp | (assign << pinshift);
			break;
		}
	}
}

void SWM_FixedPinEnable(CHIP_SWM_PIN_FIXED_T pin, bool enable) {
	if (enable) {
		SWM_EnableFixedPin(pin);
	} else {
		SWM_DisableFixedPin(pin);
	}
}

void SWM_EnableFixedPin(CHIP_SWM_PIN_FIXED_T pin) {
	LPC_SWM->PINENABLE0 &= ~((1 << (uint32_t) pin) | SWM_PINENABLE0_RESERVED);
}

void SWM_DisableFixedPin(CHIP_SWM_PIN_FIXED_T pin) {
	LPC_SWM->PINENABLE0 = (1 << (uint32_t) pin) | (LPC_SWM->PINENABLE0 & ~SWM_PINENABLE0_RESERVED);
}

bool SWM_IsEnabled(CHIP_SWM_PIN_FIXED_T pin) {
	return (bool)((LPC_SWM->PINENABLE0 & (1 << (uint32_t) pin)) == 0);
}

/****************************************************************************
                            End Of File
****************************************************************************/
