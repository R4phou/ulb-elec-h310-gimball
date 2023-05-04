/*******************************************************************************
* File Name: Mux.h
* Version 1.80
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_Mux_H)
#define CY_AMUX_Mux_H

#include "cyfitter.h"
#include "cyfitter_cfg.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
         (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))    
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif /* ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) */


/***************************************
*        Function Prototypes
***************************************/

void Mux_Start(void) ;
#define Mux_Init() Mux_Start()
void Mux_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void Mux_Stop(void); */
/* void Mux_Select(uint8 channel); */
/* void Mux_Connect(uint8 channel); */
/* void Mux_Disconnect(uint8 channel); */
/* void Mux_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define Mux_CHANNELS  4u
#define Mux_MUXTYPE   1
#define Mux_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define Mux_NULL_CHANNEL 0xFFu
#define Mux_MUX_SINGLE   1
#define Mux_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if Mux_MUXTYPE == Mux_MUX_SINGLE
# if !Mux_ATMOSTONE
#  define Mux_Connect(channel) Mux_Set(channel)
# endif
# define Mux_Disconnect(channel) Mux_Unset(channel)
#else
# if !Mux_ATMOSTONE
void Mux_Connect(uint8 channel) ;
# endif
void Mux_Disconnect(uint8 channel) ;
#endif

#if Mux_ATMOSTONE
# define Mux_Stop() Mux_DisconnectAll()
# define Mux_Select(channel) Mux_FastSelect(channel)
void Mux_DisconnectAll(void) ;
#else
# define Mux_Stop() Mux_Start()
void Mux_Select(uint8 channel) ;
# define Mux_DisconnectAll() Mux_Start()
#endif

#endif /* CY_AMUX_Mux_H */


/* [] END OF FILE */
