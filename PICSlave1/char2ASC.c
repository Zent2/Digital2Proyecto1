/* 
 * File:   char2ASC.c
 * Author: Christian Alessandro Campos López 21760
 *
 * Created on 5 de agosto de 2023, 11:14 PM
 */
#include "char2ASC.h"
//******************************************************************************
// Variables Internas
//******************************************************************************
char variable;
//******************************************************************************
// Conversión a ASCCI
//******************************************************************************

void convertir_a_ascii(uint16_t numero, char* cadena) {
    cadena[0] = (numero / 100) + '0'; // Convertir centenas a carácter ASCII
    cadena[1] = ((numero / 10) % 10) + '0'; // Convertir decenas a carácter ASCII
    cadena[2] = (numero % 10) + '0'; // Convertir unidades a carácter ASCII
    cadena[3] = '\0'; // Terminar la cadena con el caracter nulo para indicar el fin de la cadena
}