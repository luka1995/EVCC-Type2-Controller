/****************************************************************************
	Luka Penger 2015
	EV ACMP
****************************************************************************/
#ifndef __EV_ACMP_H 
#define __EV_ACMP_H

#include <lpc_types.h>

void EV_ACMP_Init(void);
void EV_ACMP_ClearInterrupt(void);
bool EV_ACMP_GetCompStatus(void);
bool EV_ACMP_GetCompEdgeStatus(void);

#endif // end __EV_ACMP_H
/****************************************************************************
                            End Of File
****************************************************************************/
