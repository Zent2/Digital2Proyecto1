/* 
 * File:   mainPIC2.c
 * Author: Christian Alessandro Campos López 21760
 *Christian Alejandro Cruz Rodas 21759
 * Created on 19 de agosto de 2023, 03:03 AM
 */

//******************************************************************************
// Palabra de configuración
//******************************************************************************
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//******************************************************************************
// Librerías Generales
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
//******************************************************************************
// Librerías Propias
//******************************************************************************
#include "I2C.h"
#include "ultrasonic.h"
#include "Servo.h"
//******************************************************************************
// Variables
//******************************************************************************
#define _XTAL_FREQ 500000     // Frecuencia de oscilador de 500kHz
uint8_t z, valorA;
uint16_t Distancia=0;
uint8_t Distancia_byte1, Distancia_byte2, bandera=0;
uint8_t dato;//dato a mandar
uint8_t dato_recibido;//dato recibido

// Definiciones de pines
#define STEPPER_PORT PORTB     // Puerto donde está conectado el ULN2003
#define IN1 RB7                // Pin 1 del ULN2003
#define IN2 RB6                // Pin 2 del ULN2003
#define IN3 RB5                // Pin 3 del ULN2003
#define IN4 RB4                // Pin 4 del ULN2003
//******************************************************************************
// Prototipos de función
//******************************************************************************
void setup(void);
void stepperClockwiseStep();
void stepperCounterClockwiseStep();
void rotateClockwise(int degrees);
void rotateCounterClockwise(int degrees);
//******************************************************************************
// Vector de Interrupción
//******************************************************************************

void __interrupt() isr(void) {
    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            __delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            __delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            if (bandera==0){
                bandera=1; //En el maestro solo escribimos lo que sea para 
                //avisar que se tiene que comenzar el escaneo
                dato_recibido=SSPBUF;//Por si acaso xD
            }
            else if (bandera==4){
                bandera=0; //En el maestro solo escribimos lo que sea para 
                //avisar que se hizo un reinicio
                dato_recibido=SSPBUF;//Por si acaso xD
            }
            
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            if (bandera==1){
                SSPBUF = 0; //Mandamos la bandera indicando que
                //Aún no se ha encontrado el objetivo
            }
            if (bandera==2){
                SSPBUF = Distancia_byte2; //Mandamos el primer byte de la distancia
                bandera=3;
            }
            else if (bandera==3){
                SSPBUF = Distancia_byte1; //Mandamos el segundo byte de la distancia
                bandera=4;
            }
            
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}
//******************************************************************************
// Función principal
//******************************************************************************

void main(void) {
    setup(); // Inicializar el microcontrolador
    //Distancia = ultrasonic_measure_distance();
    valorA=45;
    Distancia=0;
    while (1) {
        //Sensor de DISTANCIA
        //PORTA=ultrasonic_measure_distance();
        /*
        rotateClockwise(90);           // Girar 90 grados a la derecha
        __delay_ms(1000);              // Esperar un segundo
        rotateCounterClockwise(90);    // Girar 90 grados a la izquierda
        __delay_ms(1000);              // Esperar un segundo
    */
        if (bandera==1){ //Iniciamos la busqueda del objetivo
            // Gira el motor en sentido horario durante un tiempo
            
            if (Distancia<10 || Distancia>120){ //Comprobamos en cada giro que 
                //la distancia cumpla los requisitos 
                valorA=0;
                rotateCounterClockwise(90);
                __delay_ms(1000); // Puedes ajustar el tiempo según sea necesario
                
                Distancia = ultrasonic_measure_distance(); // Distancia total
                Distancia_byte1 = (uint8_t)(Distancia >> 8); // Byte alto
                Distancia_byte2 = (uint8_t)(Distancia); // Byte bajo
            }
            if (Distancia<10 || Distancia>120){ 
                valorA=45;
                rotateClockwise(90);
                __delay_ms(1000); // Puedes ajustar el tiempo según sea necesario
                Distancia = ultrasonic_measure_distance(); // Distancia total
                Distancia_byte1 = (uint8_t)(Distancia >> 8); // Byte alto
                Distancia_byte2 = (uint8_t)(Distancia); // Byte bajo
            }
            if (Distancia<10 || Distancia>120){ 
                valorA=90;
                rotateClockwise(90);
                __delay_ms(1000); // Puedes ajustar el tiempo según sea necesario
                Distancia = ultrasonic_measure_distance(); // Distancia total
                Distancia_byte1 = (uint8_t)(Distancia >> 8); // Byte alto
                Distancia_byte2 = (uint8_t)(Distancia); // Byte bajo
            }
            
            if (Distancia<10 || Distancia>120){ 
                valorA=45;
                rotateCounterClockwise(90);
                __delay_ms(1000); // Puedes ajustar el tiempo según sea necesario
                Distancia = ultrasonic_measure_distance(); // Distancia total
                Distancia_byte1 = (uint8_t)(Distancia >> 8); // Byte alto
                Distancia_byte2 = (uint8_t)(Distancia); // Byte bajo
            }
            
            
            else {
                
                PORTB=0x00;
                
                PORTA = Distancia_byte1; // Mostrar byte alto en PORTA
                PORTD = Distancia_byte2; // Mostrar byte bajo en PORTD
                bandera=2; 
            }
        }
        if (bandera==4) {//Ahora esperamos para el reinicio
            move_servo(valorA);
            __delay_ms(100);
        }
    
        
    
        
        
    }
}
//******************************************************************************
// Funciones secundarias
//******************************************************************************

void setup(void) {
    // Configuración de los puertos
    ANSEL = 0;
    ANSELH = 0;
    //Control Stepper
    TRISB=0;
    PORTB=0;
    //Mostrar valor distancia
    TRISA=0;
    TRISD=0;
    TRIG=0;
    //Oscilador a 500kHz
    OSCCONbits.IRCF = 0b011;
    OSCCONbits.SCS = 1;

    //Comunicación I2C
    I2C_Slave_Init(0x20);
    //Iniciamos sensor de distancia
    ultrasonic_init();
    servo_init();
    move_servo(0);


}


// Función para girar el motor en sentido horario (un paso)
void stepperClockwiseStep() {
    STEPPER_PORT = 0x90;  // 1001
    __delay_ms(10);
    STEPPER_PORT = 0x30;  // 0011
    __delay_ms(10);
    STEPPER_PORT = 0x60;  // 0110
    __delay_ms(10);
    STEPPER_PORT = 0xC0;  // 1100
    __delay_ms(10);
}

// Función para girar el motor en sentido antihorario (un paso)
void stepperCounterClockwiseStep() {
    STEPPER_PORT = 0xC0;  // 1100
    __delay_ms(10);
    STEPPER_PORT = 0x60;  // 0110
    __delay_ms(10);
    STEPPER_PORT = 0x30;  // 0011
    __delay_ms(10);
    STEPPER_PORT = 0x90;  // 1001
    __delay_ms(10);
}

// Función para girar el motor una cantidad de grados a la derecha
void rotateClockwise(int degrees) {
    int steps = (int)((float)degrees / 360.0 * 32.0); // 512 pasos por vuelta
    for (int i = 0; i < steps; i++) {
        stepperClockwiseStep();
    }
}

// Función para girar el motor una cantidad de grados a la izquierda
void rotateCounterClockwise(int degrees) {
    int steps = (int)((float)degrees / 360.0 * 32.0); // 512 pasos por vuelta
    for (int i = 0; i < steps; i++) {
        stepperCounterClockwiseStep();
    }
}

