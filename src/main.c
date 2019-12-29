#include "stm8.h"
#include <stdint.h>

#define MPU6050_ADDR    (D0 << 1)   /* if AD0 (pin 9 of MPU6050) is connected to gnd the address is 1101000 */

void i2c_init(void)
{
    I2C_FREQR = 1;
    /* Look at CCRL and CCRH register and eventually configure them here */ 

    I2C_CR1 |= 0x01;    /* Enable peripheral by setting PE to 1 */
}

void i2c_start(void)
{
    I2C_CR2 |= I2C_CR2_START;           /* Generate an I2C START condition */
    while(!(I2C_SR1 & I2C_SR1_SB));     
}

void i2c_stop(void)
{
    I2C_CR2 |= I2C_CR2_STOP;            /* Generate an I2C STOP condition */
    while(I2C_SR3 & I2C_SR3_MSL);       /* I2C_SR3_MSL will be 1 in master mode and 0 in slave mode */
}

void i2c_write(uint8_t data)
{
    I2C_DR = data;
    while(!(I2C_SR1 & I2C_SR1_TXE));
}

int main(void)
{
    uint8_t received_byte;     /* Hold received byte sent by host PC */
    
    CLK_CKDIVR = 0;     /* Clock is 16 MHz */
    
    /* Clockspeed is 16 000 000 and we want a baudrate of 9600. 
     * 16 000 000 / 9600 = 1667. This converted to hex is 0x0683.
     * BRR1 thus should be  0x68.
     * BRR2[3:0] should be 0x3 and BRR2[15:12] should be 0x0 */
    
    UART1_BRR1 = 0x68;
    UART1_BRR2 = 0x03;
    
    UART1_CR2 = UART_CR2_TEN | UART_CR2_REN;    /* Enable transmitter and receiver for UART1 module */
    /* According to the reference manual this is the sequence for the UART configuration */

    while(1)
    {
        /* Did we receive anything from the host PC? */
        if(UART1_SR & UART_SR_RXNE)
        {
            received_byte = UART1_DR;    /* At this point the host PC sended a byte. Let's read it */
            
            if(received_byte == 'A')            
            {
                /* Send message via i2c */
       
                /* Read i2c response */

                /* Send response via UART to host PC */
                
                UART1_DR = 'B';
            }
        }
       
        /* If we just send a response to host PC wait untill host PC received it */ 
        while(!(UART1_SR & UART_SR_TXE));
    }

    
    return 0;
}
