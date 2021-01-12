/*
 * File:   MASTER.c
 * Author: Daniel Ramos
 *
 * Created on 25 de octubre de 2020, 03:13 PM
 */


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "configuration.h"

/*  Functions Used    */
void inicializa();          //Ports initialization
void configura();           //Ports configuration
void SPIwrite(char dato);   //Sending data
void delay200();            //2 seconds delay

/*  Main Function   */
void main() {
    char recibidos;
    
    configura();
    inicializa();
    TRISA = 1;      //Dip sw like an input
    PORTA = 0;      //Cleaning the port
    
    while(1){
        __delay_ms(20);
        SPIwrite(PORTA);    //Sending the data form port A
        PORTBbits.RB2 = 1;  //Slave sending back the data
        recibidos = SSPBUF; //Receive what the slave sent
        PORTD = recibidos;  //Shows the slave message in port D
        delay200();
    }
}

/*  SPIwrite Function    */
void SPIwrite(char dato){
    PORTBbits.RB2 = 0;  //Slave receiving data
    SSPBUF = dato;
    while(!SSPSTATbits.BF); //Wait until the flag changes
}

/*  inicializa Function  */
void inicializa(){
    PORTD = 0b00000000; //Initialized port D in 0's
}

/*  configura Function   */
void configura(){
    ADCON1 = 0X0F;  //Configuring inputs/outups as digital
    
    TRISBbits.RB0 = 1;  //SDI as input-data
    TRISBbits.RB1 = 0;  //SCK to 1
    TRISCbits.RC7 = 0;  //SDO as output data
    TRISBbits.RB2 = 0;  //SS as master-output
    
    TRISD = 0b00000000; //Port D as output
    
    //SSPSTAT configuration
    SSPSTATbits.SMP = 0;    //Sample Bit as half output-bit
    SSPSTATbits.CKE = 0;    //Clock Select Bit active transition
    
    //SSPCON1 configuration
    SSPCON1 = 0b00000001;   //SPI master mode, clock = Fosc/16
    SSPCON1bits.CKP = 1;    //idle-state in high
    SSPCON1bits.SSPEN = 1;  //SPI serial port enable
}

/*  delay200 Function    */
void delay200(){
    int g;
    for(g = 0; g < 20; g++){
        __delay_ms(10);
    }
}




