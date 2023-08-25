/* 
 * File:   Servo.c
 * Author: Christian Alessandro Campos López 21760
 *
 * Created on 19 de agosto de 2023, 08:10 PM
 */
#include "Servo.h"
//******************************************************************************
// Variables Internas
//******************************************************************************
char variable;

// Definiciones de constantes
#define _XTAL_FREQ 500000      // Frecuencia del oscilador interno
//******************************************************************************
// Función para inicializar el módulo CCP1 y el timer
//******************************************************************************
void servo_init() {
    TRISCbits.TRISC2 = 0;   // Configura el pin RC2 (CCP1) como salida
    CCP1CONbits.CCP1M = 0b1100; // Configura CCP1 como modo PWM
    TMR2 = 0; // Reiniciar timer2
    PR2 = 155.25; // Valor de período para 50Hz (500kHz / (4 * (PR2 + 1)))
    CCPR1L=11.6;
    CCPR2L=11.6;
    
    T2CON = 0b00000111; // Timer2 configurado para 1:16
}


//******************************************************************************
// Función para mover el servomotor a una posición específica (0-180 grados)
//******************************************************************************
void move_servo(uint8_t degrees) {
    if (degrees < 0) {
        degrees = 0;
    } else if (degrees > 180) {
        degrees = 180;
    }
    
    uint16_t duty_cycle = (uint16_t)(31.25 + degrees * 0.1736); // Convierte grados a ciclo de trabajo
    CCPR1L = (duty_cycle >> 2); // Bits 9-2 del ciclo de trabajo
    CCP1CONbits.DC1B = (duty_cycle & 0x03); // Bits 1-0 del ciclo de trabajo
    __delay_ms(20); // Pequeña pausa para que el servo alcance la posición
}

