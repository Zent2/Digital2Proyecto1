/* 
 * File:   lcd8.c
 * Author: Christian Alessandro Campos L�pez 21760
 *
 * Created on 24 de julio de 2023, 01:20 PM
 */
/* 
 * File:   lcd.c
 * Author: Christian Alessandro Campos L�pez 21760
 *
 * Created on 22 de julio de 2023, 09:16 PM
 */
#include "lcd8.h"
//******************************************************************************
// Variables Internas
//******************************************************************************
#define _XTAL_FREQ 500000    // Frecuencia de oscilador de 8MHz

//******************************************************************************
// Lcd_Port: Escoge la posici�n del LCD
//******************************************************************************
void Lcd_Port(char a) {
    PORTD=a;
}
//******************************************************************************
// Lcd_Cmd: Habilita y desabilita el LCD para mandar datos
//******************************************************************************
void Lcd_Cmd(char a) {
    RS = 0; // => RS = 0 // Dato en el puerto lo va interpretar como comando
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}
//******************************************************************************
// Lcd_Clear: Limpia el LCD
//******************************************************************************
void Lcd_Clear(void) {
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}
//******************************************************************************
// Lcd_Set_Cursor: Escoge la posici�n del LCD
//******************************************************************************
void Lcd_Set_Cursor(char a, char b) {
    char temp, z, y;
    if (a == 1) {
        temp = 0x80 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    } else if (a == 2) {
        temp = 0xC0 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        Lcd_Cmd(0xC0);
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    }
    
}
//******************************************************************************
// Lcd_Init: Inicia la configuraci�n del LCD
//******************************************************************************
void Lcd_Init(void) {
    Lcd_Port(0x00); // Iniciar el puerto de datos en 0
    __delay_ms(20);
    Lcd_Cmd(0x38); // Configurar la interfaz a 8 bits de 2 l�neas
    __delay_ms(5);
    Lcd_Cmd(0x0C); // Encender el LCD y apagar el cursor
    __delay_ms(5);
    Lcd_Cmd(0x01); // Limpiar la pantalla y poner el cursor en la posici�n inicial
    __delay_ms(5);
    Lcd_Cmd(0x06); // Modo de entrada: mover el cursor hacia la derecha despu�s de escribir
    __delay_ms(5);
}
//******************************************************************************
// Lcd_Write_Char: Manda los valores que se quieren desplegar (char)
//******************************************************************************
void Lcd_Write_Char(char a) {
    RS = 1; // RS = 1 (Dato)
    Lcd_Port(a);
    EN = 1;
    __delay_us(40);
    EN = 0;
}
//******************************************************************************
// Lcd_Write_String: Manda los valores que se quieren despelgar (cadenas)
//******************************************************************************
void Lcd_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]);
}

//******************************************************************************
// Lcd_Shift_Right: Genera movimiento hacia la derecha
//******************************************************************************
void Lcd_Shift_Right(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}
//******************************************************************************
// Lcd_Shift_Right: Genera movimiento hacia la izquierda
//******************************************************************************
void Lcd_Shift_Left(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}