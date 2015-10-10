/****************************************************************************
	Luka Penger 2015
	SETTINGS
****************************************************************************/
#ifndef __SETTINGS_H 
#define __SETTINGS_H

#include <lpc_types.h>

#define START_ADDR_LAST_SECTOR  						0x00007C00
#define SECTOR_SIZE             						1024
#define IAP_LAST_SECTOR         						31

typedef struct {
	int selectedModuleAddress;
	float selectedIdefault;
} SETTINGS_Struct;

void SETTINGS_Load(void);
void SETTINGS_Save(void);
void SETTINGS_LoadFactorySettings(void);

#endif // end __SETTINGS_H
/****************************************************************************
                            End Of File
****************************************************************************/
