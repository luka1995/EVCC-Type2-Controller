/****************************************************************************
	Luka Penger 2015
	IAP LPC82x
****************************************************************************/

#include <IAP_LPC82x.h>

void iap_entry(unsigned int cmd_param[], unsigned int status_result[]) {
	((IAP_ENTRY_T) IAP_ENTRY_LOCATION)(cmd_param, status_result);
}

/* Prepare sector for write operation */
uint8_t IAP_PreSectorForReadWrite(uint32_t strSector, uint32_t endSector) {
	uint32_t command[5], result[5];

	command[0] = IAP_PREWRRITE_CMD;
	command[1] = strSector;
	command[2] = endSector;
	iap_entry(command, result);

	return result[0];
}

/* Copy RAM to flash */
uint8_t IAP_CopyRamToFlash(uint32_t dstAdd, uint32_t *srcAdd, uint32_t byteswrt) {
	uint32_t command[5], result[5];

	command[0] = IAP_WRISECTOR_CMD;
	command[1] = dstAdd;
	command[2] = (uint32_t) srcAdd;
	command[3] = byteswrt;
	command[4] = SystemCoreClock / 1000;
	iap_entry(command, result);

	return result[0];
}

/* Erase sector */
uint8_t IAP_EraseSector(uint32_t strSector, uint32_t endSector) {
	uint32_t command[5], result[5];

	command[0] = IAP_ERSSECTOR_CMD;
	command[1] = strSector;
	command[2] = endSector;
	command[3] = SystemCoreClock / 1000;
	iap_entry(command, result);

	return result[0];
}

/* Blank check sector */
uint8_t IAP_BlankCheckSector(uint32_t strSector, uint32_t endSector) {
	uint32_t command[5], result[5];

	command[0] = IAP_BLANK_CHECK_SECTOR_CMD;
	command[1] = strSector;
	command[2] = endSector;
	iap_entry(command, result);

	return result[0];
}

/* Read part identification number */
uint32_t IAP_ReadPID() {
	uint32_t command[5], result[5];

	command[0] = IAP_REPID_CMD;
	iap_entry(command, result);

	return result[1];
}

/* Read boot code version number */
uint8_t IAP_ReadBootCode() {
	uint32_t command[5], result[5];

	command[0] = IAP_READ_BOOT_CODE_CMD;
	iap_entry(command, result);

	return result[0];
}

/* IAP compare */
uint8_t IAP_Compare(uint32_t dstAdd, uint32_t srcAdd, uint32_t bytescmp) {
	uint32_t command[5], result[5];

	command[0] = IAP_COMPARE_CMD;
	command[1] = dstAdd;
	command[2] = srcAdd;
	command[3] = bytescmp;
	iap_entry(command, result);

	return result[0];
}

/* Reinvoke ISP */
uint8_t IAP_ReinvokeISP() {
	uint32_t command[5], result[5];

	command[0] = IAP_REINVOKE_ISP_CMD;
	iap_entry(command, result);

	return result[0];
}

/* Read the unique ID */
uint32_t IAP_ReadUID() {
	uint32_t command[5], result[5];

	command[0] = IAP_READ_UID_CMD;
	iap_entry(command, result);

	return result[1];
}

/* Erase page */
uint8_t IAP_ErasePage(uint32_t strPage, uint32_t endPage) {
	uint32_t command[5], result[5];

	command[0] = IAP_ERASE_PAGE_CMD;
	command[1] = strPage;
	command[2] = endPage;
	command[3] = SystemCoreClock / 1000;
	iap_entry(command, result);

	return result[0];
}

/****************************************************************************
                            End Of File
****************************************************************************/
