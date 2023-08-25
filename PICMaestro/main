/* 
 * File:   mainPICMaestro.c
 * Author: Christian Alessandro Campos López 21760 y
 * Christian Alejandro Cruz Rodas 21759
 *
 * Created on 21 de agosto de 2023, 11:58 PM
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
#include "USART.h"
//******************************************************************************
// Variables
//******************************************************************************
#define _XTAL_FREQ 500000     // Frecuencia de oscilador de 500kHz
uint8_t Tirar=0,bandera=10;
uint16_t Distancia, Peso;//dato a mandar
uint8_t Distancia_byte1, Distancia_byte2, Peso_byte1, Peso_byte2;

//******************************************************************************
// Prototipos de función
//******************************************************************************
void setup(void);
//******************************************************************************
// Vector de Interrupción
//******************************************************************************

void __interrupt() isr(void) {
    if(PIR1bits.RCIF)  {     //Interrupcion entrada EUSART
        Tirar= RCREG;   //Leer el valor de tirar del USART
        PIR1bits.RCIF=0;
    }
}
//******************************************************************************
// Función principal
//******************************************************************************

void main(void) {
    setup(); // Inicializar el microcontrolador

    while (1) {
        __delay_ms(2000);
        while (bandera==10){
            I2C_Master_Start();
            I2C_Master_Write(0x11); //Se le habla al primer PIC esperando que
            //lea el peso
            bandera = I2C_Master_Read(0); //Se guarda el valor leído de la 
            //bandera para verificar si ya leyó el peso
            I2C_Master_Stop();
            __delay_ms(100);
            
        }
        
        Peso_byte2=bandera;    
        I2C_Master_Start();
        I2C_Master_Write(0x11); //Se le habla al primer PIC para leer 2do byte
        //del peso
        Peso_byte1 = I2C_Master_Read(0); 
        I2C_Master_Stop();
        __delay_ms(100);
        
        I2C_Master_Start();
        I2C_Master_Write(0x20); //Se le habla al segundo PIC para empezar a leer
        //la distancia
        I2C_Master_Write(0); //Solo se le manda un valor para que comience
        I2C_Master_Stop();
        __delay_ms(3000);
        
        I2C_Master_Start();
        I2C_Master_Write(0x21); //Se le habla al segundo PIC esperando que
        //lea la distancia
        bandera = I2C_Master_Read(0); //Se guarda el valor leído de la 
        //bandera para verificar si ya leyó la distancia
        I2C_Master_Stop();
        __delay_ms(1000);
        
        while (bandera==0){
            I2C_Master_Start();
            I2C_Master_Write(0x21); //Se le habla al segundo PIC esperando que
            //lea la distancia
            bandera = I2C_Master_Read(0); //Se guarda el valor leído de la 
            //bandera para verificar si ya leyó el peso
            I2C_Master_Stop();
            __delay_ms(1000);
        }
        
        
        
        Distancia_byte2=bandera; //Se lee el primer Byte de la distancia    
        __delay_ms(100);
        I2C_Master_Start();
        I2C_Master_Write(0x21); //Se le habla al segundo PIC para leer el 2do
        //byte de la distancia
        Distancia_byte1 = I2C_Master_Read(0); 
        I2C_Master_Stop();
        __delay_ms(2000);
        PORTA = Distancia_byte1; // Mostrar byte alto en PORTA
        PORTD = Distancia_byte2; // Mostrar byte bajo en PORTD
        I2C_Master_Start();
        I2C_Master_Write(0x10); //Se le habla al primer PIC para indicar que
        //se leyó la distancia
        I2C_Master_Write(0); //Solo se le manda un valor para que siga 
        I2C_Master_Stop();
        __delay_ms(100);
        
        I2C_Master_Start();
        I2C_Master_Write(0x10); //Se le habla al primer PIC para indicar que
        //se leyó la distancia
        I2C_Master_Write(Distancia_byte1); //Primer byte distancia
        I2C_Master_Stop();
        __delay_ms(100);
        
        I2C_Master_Start();
        I2C_Master_Write(0x10); //Se le habla al primer PIC para indicar que
        //se leyó la distancia
        I2C_Master_Write(Distancia_byte2); //Segundo byte distancia
        I2C_Master_Stop();
        __delay_ms(2000);
        
        I2C_Master_Start();
        I2C_Master_Write(0x30); //Se le habla al tercer PIC para indicar que
        //mandar la distancia
        I2C_Master_Write(Distancia_byte1); //Primer byte distancia
        I2C_Master_Stop();
        __delay_ms(100);
        
        I2C_Master_Start();
        I2C_Master_Write(0x30); //Se le habla al tercer PIC para indicar que
        //valor es el segundo byte de la distancia
        I2C_Master_Write(Distancia_byte2); //Segundo byte distancia
        I2C_Master_Stop();
        __delay_ms(100);
        
        I2C_Master_Start();
        I2C_Master_Write(0x30); //Se le habla al tercer PIC para indicar que
        //el primer byte del peso
        I2C_Master_Write(Peso_byte1); //Primer byte peso
        I2C_Master_Stop();
        __delay_ms(100);
        
        I2C_Master_Start();
        I2C_Master_Write(0x30); //Se le habla al tercer PIC para indicar que
        //que reciba el segudno byte de peso
        I2C_Master_Write(Peso_byte2); //Segundo byte peso
        I2C_Master_Stop();
        __delay_ms(3000);
        
        
        I2C_Master_Start();
        I2C_Master_Write(0x31); //Se le habla al tercer PIC esperando que
        //termine de preparar el lanzamiento
        bandera = I2C_Master_Read(0); //Se guarda el valor leído de la 
        //bandera para verificar si ya está listo el lanzamiento
        I2C_Master_Stop();
        PORTB=bandera;
        
        //Mandar todos los datos a USART
        UART_Write(Distancia_byte1);
        __delay_ms(100);
        UART_Write(Distancia_byte2);
        __delay_ms(100);
        UART_Write(Peso_byte1);
        __delay_ms(100);
        UART_Write(Peso_byte2);
        __delay_ms(100);

        while (bandera!=5){
            I2C_Master_Start();
            I2C_Master_Write(0x31); //Se le habla al tercer PIC esperando que
            //termine de preparar el lanzamiento
            bandera = I2C_Master_Read(0); //Se guarda el valor leído de la 
            //bandera para verificar si ya está listo el lanzamiento
            I2C_Master_Stop();
            __delay_ms(200);
        }
        
        if (bandera==5){
            __delay_ms(100);
            I2C_Master_Start();
            I2C_Master_Write(0x10); //Se le habla al primer PIC para indicar que
            //el lanzamiento está listo
            I2C_Master_Write(0); //Solo se le manda un valor para que siga 
            I2C_Master_Stop();
            __delay_ms(2000);
            I2C_Master_Start();
            I2C_Master_Write(0x31); //Se le habla al tercer PIC esperando que
            //ya haya realizado el lanzamiento
            bandera = I2C_Master_Read(0); //Se guarda el valor leído de la 
            //bandera para verificar si se realizó el lanzamiento
            I2C_Master_Stop();
            __delay_ms(100);
            
       }
        while (bandera!=6){
            I2C_Master_Start();
            I2C_Master_Write(0x31); //Se le habla al tercer PIC esperando que
            //haya realizado el lanzamiento
            bandera = I2C_Master_Read(0); //Se guarda el valor leído de la 
            //bandera para verificar si ya se realizó el lanzamiento
            I2C_Master_Stop();
            __delay_ms(100);
            //Leer el valor de tirar del USART
            //Tirar=UART_Read();
            PIE1bits.RCIE = 1;//Habilitar interrupción USART 
            __delay_ms(100);
            PIE1bits.RCIE = 0;//Deshabilitar interrupción USART 
            __delay_ms(1000);
            UART_Write(2);
            
            if (Tirar==1){
                I2C_Master_Start();
                I2C_Master_Write(0x30); //Se le habla al tercer PIC para indicar que
                //que haga el lanzamiento
                I2C_Master_Write(Tirar); //Se le manda el valor de Tirar
                I2C_Master_Stop();
                __delay_ms(100);
                UART_Write(Tirar);
                Tirar=0;
            }
        }
        
        if (bandera==6){
            I2C_Master_Start();
            I2C_Master_Write(0x10); //Se le habla al primer PIC para indicar que
            //el lanzamiento se realizó
            I2C_Master_Write(0); //Solo se le manda un valor para que siga 
            I2C_Master_Stop();
            __delay_ms(5000);
        }
        
        I2C_Master_Start();
        I2C_Master_Write(0x10); //Se le habla al primer PIC para indicar que
        //se reinicia el programa
        I2C_Master_Write(0); 
        I2C_Master_Stop();
        __delay_ms(100);
        
        I2C_Master_Start();
        I2C_Master_Write(0x20); //Se le habla al segundo PIC para indicar que
        //se reinicia el programa
        I2C_Master_Write(0); 
        I2C_Master_Stop();
        __delay_ms(100);
        
        __delay_ms(200);
    }
}
//******************************************************************************
// Funciones secundarias
//******************************************************************************

void setup(void) {
    // Configuración de los puertos
    
    ANSEL = 0;
    ANSELH = 0;
    TRISA=0;
    TRISD=0;
    TRISB=0;
    PORTA=0;
    PORTB=0;
    PORTD=0;
    
    
    //Oscilador a 500kHz
    OSCCONbits.IRCF = 0b011;
    OSCCONbits.SCS = 1;
    
    //Configuracion EUSART
    UART_Init();
    // Habilitar interrupciones globales
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIR1bits.RCIF = 0;            //Bandera RX
    PIE1bits.RCIE = 0;            //INT EUSART RC
    
    //Inicializar Comuncación I2C
    I2C_Master_Init(1000);
    

}


