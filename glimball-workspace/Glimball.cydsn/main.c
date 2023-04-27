/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "functions.h"
#include "interruptions.c"



int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    initialize();
    for(;;)
    {   
        react();
        modes();
    }
}

/* [] END OF FILE */
