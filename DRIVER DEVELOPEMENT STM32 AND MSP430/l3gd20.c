
#include "l3gd20.h"


  
/** @defgroup L3GD20_Private_Variables
  * @{
  */ 
GYRO_DrvTypeDef L3gd20Drv =
{
  L3GD20_Init,
  L3GD20_DeInit,
  L3GD20_ReadID,
  L3GD20_RebootCmd,
  L3GD20_LowPower,
  L3GD20_INT1InterruptConfig,
  L3GD20_EnableIT,
  L3GD20_DisableIT,
  0,
  0,
  L3GD20_FilterConfig,
  L3GD20_FilterCmd,
  L3GD20_ReadXYZAngRate
};


void L3GD20_Init(uint16_t InitStruct)
{  
  uint8_t ctrl = 0x00;
  
  /* Configure the low level interface */
  GYRO_IO_Init();
  
  /* Write value to MEMS CTRL_REG1 register */
  ctrl = (uint8_t) InitStruct;
  GYRO_IO_Write(&ctrl, L3GD20_CTRL_REG1_ADDR, 1);
  
  /* Write value to MEMS CTRL_REG4 register */  
  ctrl = (uint8_t) (InitStruct >> 8);
  GYRO_IO_Write(&ctrl, L3GD20_CTRL_REG4_ADDR, 1);
}




void L3GD20_DeInit(void)
{
}

uint8_t L3GD20_ReadID(void)
{
  uint8_t tmp;
  
  /* Configure the low level interface */
  GYRO_IO_Init();
  
  /* Read WHO I AM register */
  GYRO_IO_Read(&tmp, L3GD20_WHO_AM_I_ADDR, 1);
  
  /* Return the ID */
  return (uint8_t)tmp;
}


void L3GD20_RebootCmd(void)
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG5 register */
  GYRO_IO_Read(&tmpreg, L3GD20_CTRL_REG5_ADDR, 1);
  
  /* Enable or Disable the reboot memory */
  tmpreg |= L3GD20_BOOT_REBOOTMEMORY;
  
  /* Write value to MEMS CTRL_REG5 register */
  GYRO_IO_Write(&tmpreg, L3GD20_CTRL_REG5_ADDR, 1);
}


void L3GD20_LowPower(uint16_t InitStruct)
{  
  uint8_t ctrl = 0x00;

  /* Write value to MEMS CTRL_REG1 register */
  ctrl = (uint8_t) InitStruct;
  GYRO_IO_Write(&ctrl, L3GD20_CTRL_REG1_ADDR, 1);
}


void L3GD20_INT1InterruptConfig(uint16_t Int1Config)
{
  uint8_t ctrl_cfr = 0x00, ctrl3 = 0x00;
  
  /* Read INT1_CFG register */
  GYRO_IO_Read(&ctrl_cfr, L3GD20_INT1_CFG_ADDR, 1);
  
  /* Read CTRL_REG3 register */
  GYRO_IO_Read(&ctrl3, L3GD20_CTRL_REG3_ADDR, 1);
  
  ctrl_cfr &= 0x80;
  ctrl_cfr |= ((uint8_t) Int1Config >> 8);
  
  ctrl3 &= 0xDF;
  ctrl3 |= ((uint8_t) Int1Config);   
  
  /* Write value to MEMS INT1_CFG register */
  GYRO_IO_Write(&ctrl_cfr, L3GD20_INT1_CFG_ADDR, 1);
  
  /* Write value to MEMS CTRL_REG3 register */
  GYRO_IO_Write(&ctrl3, L3GD20_CTRL_REG3_ADDR, 1);
}


void L3GD20_EnableIT(uint8_t IntSel)
{  
  uint8_t tmpreg;
  
  /* Read CTRL_REG3 register */
  GYRO_IO_Read(&tmpreg, L3GD20_CTRL_REG3_ADDR, 1);
  
  if(IntSel == L3GD20_INT1)
  {
    tmpreg &= 0x7F;	
    tmpreg |= L3GD20_INT1INTERRUPT_ENABLE;
  }
  else if(IntSel == L3GD20_INT2)
  {
    tmpreg &= 0xF7;
    tmpreg |= L3GD20_INT2INTERRUPT_ENABLE;
  }
  
  /* Write value to MEMS CTRL_REG3 register */
  GYRO_IO_Write(&tmpreg, L3GD20_CTRL_REG3_ADDR, 1);
}


void L3GD20_DisableIT(uint8_t IntSel)
{  
  uint8_t tmpreg;
  
  /* Read CTRL_REG3 register */
  GYRO_IO_Read(&tmpreg, L3GD20_CTRL_REG3_ADDR, 1);
  
  if(IntSel == L3GD20_INT1)
  {
    tmpreg &= 0x7F;	
    tmpreg |= L3GD20_INT1INTERRUPT_DISABLE;
  }
  else if(IntSel == L3GD20_INT2)
  {
    tmpreg &= 0xF7;
    tmpreg |= L3GD20_INT2INTERRUPT_DISABLE;
  }
  
  /* Write value to MEMS CTRL_REG3 register */
  GYRO_IO_Write(&tmpreg, L3GD20_CTRL_REG3_ADDR, 1);
}


void L3GD20_FilterConfig(uint8_t FilterStruct) 
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG2 register */
  GYRO_IO_Read(&tmpreg, L3GD20_CTRL_REG2_ADDR, 1);
  
  tmpreg &= 0xC0;
  
  /* Configure MEMS: mode and cutoff frequency */
  tmpreg |= FilterStruct;
  
  /* Write value to MEMS CTRL_REG2 register */
  GYRO_IO_Write(&tmpreg, L3GD20_CTRL_REG2_ADDR, 1);
}


void L3GD20_FilterCmd(uint8_t HighPassFilterState)
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG5 register */
  GYRO_IO_Read(&tmpreg, L3GD20_CTRL_REG5_ADDR, 1);
  
  tmpreg &= 0xEF;
  
  tmpreg |= HighPassFilterState;
  
  /* Write value to MEMS CTRL_REG5 register */
  GYRO_IO_Write(&tmpreg, L3GD20_CTRL_REG5_ADDR, 1);
}

/**
  * @brief  Get status for L3GD20 data
  * @param  None         
  * @retval Data status in a L3GD20 Data
  */
uint8_t L3GD20_GetDataStatus(void)
{
  uint8_t tmpreg;
  
  /* Read STATUS_REG register */
  GYRO_IO_Read(&tmpreg, L3GD20_STATUS_REG_ADDR, 1);
  
  return tmpreg;
}

/**
* @brief  Calculate the L3GD20 angular data.
* @param  pfData: Data out pointer
* @retval None
*/
void L3GD20_ReadXYZAngRate(float *pfData)
{
  uint8_t tmpbuffer[6] ={0};
  int16_t RawData[3] = {0};
  uint8_t tmpreg = 0;
  float sensitivity = 0;
  int i =0;
  
  GYRO_IO_Read(&tmpreg,L3GD20_CTRL_REG4_ADDR,1);
  
  GYRO_IO_Read(tmpbuffer,L3GD20_OUT_X_L_ADDR,6);
  
  /* check in the control register 4 the data alignment (Big Endian or Little Endian)*/
  if(!(tmpreg & L3GD20_BLE_MSB))
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i+1] << 8) + tmpbuffer[2*i]);
    }
  }
  else
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i] << 8) + tmpbuffer[2*i+1]);
    }
  }
  
  /* Switch the sensitivity value set in the CRTL4 */
  switch(tmpreg & L3GD20_FULLSCALE_SELECTION)
  {
  case L3GD20_FULLSCALE_250:
    sensitivity=L3GD20_SENSITIVITY_250DPS;
    break;
    
  case L3GD20_FULLSCALE_500:
    sensitivity=L3GD20_SENSITIVITY_500DPS;
    break;
    
  case L3GD20_FULLSCALE_2000:
    sensitivity=L3GD20_SENSITIVITY_2000DPS;
    break;
  }
  /* Divide by sensitivity */
  for(i=0; i<3; i++)
  {
    pfData[i]=(float)(RawData[i] * sensitivity);
  }
}

