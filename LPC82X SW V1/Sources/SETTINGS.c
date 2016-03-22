/****************************************************************************
	Luka Penger 2015
	SETTINGS
****************************************************************************/

#include "SETTINGS.h"
#include "IAP_LPC82x.h"
#include "FMC_LPC82x.h"
#include "EV_PWM.h"

SETTINGS_Struct settingsData;

void SETTINGS_Load(void) {
	if (IAP_BlankCheckSector(IAP_LAST_SECTOR, IAP_LAST_SECTOR) == IAP_SECTOR_NOT_BLANK) {
		memcpy(&settingsData, (void *)START_ADDR_LAST_SECTOR, sizeof(SETTINGS_Struct));
	} else {
		SETTINGS_LoadFactorySettings();
	}
}

void SETTINGS_Save(void) {
	/* Disable interrupt mode so it doesn't fire during FLASH updates */
	__disable_irq();
	
	/* IAP Flash programming */
	/* Prepare to write/erase the last sector */
	IAP_PreSectorForReadWrite(IAP_LAST_SECTOR, IAP_LAST_SECTOR);

	/* Erase the last sector */
	IAP_EraseSector(IAP_LAST_SECTOR, IAP_LAST_SECTOR);

	/* Prepare to write/erase the last sector */
	IAP_PreSectorForReadWrite(IAP_LAST_SECTOR, IAP_LAST_SECTOR);
	
	/* Write to the last sector */
	IAP_CopyRamToFlash(START_ADDR_LAST_SECTOR, (uint32_t *)&settingsData, SECTOR_SIZE);

	/* Re-enable interrupt mode */
	__enable_irq();
	
	/* Start the signature generator for the last sector */
	FMC_ComputeSignatureBlocks(START_ADDR_LAST_SECTOR, (SECTOR_SIZE / 31));

	/* Check for signature geenration completion */
	while (FMC_IsSignatureBusy());
}

void SETTINGS_LoadFactorySettings(void) {
	settingsData.selectedModuleAddress = 0;
	settingsData.selectedIdefault = EV_CURRENT_PWM_DUTY_6A;
	settingsData.enableProximity = 1;
}

/****************************************************************************
                            End Of File
****************************************************************************/
