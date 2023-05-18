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


