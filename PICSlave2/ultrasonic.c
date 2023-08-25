/* 
 * File:   ultrasonic.c
 * Author: Christian Alessandro Campos López 21760
 *
 * Created on 19 de agosto de 2023, 11:46 AM
 */
#include "ultrasonic.h"


#include "ultrasonic.h"
#include <xc.h>

#define _XTAL_FREQ 500000     // Frecuencia del oscilador (8 MHz)

//******************************************************************************
// Inicio del sensor ultrasónico
//******************************************************************************
void ultrasonic_init(void) {
    // Configuración de puertos
    TRIGPin=0;
    ECHOPin=1;
}

//******************************************************************************
// Cálculo de la distancia leída por el sensor
//******************************************************************************
float ultrasonic_measure_distance(void) {
    float low_time, high_time;
    float distance;
    float distancia_final;
    uint16_t contador=0; 
    
    TRIG=1;
    __delay_us(10);
    TRIG=0;
    while (ECHO==0);
    while (ECHO==1){
        contador++;
    }
    distancia_final=16*(contador)/10.5;
    
    return distancia_final;
}



