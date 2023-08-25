/* 
 * File:   ultrasonic.h
 * Author: Christian Alessandro Campos López 21760
 *
 * Created on 19 de agosto de 2023, 11:45 AM
 */
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ULTRASONIC_H
#define	ULTRASONIC_H

#ifndef TRIG
#define TRIG RE0
#endif 

#ifndef ECHO
#define ECHO RE1
#endif 
#ifndef TRIGPin
#define TRIGPin TRISE0
#endif 

#ifndef ECHOPin
#define ECHOPin TRISE1
#endif 

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
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
// Inicio del sensor ultrasónico
//******************************************************************************
void ultrasonic_init(void);
//******************************************************************************
// Cálculo de la distancia leída por el sensor
//******************************************************************************
float ultrasonic_measure_distance(void);

#endif // ULTRASONIC_H


