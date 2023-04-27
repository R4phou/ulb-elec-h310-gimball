/**
* C file: write all the functions that are listed in the header file corresponding
* All imports and include should be done in header file
*/
#include "functions.h"
#define MIN_SERVO 500
#define MAX_SERVO 2508
#define SERVO_ANGLE (MAX_SERVO - MIN_SERVO)/180
#define pi 3.1415


int mode = 0;  // 1 = glimball | 0 = test
int test_mode = 0; // potentiomètre = 1 | keypad = 0


int iterator = 0;
float sin_value;
float sin_wave[100];

CY_ISR(ISR_adress){
    iterator++;
    VDAC_SetValue((uint8)(sin_wave[iterator]));
    if (iterator>=100)iterator=0;
    Timer_ReadStatusRegister();
}


void initialize(){
    LCD_Start();
    LCD_ClearDisplay();
    PWM_Init();
    PWM_Start();
    Mux_Start();
    ADC_Start();
    keypadInit();
    Timer_Start();
    VDAC_Start();
    fill_sine(100);
}

void fill_sine(int len){
    for (int i =0; i < len; i++){
        sin_wave[i] = sin(i*2*pi/len)*128 + 128;
    }
}

void light_LEDS(){
    LED1_Write(1);
    LED2_Write(1);
    LED3_Write(1),
    LED4_Write(1);
}

void turn_off_LEDS(){
    LED1_Write(0);
    LED2_Write(0);
    LED3_Write(0),
    LED4_Write(0);
}

void switch_to_glimball_mode(){
    mode = 1;
    light_LEDS();
    const char8* var = "Glim";
    print_screen(var);
}

void switch_to_test_mode(){
    mode = 0;
    turn_off_LEDS();
    const char8* var = "Test";
    print_screen(var);
}


void read_SW1(){
    if (SW1_Read()){
        CyDelay(200);
        switch_mode();
    }
}

void read_SW2(){
    if (!mode && SW2_Read()){
        CyDelay(200);
        if (test_mode) {
            test_mode = 0;
            const char8* var = "keypad";
            print_screen(var);
        }
        else if (!test_mode){
            test_mode = 1;
            const char8* var = "pot";
            print_screen(var);
        } 
    }
}

void switch_mode(){
    if (mode){
        switch_to_test_mode();
    }
    else if (!mode){
        switch_to_glimball_mode();
    }
    else{
        error();
    }
}

void error(){
    LCD_Position(0,0);
    LCD_PrintString("ERROR");
    CyDelay(100000);
}

void activate_sound(){
    ISR_SOUND_StartEx(ISR_adress);
}

void rotate_left(){
    uint16 pos = PWM_ReadCompare();
    LCD_ClearDisplay();
    LCD_PrintDecUint16(pos);
    pos -= 10*SERVO_ANGLE;
    if (pos < MIN_SERVO) pos = MIN_SERVO;
    PWM_WriteCompare(pos);
    CyDelay(50);
}

void rotate_right(){
    uint16 pos = 0;
    pos = PWM_ReadCompare();
    LCD_ClearDisplay();
    LCD_PrintDecUint16(pos);
    pos += 10*SERVO_ANGLE;
    if (pos > MAX_SERVO) pos = MAX_SERVO;
    PWM_WriteCompare(pos);
    CyDelay(50);
    
}

void react_to_keypad(){
    uint8_t value = keypadScan();
    if (value != 'z'){
        if ( value == '1'){
            CyDelay(200);
            switch_mode();
        } 
        else if (!mode && !test_mode){
            if (value == '2'){
                CyDelay(100);
                rotate_left();
            }
            else if(value == '3'){
                CyDelay(100);
                rotate_right();
            }
        }
    }
}

void print_screen(const char8 * string){
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString(string);
}

void testing_mode(){
    if (!test_mode) return;
    uint32_t potval = 0;
    Mux_Select(0);
    CyDelay(10);
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT))  potval = ADC_GetResult32();
    potval = MIN_SERVO + (potval*(MAX_SERVO-MIN_SERVO))/(float)(0xFFFF); // transférer le max du potentiomètre en min et max du CMP du servo
    PWM_WriteCompare(potval);
    CyDelay(50);
}

void glimball_mode(){
    uint32_t x=0;//y,z = 0;
    
    Mux_Select(1); //Selection du premier channel
    CyDelay(10);
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT))  x = ADC_GetResult32();
    Mux_Select(2); ///Selection du 2eme channel
    CyDelay(10);
    ADC_StartConvert();
    //if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT))  y = ADC_GetResult32();
    Mux_Select(3); //Selection du 3eme channel
    CyDelay(10);
    ADC_StartConvert();
    //if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT))  z = ADC_GetResult32();
    
    LCD_ClearDisplay();
    LCD_Position(0,0);
    //Conversion des resultats
    x = (x*3000/(0xFFFF)); //Donner la valeur entre 0 et 3000 mV
    //y = (y*3000/(0xFFFF));
    //z = (z*3000/(0xFFFF));
    //Arrondi des valeures
    //float x_print = x / 1000.0;
    //float y_print = y / 1000.0;
    //float z_print = z / 1000.0;
    
    LCD_PrintNumber(x);
    LCD_PrintString(" mV");
    //LCD_PrintNumber(y);
    //LCD_PrintString(" ");
    //LCD_PrintNumber(z);
   
}

void react(){
    react_to_keypad();
    read_SW1();
    if (!mode) read_SW2();
}

void modes(){   
   if (!mode){
        testing_mode();
    }
    else{
        glimball_mode();
    }
}