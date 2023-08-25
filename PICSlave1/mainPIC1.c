/* 
 * File:   mainPIC1.c
 * Author: Christian Alessandro Campos López 21760
 * Christian Alejandro Cruz Rodas 21759
 * Created on 19 de agosto de 2023, 01:37 AM
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
#include <pic16f887.h>
//******************************************************************************
// Librerías Propias
//******************************************************************************
#include "I2C.h"
#include "lcd8.h"
#include "char2ASC.h"
//******************************************************************************
// Variables
//******************************************************************************
#define _XTAL_FREQ 500000     // Frecuencia de oscilador de 500kHz
uint8_t z, dato_recibido, bandera=0, bandera2=0;
uint16_t Peso, Distancia;//Mandaremos el peso
char PesoR[3], DistanciaR[3];
uint8_t Distancia_byte1, Distancia_byte2, Peso_byte1, Peso_byte2;

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
            if (bandera==3){
                bandera=4; //En el maestro solo escribimos lo que sea para 
                //avisar que ya se encontró un objetivo y se leyó la distancia
                dato_recibido=SSPBUF;//Por si acaso xD
            }
            else if (bandera==4){
                Distancia_byte1 = SSPBUF;// Guardar el primer byte de la distancia
                bandera=5;
            }
            else if (bandera==5){
                Distancia_byte2 = SSPBUF;// Guardar el segundo byte de la distancia
                bandera=6; //Indicar que ya se terminó de leer la distancia
            }
            else if (bandera==7){
                bandera=8; //En el maestro solo escribimos lo que sea para indicar
                //que el lanzamiento está listo
                dato_recibido=SSPBUF; //Por si acaso xD
            }
            else if (bandera==8){
                bandera=9; //En el maestro solo escribimos lo que sea para indicar
                //que el lanzamiento se realizó
                dato_recibido=SSPBUF; //Por si acaso xD
            }
            else if (bandera==10){
                bandera=0; //En el maestro solo escribimos lo que sea para indicar
                //que ya terminó el programa y se reinicia
                bandera2=0;
                dato_recibido=SSPBUF; //Por si acaso xD
            }
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            if (bandera==0){
                SSPBUF = 10; //mandamos 0 indicando que no hemos leído el peso
                
            }
            else if (bandera==1){
                SSPBUF = Peso_byte2; //mandamos el primer byte del peso  
                bandera=2;
            }
            else if (bandera==2){
                SSPBUF = Peso_byte1; //mandamos el segundo byte del peso 
                bandera=3;
            }
            else if (bandera==3){
                SSPBUF = bandera; //mandamos la bandera indicando que aún 
                //No se ha encontrado objetivo xd
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
    //Mensaje introductorio xd
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("ProyectoDigital2");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("Catapulta");
    __delay_ms(1000);
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("De Cruz");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("y Campos");
    __delay_ms(1000);
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Aka: Los");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("Christians");
    __delay_ms(1000);
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Aka: Los");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("Christians");
    __delay_ms(1000);
    while (1) {
        if (bandera==0){ //Inicio del programa
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Coloque el");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("Proyectil"); //Pedir que se coloque el proyectil
            __delay_ms(1000);
            while(Peso==0){ //Esperar a que el sensor de peso lea un valor
                Lcd_Clear();
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("Esperando");
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("Y viendo");
                __delay_ms(1000);
                Peso=512; //Lectura del peso MODIFICAR 
                Peso_byte1 = (uint8_t)(Peso >> 8); // Byte alto
                Peso_byte2 = (uint8_t)(Peso & 0xFF); // Byte bajo
                convertir_a_ascii(Peso, PesoR);
            }
            //Al leer el peso se muestra en la LCD
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("El peso es de:");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_Char(PesoR[0]); // Mostrar centenas
            Lcd_Write_Char(PesoR[1]); // Mostrar decenas
            Lcd_Write_Char(PesoR[2]); // Mostrar unidades
            Lcd_Write_String("g");
            __delay_ms(1000);
            bandera=1; // Cambiamos a un modo donde en realidad no se realiza
            //nada en el Loop hasta que se envían todos los valores al maestro
            
        }
        if (bandera==3){ //Se espera a que se escanee el objetivo 
            //Y recibamos los valores de distancia
            if (bandera2==0){
                Lcd_Clear();
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("Escaneando");
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("Objetivo");
                bandera2=1;
            }
            __delay_ms(100);
        }
        if (bandera==6){ //Ya tenemos la distancia, ahora la mostramos
            Distancia = ((uint16_t)Distancia_byte1 << 8) + Distancia_byte2;
            PORTA = Distancia_byte1; // Mostrar byte alto en PORTA
            PORTB = Distancia_byte2; // Mostrar byte bajo en PORTD
            convertir_a_ascii(Distancia, DistanciaR);
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Objetivo");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("Encontrado");
            __delay_ms(2000);
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Distancia");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_Char(DistanciaR[0]); // Mostrar centenas
            Lcd_Write_Char(DistanciaR[1]); // Mostrar decenas
            Lcd_Write_Char(DistanciaR[2]); // Mostrar unidades
            Lcd_Write_String("cm");
            __delay_ms(3000);
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Preparando");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("Lanzamiento");
            bandera=7; //Esperamos a que se prepare el lanzamiento
        }
        if (bandera==8){ //El lanzamiento está listo
            //Ahora esperamos la señal
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Esperando");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("Señal.");
            __delay_ms(200);
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Esperando");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("Señal..");
            __delay_ms(200);
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Esperando");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("Señal...");
            __delay_ms(200);
        }
        if (bandera==9){ //Ya se mandó la señal: FUEGOOOO 
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Fuego!!!");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("POR NARNIA");
            __delay_ms(200);
            bandera=10; //Esperamos el reinicio 
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
    TRISB=0;
    TRISD=0;
    PORTA=0;
    PORTB=0;
    TRISCbits.TRISC7=0;
    TRISCbits.TRISC6=0;
    //Oscilador a 500kHz
    OSCCONbits.IRCF = 0b011;
    OSCCONbits.SCS = 1;
    
    //Configuración LCD
    Lcd_Init();
    //Comunicación I2C
    I2C_Slave_Init(0x10);
}


