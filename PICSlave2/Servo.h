/* 
 * File:   Servo.h
 * Author: Christian Alessandro Campos López 21760
 *
 * Created on 19 de agosto de 2023, 08:10 PM
 */
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SERVO_H
#define	SERVO_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
// Definiciones de constantes
#define _XTAL_FREQ 500000  // Frecuencia del oscilador interno
/** Para el ReadMe
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
//******************************************************************************
// Función para inicializar el módulo CCP1 y el timer
//******************************************************************************
void servo_init();

//******************************************************************************
// Función para mover el servomotor a una posición específica (0-180 grados)
//******************************************************************************
void move_servo(uint8_t degrees);


#endif	/* SERVO_H */

