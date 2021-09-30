/*
  Archivo:  LAB07.c
  Autor:    Alejandro Ramirez Morales
  Creado:   27/sep/21
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>

/*
 +----------------------------------------------------------------------------+
 |                                VARIABLES                                   |
 +----------------------------------------------------------------------------+
 */
uint8_t unidades;
uint8_t decenas;
uint8_t centenas;

uint8_t display0;
uint8_t display1;
uint8_t display2;

uint8_t cuenta;
uint8_t turno;
/*
 +----------------------------------------------------------------------------+
 |                          PROTOTIPOS DE FUNCIONES                           |
 +----------------------------------------------------------------------------+
 */
void setup(void);
uint8_t ArregloNumero (uint8_t numero);
void Displayturno (void);
/*
 +----------------------------------------------------------------------------+
 |                               INTERRUPCIONES                               |
 +----------------------------------------------------------------------------+
 */
void __interrupt() isr (void)
{
     if(INTCONbits.T0IF)         // Si la bandera está encendida, entrar
    {            
         switch(turno){
             PORTD = 0;
            case 0:
                PORTC = display0;
                PORTD = 0b001;
                break;
            case 1:
                PORTC = display1;
                PORTD = 0b010;
                break;
            case 2: 
                PORTC = display2;
                PORTD = 0b100;
                break; 
             default:
                break;  
               
         }
         
        
        PORTE++;                // Incrementar PuertoC
        INTCONbits.T0IF = 0;    // Limpiar bandera
        TMR0 = 5;             // Timer0 a 0.099 segundos
    }  
}

/*
 +----------------------------------------------------------------------------+
 |                                   LOOP                                     |
 +----------------------------------------------------------------------------+
 */
void main(void) 
{
    setup(); // Se ejecuta funcion setup
    
    while(1)
    {
        unidades = cuenta%10;
        int cociente0 = (int)(cuenta/10);
    
        decenas = cociente0%10;
        centenas = (int)(cociente0/10);
        
        display0 = ArregloNumero(unidades);
        display1 = ArregloNumero(decenas);
        display2 = ArregloNumero(centenas);
        
        PORTA = cuenta;
        Displayturno();
        
        if(!PORTBbits.RB0)      // Si RB0 no es 1:
        {
            while(!RB0);        // Mientras RB0 no sea 1, aumentar
            cuenta ++;
        }
        
        if(!PORTBbits.RB1)      // Si RB1 no es 1:
        {
            while(!RB1);        // Mientras RB1 no sea 1, decrementar
            cuenta --;
        }
    }
}

/*
 +----------------------------------------------------------------------------+
 |                                  SETUP                                     |
 +----------------------------------------------------------------------------+
 */
void setup(void)
{
    // Ports 
    ANSEL   =   0;              // Digital Ports
    ANSELH  =   0;
    
    TRISA   =   0;              // PORTA - salida
    TRISB   =   0b11;           // RB0 y RB1 - outs
    TRISC   =   0;
    TRISD   =   0;              // PORTC - salida
    TRISE   =   0;
    
    PORTA   =   0;              // PORTA en 0
    PORTB   =   0;              // PORTB en 0
    PORTC   =   0;              // PORTB en 0
    PORTD   =   0;              // PORTC en 0
    PORTE   =   0;              // PORTB en 0
    
    
    // Reloj
    OSCCONbits.IRCF = 0b010;    // 24MHz
    OSCCONbits.SCS = 1;         // Activar reloj interno
    
    // Interrupciones
    INTCONbits.GIE  = 1;        // Interrupciones globales activadas
    INTCONbits.PEIE = 1;        // Interrupciones perifericas activadas
    INTCONbits.RBIE = 1;        // Interrupciones de PORTB activadas
    INTCONbits.RBIF = 0;        // Limpiar bandera de PORTB
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    
    // IOCB
    OPTION_REGbits.nRBPU = 0;   // Pull-ups activadas
    WPUB = 0b11;                // Pull-ups asignadas a RB0 y RB1
    
    // TMR0
    OPTION_REGbits.PS0  = 0;
    OPTION_REGbits.PS1  = 0;
    OPTION_REGbits.PS2  = 0;    // 1:2 Prescaler
    OPTION_REGbits.PSA  = 0;
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.T0SE = 0;
    TMR0 = 5;
    return;   
}

uint8_t ArregloNumero (uint8_t numero)
{
    switch(numero){
        case 0:
            return 0b00111111;
            break;
            
        case 1:
            return 0b00000110;
            break;
            
        case 2:
            return 0b01011011;
            break;
            
        case 3:
            return 0b01001111;
            break;
            
        case 4:
            return 0b01100110;
            break;
            
        case 5:
            return 0b01101101;
            break;
            
        case 6:
            return 0b01111101;
            break;
            
        case 7:
            return 0b00000111;
            break;
            
        case 8:
            return 0b01111111;
            break;
            
        case 9:
            return 0b01101111;
            break;
            
        default:
            return 0b00111111;
            break;   
    }   
}
void Displayturno (void)
{
    switch(turno){
        case 0:
            turno = 1 ;
            break;
            
        case 1:
            turno = 2 ;
            break;
            
        case 2:
            turno = 0 ;
            break;
             
            
        default:
            turno = 0 ;
            break;   
    }  
    return;
}
