
#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define TXLED BIT0
#define Button BIT3

long i = 0;

char standard[] = "Standard Message\r\n";

float A = 3.33;
float V = 1.12;

char current = 'A';
char voltage = 'V';

void sendData(char* tx_data);
void packet(float val, char type);

int index = 0;

int main(void)
{
     WDTCTL = WDTPW + WDTHOLD; // Stop WDT

    /* Use Calibration values for 1MHz Clock DCO*/
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    /* Configure Pin Muxing P1.1 RXD and P1.2 TXD */
    P1SEL = BIT1 | BIT2 ;
    P1SEL2 = BIT1 | BIT2;

    P1DIR &= ~Button;
    P1OUT |= Button;
    P1REN |= Button;

    P1DIR  |=  BIT0 + BIT4 + BIT5 + BIT6;  // P1.0 and P1.6 output
    P1OUT  &= ~(BIT0 + BIT4 + BIT5 + BIT6);  // P1.0 and P1.6 = 0

    P2DIR |= BIT0 + BIT1 + BIT2  + BIT4 + BIT5;
    P2OUT &= ~(BIT0 + BIT1 + BIT2 + BIT4 + BIT5);

    /* Place UCA0 in Reset to be configured */
    UCA0CTL1 = UCSWRST;

    /* Configure */
    UCA0CTL1 |= UCSSEL_2; // SMCLK
    UCA0BR0 = 104; // 1MHz 9600
    UCA0BR1 = 0; // 1MHz 9600
    UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1

    /* Take UCA0 out of reset */
    UCA0CTL1 &= ~UCSWRST;

    /* Enable USCI_A0 RX interrupt */
    IE2 |= UCA0RXIE;

    __bis_SR_register(GIE); // Enter LPM0, interrupts enabled

    while(1){

        packet(A, current);
        packet(V, voltage);


        A += 0.06;
        if (A > 6){
            A = 0.51;
        }

        V += 0.44;
        if (V > 20){
            V = 0;
        }
    }
}

void packet(float val, char type){

    char buffer[sizeof(val) + 5];
    sprintf(buffer, "%.2f", val);

    if (type == 'A'){
        strcat(buffer, ",A,1E");
    } else if (type == 'V'){
        strcat(buffer, ",V,1E");
    }

    sendData(buffer);
    _delay_cycles(10000);
}

void sendData(char* tx_data){
    unsigned int i = 0;
    P1OUT ^= BIT0;
    while(tx_data[i]){
        while((UCA0STAT & UCBUSY));
        UCA0TXBUF = tx_data[i];
        i++;
    }

}

/* Echo back RXed character, confirm TX buffer is ready first */
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{

    P1OUT ^= BIT6;

    switch (UCA0RXBUF){

    case '1':
        // Solar Farm active
        P1OUT ^= BIT6;
    break;

    case '2':
        // Battery Active
        P1OUT ^= BIT6;
    break;

    case '3':
        // 5V DC Load Active
        P1OUT ^= BIT6;
    break;

    case '4':
        // 5V AC Load Active
        P1OUT ^= BIT6;
    break;

    case '5':
        // 24V DC Load Active
        P1OUT ^= BIT6;
    break;

    case '6':
        // 24V AC Load Active
        P1OUT ^= BIT6;
    break;

    case '11':
        // Enable Solar Farm to Battery Relay
        P1OUT ^= BIT6;
    break;

    case '10':
        // Disable Solar Farm to Battery Relay
        P1OUT ^= BIT6;
    break;

    case '21':
        // Enable Battery to loads Relay
        P1OUT ^= BIT6;
    break;

    case '20':
        // Disable Battery to loads Relay
        P1OUT ^= BIT6;
    break;

    case '31':
        // Enable 5V DC Load Relay
        P1OUT ^= BIT6;
    break;

    case '30':
        // Disable 5V DC Load Relay
        P1OUT ^= BIT6;
    break;

    case '41':
        // Enable 5V AC Load Relay
        P1OUT ^= BIT6;
    break;

    case '40':
        // Disable 5V AC Load Relay
        P1OUT ^= BIT6;
    break;

    case '51':
        // Enable 24V DC Load Relay
        P1OUT ^= BIT6;
    break;

    case '50':
        // Disable 24V DC Load Relay
        P1OUT ^= BIT6;
    break;

    case '61':
        // Enable 24V AC Load Relay
        P1OUT ^= BIT6;
    break;

    case '60':
        // Disable 24V AC Load Relay
        P1OUT ^= BIT6;
    break;

    default:
        while (!(IFG2&UCA0TXIFG)); // USCI_A0 TX buffer ready?
        UCA0TXBUF = '0';
        UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
    break;

    }


}
