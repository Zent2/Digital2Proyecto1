/* 
 * File:   USART.h
 * Author: Christian Alessandro Campos López 21760
 *
 * Created on 23 de agosto de 2023, 10:43 AM
 */
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef USART_H
#define	USART_H

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
// Inicio de comunicación Serial
//******************************************************************************
void UART_Init();
//******************************************************************************
// Escribir en la comunicación serial
//******************************************************************************
void UART_Write(uint8_t data);
//******************************************************************************
// Leer de la comunicación Serial
//******************************************************************************
char UART_Read();
//******************************************************************************
// Función para enviar cadena
//******************************************************************************
void enviar_cadena(char* cadena);
#endif	/* USART_H */

