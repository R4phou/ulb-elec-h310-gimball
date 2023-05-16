/**
* C file: write all the functions that are listed in the header file corresponding
* All imports and includes should be done in matching header file
*/
#include "functions.h"
#define MIN_SERVO 500
#define MAX_SERVO 2500
#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define SERVO_ANGLE (MAX_SERVO - MIN_SERVO)/(MAX_ANGLE - MIN_ANGLE)
#define pi 3.1415
#define MIN_ACC 18000
#define MAX_ACC 29000
#define ZERO_ACC (MAX_ACC-MIN_ACC)/2
#define N_MAX 10000
#define STEP_MIN 40
#define STEP_MAX 400
#define DELAY 50
#define WINDOW 3


/* Global variables */

int mode = 0;  // test = 0 | gimball = 1
int test_mode = 0; // keypad = 0 | Potentiometer = 1 | joystick = 2


int iterator = 0; // variables needed for the sound
float sin_wave[N_MAX];
int step = 0;

uint8 angle_buffer[10] = {0}; // Variables needed for the moving_average
int buffer_index = 0;

uint16 pos = (MAX_SERVO-MIN_SERVO)/2; // position of the servo



/*                      Interruptions handlers               */

CY_ISR(isr_sound_handler){ // Interruption for the sound (and the modification of the position of the servo)
    iterator += step;
    VDAC_SetValue((uint8)(sin_wave[iterator]));
    if (iterator>=N_MAX)iterator=0;
    PWM_WriteCompare(pos); // rotates the servo
    Timer_ReadStatusRegister();
}

CY_ISR(isr_uart_handler){ // Receive instructions from Computer
    uint8 rxData; // Value readed from computer
    uint8_t status;
    do {
        status = UART_ReadRxStatus();
    if ((status & UART_RX_STS_PAR_ERROR) |
        (status & UART_RX_STS_STOP_ERROR) |
        (status & UART_RX_STS_BREAK) |
        (status & UART_RX_STS_OVERRUN)) {
    // Parity , framing , break or overrun error
        print_screen("UART err", 1, 0);
    }
    if ((status & UART_RX_STS_FIFO_NOTEMPTY)!=0){
        rxData = UART_GetChar();
        UART_PutChar(rxData);
        read_computer(&rxData);
    }
    }while((status & UART_RX_STS_FIFO_NOTEMPTY) != 0);
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
    ISR_SOUND_StartEx(isr_sound_handler);
    fill_sine(N_MAX);
    print_screen("Keypad : ", 0, 0);
}




/*          Useful functions                                */

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

void error(){
    print_screen("ERROR", 0, 0);
    CyDelay(100000); // Error freezes everything
}

void rotate_servo(int angle){
    uint16 temp_pos = MIN_SERVO + angle*SERVO_ANGLE;
    if (temp_pos > MAX_SERVO) temp_pos = MAX_SERVO;
    if (temp_pos < MIN_SERVO) temp_pos = MIN_SERVO;
    pos = temp_pos;
}

void fill_sine(int len){
    for (int i =0; i < len; i++){
        sin_wave[i] = sin(i*2*pi/len)*8 + 8;
    }
}

void print_screen(const char8 * string, int row, int column){
    LCD_ClearDisplay();
    LCD_Position(row,column);
    LCD_PrintString(string);
    UART_PutString(string);
}

void read_computer(uint8* data){
    if (*data == 't') switch_mode();
    if (!mode && !test_mode){
        if (*data == 'l') rotate_servo(-10);
        else if(*data == 'r') rotate_servo(10);
    }
}

/*          Accelerometer information to angle              */
void get_angle(uint16* accX){
    // Read the accelerometer
    Mux_Select(1); //Selection of the first channel
    CyDelay(10); // For the time needed to select the good MUX gate
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)) *accX = ADC_GetResult16();
    
    // Verify that the value of the accelerometer does not go in overflow (avoid ghost moves)
    *accX = (*accX > MAX_ACC) ? MAX_ACC : ((*accX < MIN_ACC) ? MIN_ACC : *accX);
    
    // Modfify the step (for the sound)
    step = STEP_MIN + (*accX-MIN_ACC)*(STEP_MAX-STEP_MIN)/(float)(MAX_ACC-MIN_ACC);
    
    // Change the value of the accelerometer into an angle
    angle_buffer[buffer_index] =  MIN_ANGLE + (*accX-MIN_ACC)*(MAX_ANGLE-MIN_ANGLE)/(float)(MAX_ACC-MIN_ACC);
    uint8 angle = (uint8) moving_average();
    buffer_index = (buffer_index + 1) % WINDOW;
    
    // Print the angle on the LCD + UART
    print_angle(&angle);
}

float moving_average() {
    float sum = 0;
    for (int i = 0; i < WINDOW; i++) {
        sum += angle_buffer[i];
    }
    return sum / WINDOW;
}

void print_angle(uint8* angle){
    LCD_Position(1,0);
    char x_char[12];
    sprintf(x_char, "%.3u", (unsigned int) *angle);
    LCD_PrintString(x_char);
    strcat(x_char, "\n");
    UART_PutString(x_char);
}

/*          Test mode               */

void testing_mode(){
    read_SW2();
    switch (test_mode) {
    case 0:
        test_keyboard();
        break;
    case 1:
        test_pot();
        break;
    case 2:
        test_joystick();
        break;
    }
}

void test_pot(){
    uint16 potval = 0;
    Mux_Select(0);
    CyDelay(10); // For the time needed to select the good MUX gate
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT))  potval = ADC_GetResult32();
    pos =  MIN_SERVO + (potval*(MAX_SERVO-MIN_SERVO))/(float)(0xFFFF);
}

void test_keyboard(){
    uint8_t value = keypadScan();
    if (value != 'z'){
        if (value == '2'){
            CyDelay(100); // For the sensibility of the button
            rotate_servo(-10);
        }
        else if(value == '3'){
            CyDelay(100); // For the sensibility of the button
            rotate_servo(10);
        }
    }
}

void test_joystick(){
    int16 joyval = 0;
    Mux_Select(2);
    CyDelay(10); // For the time needed to select the good MUX gate
    ADC_StartConvert();
    if (ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)){
        int32_t result = ADC_GetResult32();
        if (result > joyval) rotate_servo(1);
        else if (result < joyval) rotate_servo(-1);
        joyval = result;
    }
    pos = MIN_SERVO + (joyval*(MAX_SERVO-MIN_SERVO))/(float)(0xFFFF);
}


/*          Gimball mode               */

void gimball_mode(uint16* accX){
    // Regulation
    float k; // Proportionality constant to be determined
    if (*accX < ZERO_ACC) {
        k = 2.5;
    }else{
        k = 1.25;
    }
    pos = pos + k*(*accX - ZERO_ACC)*90/(MAX_ACC-MIN_ACC); // pos = last_pos + k*error
}


/*          Reaction to keypad and buttons                */

void react(){
    if(read_SW1() || react_to_keypad()) switch_mode();
}

int react_to_keypad(){
    uint8_t value = keypadScan();
    if (value != 'z'){
        if (value == '1'){
        CyDelay(200); // For the sensibility of the button
        return 1;
        }
    }
    return 0;
}

int read_SW1(){
    if (SW1_Read()){
        CyDelay(200); // For the sensibility of the button
        return 1;
    }
    return 0;
}

void read_SW2(){
    if (!mode && SW2_Read()){
        CyDelay(200); // For the sensibility of the button
        if (test_mode == 2) {
            test_mode = 0;
            print_screen("keypad ", 0, 0);
        }
        else if (test_mode == 0){
            test_mode = 1;
            print_screen("pot ", 0, 0);
        }
        else if (test_mode==1){
            test_mode = 2;
            print_screen("joystick ", 0, 0);
        }
    }
}


/*          Determination of the mode                */

void switch_to_gimball_mode(){
    mode = 1;
    light_LEDS();
    print_screen("Glim : ", 0, 0);
}

void switch_to_test_mode(){
    mode = 0;
    turn_off_LEDS();
    print_screen("Test : ", 0, 0);
}

void switch_mode(){
    mode ? switch_to_test_mode():switch_to_gimball_mode();
}

void modes(){
    uint16 pos = 0;
    get_angle(&pos);
    mode ? gimball_mode(&pos):testing_mode();
}