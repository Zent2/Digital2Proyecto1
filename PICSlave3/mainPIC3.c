
/* 
 * File:   mainPIC3.c
 * Author: Christian Alessandro Campos López 21760
 *Christian Alejandro Cruz Rodas 21759
 * Created on 19 de agosto de 2023, 03:04 AM
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
#include "Servo.h"

//******************************************************************************
// Variables
//******************************************************************************
#define _XTAL_FREQ 500000     // Frecuencia de oscilador de 500kHz
uint8_t z, Angulo_Inicial=45;
uint8_t dato;//dato a mandar
uint8_t dato_recibido;//dato recibido
uint16_t Distancia, Peso;//dato a mandar
uint8_t Distancia_byte1, Distancia_byte2, Peso_byte1, Peso_byte2;
uint8_t bandera=0, Tirar=0; //Bandera para los datos

//******************************************************************************
// Prototipos de función
//******************************************************************************
void setup(void);
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
            dato_recibido = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            if (bandera==0){
                Distancia_byte1=dato_recibido;
                bandera=1;
            }
            else if (bandera==1){
                Distancia_byte2=dato_recibido;
                bandera=2;
            }
            else if (bandera==2){
                Peso_byte1=dato_recibido;
                bandera=3;
            }
            else if (bandera==3){
                Peso_byte2=dato_recibido;
                bandera=4;
                // Combinar los bytes para obtener las variables completas de 16 bits
                Distancia = ((uint16_t)Distancia_byte1 << 8) | Distancia_byte2;
                Peso= ((uint16_t)Peso_byte1 << 8) | Peso_byte2;
            }
            else if (bandera==5){
                Tirar=dato_recibido;
            }
            else if (bandera==6){
                //El lanzamiento se realizó
                move_servo(0);
                bandera=0; //Reiniciamos 
            }
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = bandera; //Enviamos la bandera para ver si ya es 5
            //Si ya es 5 se indica que está listo para esperar la señal de lanzamiento
            //Al ser 6 se indica que el lanzamiento ya se logró
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
    move_servo(0);
    __delay_ms(10);
    while (1) {   
        
        
        
        if (bandera==4){
            //Ecuación de angulo inicial
            
            Angulo_Inicial=90-57.3*((Distancia/100)*(Peso/1000)*(Distancia/100))/(0.89*(Peso+40)/1000);
            PORTD=Angulo_Inicial;
            move_servo(Angulo_Inicial);
            //Aquí se coloca el servo en el ángulo inicial 
            bandera=5;
        }
        if ((Tirar||RB7) && bandera==5){
            move_servo(90);
            //Aquí se suelta el servo para que este a 90 grados
            bandera=6;
            __delay_ms(1000);
            
        }
        if (bandera==6){
            move_servo(0);
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
    TRISA=0;
    TRISD=0;
    PORTA=0;
    PORTD=0;
    TRISBbits.TRISB7=1;
    RB7=0;
    //Oscilador a 500kHz
    OSCCONbits.IRCF = 0b011;
    OSCCONbits.SCS = 1;

    //Comunicación I2C
    I2C_Slave_Init(0x30);
    //Iniciar control del servo
    
    servo_init();
    move_servo(0);


}


