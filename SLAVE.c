/*
 * File:   SLAVE.c
 * Author: Daniel Ramos
 *
 * Created on 25 de octubre de 2020, 03:51 PM
 */


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "configuration.h"

/*  Functions Used    */
void inicializa();          //Ports initialization
void configura();           //Ports configuration
void SPIwrite(char dato);   //Sending data

/*  Global variable  */
int datout;

/*  Main Function   */
void main(void) {
    char recibidos;
    
    configura();
    inicializa();
    
    while(1){
        recibidos = SSPBUF; //Saving SSPBUF
        PORTD = recibidos;  //Master-data
        SPIwrite(recibidos + 0x01); //Adding 1 bit and sending it back to master
    }
}

/*  SPIwrite Function   */
void SPIwrite(char dato){
    SSPBUF = dato;
    while(!SSPSTATbits.BF); //Wait until the flag changes
    SSPCON1bits.SSPOV = 0;  //Cleaning flag-overflow
}

/*  inicializa Function   */
void inicializa(){
    PORTD = 0b00000000; //Initializating port D in low
}

/*  configura Function   */
void configura(){
    ADCON1 = 0X0F;  //Configuring inputs/outups as digital
    
    TRISBbits.RB0 = 1;  //SDI as input-data
    TRISBbits.RB1 = 1;  //SCK high
    TRISCbits.RC7 = 0;  //SDO as output-data
    TRISAbits.RA5 = 1;  //SS as slave-input
    
    TRISD = 0; //Port D as output
    
    //Configuración SSPSTAT
    SSPSTATbits.SMP = 0;    //Sample Bit as half output-bit
    SSPSTATbits.CKE = 0;    //Clock Select Bit active transition
    
    //SSPCON1 configuration
    SSPCON1 = 0b00000101;   //SPI slave mode, clock = pin SCK, SS enable
    SSPCON1bits.CKP = 1;    //idle-clock state high
    SSPCON1bits.SSPEN = 1;  //SPI serial port enable
    SSPCON1bits.SSPOV = 0;  //Cleaning flag overflow
}

