#include "stm8.h"
#include <stdint.h>

int main(void)
{
    CLK_CKDIVR = 0;     /* Clock is 16 MHz */
    
    /* Clockspeed is 16 000 000 and we want a baudrate of 9600. 
     * 16 000 000 / 9600 = 1667. This converted to hex is 0x0683.
     * BRR1 thus should be  0x68.
     * BRR2[3:0] should be 0x3 and BRR2[15:12] should be 0x0 */
    
    UART1_BRR1 = 0x68;
    UART1_BRR2 = 0x03;
    
    UART1_CR2 = UART_CR2_TEN | UART_CR2_REN;    /* Enable transmitter and receiver for UART1 module */
    /* According to the reference manual this is the sequence for the UART configuration */


    
    return 0;
}
