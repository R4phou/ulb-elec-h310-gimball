/**
* Header file: list all the name and type of the functions that will be written
*/
#include "project.h"

/**
 * Function that lights all the LEDS
*/
void light_LEDS();



/**
 * Function that turn off all the LEDS
*/
void turn_off_LEDS();



/**
 * Function that switch modes
 * Take also care of switching the LEDS if the mode is switched
*/
void switch_to_glimball_mode();


/**
 * Function that switch to the test mode
 * Take also care of switching the LEDS if the mode is switched
*/
void switch_to_test_mode();


/**
 * function that changes the mode when a button is pressed
*/
void switch_mode();


/**
 * Prints ERROR on the LCD screen
*/
void error();



/*
    Initialisation de tous les composants de bases
*/
void initialize();