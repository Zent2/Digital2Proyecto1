/* 
 * File:   char2ASC.h
 * Author: Christian Alessandro Campos López 21760
 *
 * Created on 5 de agosto de 2023, 11:13 PM
 */
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CHAR2ASC_H
#define	CHAR2ASC_H

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
// Convertir a ascii
//******************************************************************************

void convertir_a_ascii(uint16_t numero, char* cadena);
#endif	/* CHAR2ASC_H */

