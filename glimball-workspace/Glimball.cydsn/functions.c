/**
* C file: write all the functions that are listed in the header file corresponding
* All imports and include should be done in header file
*/
#include "functions.h"
#define MIN_SERVO 500
#define MAX_SERVO 2500
#define SERVO_ANGLE (MAX_SERVO - MIN_SERVO)/180
#define pi 3.1415
#define MIN_ACC 18800
#define MAX_ACC 27400
#define N_MAX 2000
#define STEP_MIN 10
#define STEP_MAX 100


int mode = 0;  // 1 = glimball | 0 = test
int test_mode = 0; // potentiomètre = 1 | keypad = 0


int iterator = 0;
float sin_value;
float sin_wave[N_MAX];
int step = 0;



CY_ISR(isr_sound_handler){
    /* uint32_t x=0;//y,z = 0;
    uint32_t step = 0;
    
    Mux_Select(1); //Selection du premier channel
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT))  x = ADC_GetResult32();
    
    step = STEP_MIN + (x-MIN_ACC)*(STEP_MAX-STEP_MIN)/(float)(MAX_ACC-MIN_ACC); // transférer le max du potentiomètre en min et max du CMP du servo
    
    char step_char[12];
    sprintf(step_char, "step : %.3u \n",(unsigned int) step);
    UART_PutString(step_char); */
    
    iterator += step;
    VDAC_SetValue((uint8)(sin_wave[iterator]));
    if (iterator>=N_MAX)iterator=0;
    Timer_ReadStatusRegister();
}

CY_ISR(isr_uart_handler){ // Receive instructions from Computer
    uint8 rxData; // Value readed from computer
    uint8_t status = 0;
    do {
    status = UART_ReadRxStatus();
    if ((status & UART_RX_STS_PAR_ERROR ) |
        ( status & UART_RX_STS_STOP_ERROR ) |
        ( status & UART_RX_STS_BREAK ) |
        ( status & UART_RX_STS_OVERRUN ) ) {
    // Parity , framing , break or overrun error
        LCD_Position(1 ,0) ;
        LCD_PrintString(" UART err ") ;
    }
    if ((status & UART_RX_STS_FIFO_NOTEMPTY)!=0){
        rxData = UART_GetChar();
        UART_PutChar(rxData);
        read_computer(&rxData);
        
        //ICI appeller fonction qui fait tourner du rxData
    }
    }while ((status & UART_RX_STS_FIFO_NOTEMPTY) != 0);
}

void read_computer(uint8* data){
    if (!test_mode && !mode){
        if (strcmp((const char *) data, "l") == 0) {
            rotate_left();
        }
        else if(strcmp((const char *) data, "r") == 0){
           rotate_right();
        }
    }
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
    UART_Start();
    isr_uart_StartEx(isr_uart_handler);
    
    fill_sine(N_MAX);
}

void fill_sine(int len){
    for (int i =0; i < len; i++){
        sin_wave[i] = sin(i*2*pi/len)*8 + 8;
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
    ISR_SOUND_StartEx(isr_sound_handler);
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
    uint32_t servo = 0; // Envoyé au servomoteur (mV)
    
    Mux_Select(0);
    CyDelay(10);
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT))  potval = ADC_GetResult32();
    
    servo = MIN_SERVO + (potval*(MAX_SERVO-MIN_SERVO))/(float)(0xFFFF); // transférer le max du potentiomètre en min et max du CMP du servo
    
    PWM_WriteCompare(servo);
    CyDelay(50);
}

void glimball_mode(){
    uint32_t x=0;//y,z = 0;
    uint32_t servoX = 0; // Envoyé au servomoteur (mV)
    
    Mux_Select(1); //Selection du premier channel
    CyDelay(10);
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT))  x = ADC_GetResult32();
    
    LCD_ClearDisplay();
    LCD_Position(0,0);
    //Conversion des resultats
    
    servoX = MIN_SERVO + (x-MIN_ACC)*(MAX_SERVO-MIN_SERVO)/(float)(MAX_ACC-MIN_ACC); // transférer le max du potentiomètre en min et max du CMP du servo
    PWM_WriteCompare(servoX);
    CyDelay(40);
    
    step = STEP_MIN + (x-MIN_ACC)*(STEP_MAX-STEP_MIN)/(float)(MAX_ACC-MIN_ACC);
    
    LCD_PrintNumber(x);
    LCD_PrintString(" mV");
    
    char x_char[12];
    sprintf(x_char, "pot : %.3u \n",(unsigned int) x);
    UART_PutString(x_char);
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