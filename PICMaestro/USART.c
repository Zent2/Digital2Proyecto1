/* 
 * File:   USART.c
 * Author: Christian Alessandro Campos López 21760
 *
 * Created on 23 de agosto de 2023, 10:49 AM
 */
#include "USART.h"
#define _XTAL_FREQ 500000     // Frecuencia de oscilador de 500kHz
//******************************************************************************
// Variables Internas
//******************************************************************************
char variable;
//******************************************************************************
// Inicio de comunicación Serial
//******************************************************************************
void UART_Init(){
    TRISC6 = 0;  // Configurar TX como salida
    TRISC7 = 1;  // Configurar RX como entrada
    TXSTAbits.SYNC = 0;             //Modo asincrono
    TXSTAbits.BRGH = 1;             //High speed baud rate

    BAUDCTLbits.BRG16 = 1;          //16-bit Baud Rate
    SPBRG = 12;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;             //Serial port enable
    RCSTAbits.RX9 = 0;              //8 bits de datos
    RCSTAbits.CREN = 1;             //Habilitar para recibir datos
    
    TXSTAbits.TXEN = 1;             //Habilitar para enviar datos
}
//******************************************************************************
// Escribir en la comunicación serial
//******************************************************************************
void UART_Write(uint8_t data){
    while (!TXIF);  // Esperar a que el registro de transmisión esté vacío
    TXREG = data;   // Enviar el dato
}
//******************************************************************************
// Leer de la comunicación Serial
//******************************************************************************
char UART_Read(){
    while (!RCIF);  // Esperar a recibir un dato
    return RCREG;   // Retornar el dato recibido
}
//******************************************************************************
// Función para enviar cadena
//******************************************************************************
void enviar_cadena(char* cadena){
    int i = 0;
    while(*(cadena+i) != '\0') { //Mientras no se alcance el final de la cadena
        while(!PIR1bits.TXIF); //Esperar a que el registro de transmisión esté disponible
        TXREG = *(cadena+i); //Enviar el carácter actual
        i++; //Incrementar el índice para apuntar al siguiente carácter
    }
    __delay_ms(100);
    TXREG=32; //Espacio
}