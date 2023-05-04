/*******************************************************************************
* File Name: Int_sound.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_Int_sound_H)
#define CY_ISR_Int_sound_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Int_sound_Start(void);
void Int_sound_StartEx(cyisraddress address);
void Int_sound_Stop(void);

CY_ISR_PROTO(Int_sound_Interrupt);

void Int_sound_SetVector(cyisraddress address);
cyisraddress Int_sound_GetVector(void);

void Int_sound_SetPriority(uint8 priority);
uint8 Int_sound_GetPriority(void);

void Int_sound_Enable(void);
uint8 Int_sound_GetState(void);
void Int_sound_Disable(void);

void Int_sound_SetPending(void);
void Int_sound_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Int_sound ISR. */
#define Int_sound_INTC_VECTOR            ((reg32 *) Int_sound__INTC_VECT)

/* Address of the Int_sound ISR priority. */
#define Int_sound_INTC_PRIOR             ((reg8 *) Int_sound__INTC_PRIOR_REG)

/* Priority of the Int_sound interrupt. */
#define Int_sound_INTC_PRIOR_NUMBER      Int_sound__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Int_sound interrupt. */
#define Int_sound_INTC_SET_EN            ((reg32 *) Int_sound__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Int_sound interrupt. */
#define Int_sound_INTC_CLR_EN            ((reg32 *) Int_sound__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Int_sound interrupt state to pending. */
#define Int_sound_INTC_SET_PD            ((reg32 *) Int_sound__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Int_sound interrupt. */
#define Int_sound_INTC_CLR_PD            ((reg32 *) Int_sound__INTC_CLR_PD_REG)


#endif /* CY_ISR_Int_sound_H */


/* [] END OF FILE */
