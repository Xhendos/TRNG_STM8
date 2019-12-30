#include "stm8.h"

#include <stdint.h>
#include <string.h>

#define MPU6050_ADDR    (0x68 << 1)     /* if AD0 (pin 9 of MPU6050) is connected to gnd the address is 1101000 */

#define I2C_READ    1
#define I2C_WRITE   0

/* MPU6050 register addresses */
#define PWR_MGMT_1      (0x6B)
#define WHO_AM_I        (0x75)

/* i2c functions */
void i2c_init(void)
{
    I2C_FREQR = 16;
    /* Look at CCRL and CCRH register and eventually configure them here */ 

    I2C_CCRH = 0x00;
    I2C_CCRL = 0x50;
    I2C_TRISER = 0x11;

    I2C_CR2 |= I2C_CR2_ACK;


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

void i2c_write_addr(uint8_t addr)
{
    I2C_DR = addr;
    while(!(I2C_SR1 & I2C_SR1_ADDR));
    (void) I2C_SR3;     /* Clear event EV_6 */
    
    I2C_CR2 |= I2C_CR2_ACK; 
}

uint8_t i2c_read_byte(void)
{
    uint8_t ret;

    I2C_CR2 &= ~(I2C_CR2_ACK);      /* Send a Nack on next byte receive */
    (void) I2C_SR3;                 /* Clear event E6_3 for master receiver when N = 1 */
    i2c_stop();                     /* Must be happend just after E6_3 */

    while(!(I2C_SR1 & I2C_SR1_RXNE));   /* Wait untill the byte has been received */
    
    ret = I2C_DR;
    return ret;
}

/* Main program */
int main(void)
{
    uint8_t received_byte;      /* Hold received byte sent by host PC */
    uint8_t who_am_i;           /* who_am_i read from the MPU6050 */

    CLK_CKDIVR = 0;     /* Clock is 16 MHz */
    CLK_PCKENR1 |= PCKEN10;

    i2c_init();         /* Initialise the i2c module */
    
    /* Clockspeed is 16 000 000 and we want a baudrate of 9600. 
     * 16 000 000 / 9600 = 1667. This converted to hex is 0x0683.
     * BRR1 thus should be  0x68.
     * BRR2[3:0] should be 0x3 and BRR2[15:12] should be 0x0 */
    
    UART1_BRR1 = 0x68;
    UART1_BRR2 = 0x03;
    
    UART1_CR2 = UART_CR2_TEN | UART_CR2_REN;    /* Enable transmitter and receiver for UART1 module */
    /* According to the reference manual this is the sequence for the UART configuration */

    /* Wake up the MPU6050*/ 
    i2c_start();
    i2c_write_addr((0x68 << 1));
    i2c_write(PWR_MGMT_1);
    i2c_write(0);
    i2c_stop(); 
     
    /* Request to read from WHO_AM_I register. The value in this register should be 0110 1000 */
    i2c_start();
    i2c_write_addr(MPU6050_ADDR + I2C_WRITE);
    i2c_write(WHO_AM_I);
    i2c_stop();

    /* Read the actual value from the WHO_AM_I register */
    i2c_start();
    i2c_write_addr(MPU6050_ADDR + I2C_READ);
    who_am_i = i2c_read_byte();

    if(who_am_i != 104) /* If no MPU6050 sensor found quit the program execution */
        return 1;

    /* Wait in an infinite loop for a request */
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
