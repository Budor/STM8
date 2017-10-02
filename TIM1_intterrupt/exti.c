/*                        放置stm8s中断函数
---------------------Interrupt vector numbers-----------------------------------
#define AWU_vector                           0x03
#define SPI_TXE_vector                       0x0C
#define SPI_RXNE_vector                      0x0C
#define SPI_WKUP_vector                      0x0C
#define SPI_CRCERR_vector                    0x0C
#define SPI_OVR_vector                       0x0C
#define SPI_MODF_vector                      0x0C
#define TIM1_OVR_UIF_vector                  0x0D
#define TIM1_CAPCOM_BIF_vector               0x0D
#define TIM1_CAPCOM_TIF_vector               0x0D
#define TIM1_CAPCOM_CC1IF_vector             0x0E
#define TIM1_CAPCOM_CC2IF_vector             0x0E
#define TIM1_CAPCOM_CC3IF_vector             0x0E
#define TIM1_CAPCOM_CC4IF_vector             0x0E
#define TIM1_CAPCOM_COMIF_vector             0x0E
#define TIM2_OVR_UIF_vector                  0x0F
#define TIM2_CAPCOM_CC1IF_vector             0x10
#define TIM2_CAPCOM_TIF_vector               0x10
#define TIM2_CAPCOM_CC2IF_vector             0x10
#define TIM2_CAPCOM_CC3IF_vector             0x10
#define UART1_T_TXE_vector                   0x13
#define UART1_T_TC_vector                    0x13
#define UART1_R_OR_vector                    0x14
#define UART1_R_RXNE_vector                  0x14
#define UART1_R_IDLE_vector                  0x14
#define UART1_R_PE_vector                    0x14
#define UART1_R_LBDF_vector                  0x14
#define I2C_ADD10_vector                     0x15
#define I2C_ADDR_vector                      0x15
#define I2C_OVR_vector                       0x15
#define I2C_STOPF_vector                     0x15
#define I2C_BTF_vector                       0x15
#define I2C_WUFH_vector                      0x15
#define I2C_RXNE_vector                      0x15
#define I2C_TXE_vector                       0x15
#define I2C_BERR_vector                      0x15
#define I2C_ARLO_vector                      0x15
#define I2C_AF_vector                        0x15
#define I2C_SB_vector                        0x15
#define ADC1_AWS0_vector                     0x18
#define ADC1_AWS1_vector                     0x18
#define ADC1_AWS2_vector                     0x18
#define ADC1_AWS3_vector                     0x18
#define ADC1_AWS4_vector                     0x18
#define ADC1_AWS5_vector                     0x18
#define ADC1_AWS6_vector                     0x18
#define ADC1_EOC_vector                      0x18
#define ADC1_AWS8_vector                     0x18
#define ADC1_AWS9_vector                     0x18
#define ADC1_AWDG_vector                     0x18
#define ADC1_AWS7_vector                     0x18
#define TIM4_OVR_UIF_vector                  0x19
#define FLASH_EOP_vector                     0x1A
#define FLASH_WR_PG_DIS_vector               0x1A
*/

#include "main.h"
#include "led.h"

#pragma vector=TIM1_OVR_UIF_vector
__interrupt void TIM1_Interrupt(void)
{
  static uint8 i = 0;
  switch(i)
  {
    case 0:LED_ON;i++;break;
    case 1:LED_OFF;i=0;break;
  }
  TIM1_SR1 &= ~(1<<0);//清除更新中断标志
}

