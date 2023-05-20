
/*
One-dimensional Gimbal Project - ELEC-H310 - Digital Electronics

Achten Alexandre
Hanssens Dimitri
Humblet Raphaël
Roybin Alexandre

March - April 2023
*/

#include "functions.h"


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    initialize();
    for(;;){   
        react();
        modes();
    }
}


