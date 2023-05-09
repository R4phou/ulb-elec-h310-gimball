/**
* Header file: list all the name and type of the functions that will be written
*/
#include "./Generated_Source/PSoC5/project.h"
#include "keypad.h"
#include "math.h"
#include "stdio.h"
#include "string.h"


/**
 * Function that lights all the LEDS
*/
void light_LEDS();



/**
 * Function that turn off all the LEDS
*/
void turn_off_LEDS();

/**
 * Function that fills the vector of the sine (len = lenght of the vector)
*/
void fill_sine(int len);


/**
 * Function that reads the input of the computer and turn the servo
*/
void read_computer(uint8* rxData);

/**
 * Function that activates the interruption
*/
void activate_sound();


/**
 * Function that switch modes
 * Take also care of switching the LEDS if the mode is switched
*/
void switch_to_gimball_mode();


/**
 * Function that switch to the test mode
 * Take also care of switching the LEDS if the mode is switched
*/
void switch_to_test_mode();


/**
 * function that changes the mode when a button is pressed
*/
void read_SW1();



/**
 * Function that reads the button 2 and change the testing mode 
*/
void read_SW2();
    
    
/**
 * Switch the mode
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

/**
 * Test the reaction on the buttons and keyboard
*/
void react();

/**
 * What is done in the testing mode
*/
void testing_mode();


/**
 * What is done in the glimball mode
*/
void gimball_mode();


/**
 * Execute the different modes depending on the mode
*/
void modes();


/**
 * Print the string on the LCD at the wanted position
*/
void print_screen(const char8 * string, int row, int column);

/*
    Initialisation de tous les composants de bases
*/
void initialize();