/**
* Header file: list all the name and type of the functions that will be written
*/
#include "project.h"
#include "keypad.h"


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
 * Prints ERROR on the LCD screen and delays for 10 seconds
*/
void error();


/**
 * Function that concerns the reading of the keypad and calls the function that needs to be called
*/
void react_to_keypad();



/**
 * Function that rotates the servo to the right
*/
void rotate_right();

/**
 * Function that rotates the servo to the left
*/
void rotate_left();

/*
    Initialisation de tous les composants de bases
*/
void initialize();