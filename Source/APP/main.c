#include "modbus.h"

uint8_t  cnt = 0;   //��λ��������
uint8_t temp;
int main(void)
{
    //SysTick_Config(SystemCoreClock / 1000);
     Delay_us(1000);

    HAL_Init();
   
    cnt = Eeprom_Read_OneByte(RESET_CNT);
    if(cnt >=4) cnt = 0;
    Read_Device_Param();                                                        //��ȡ�豸����
    gUart1_Config();                                                            //����1��ʼ��
    PCap_Init();                                                                //PCapоƬ��ʼ��
    gTim2Config();                                                              //��ʱ��2����
    ADC_Chn_Config();                                                           //ADC��ͨ������
    DAC_Chn_Selection();                                                        //DACͨ��ѡ������
    User_Iwdg_Init();                                                         //��ʼ�����Ź�
     

      //EepInitFlg = Eeprom_Read_OneByte(RUN_ADDR_BASE);
    while(1)
    {
     User_Iwdg_Feed();                                                       //ι���Ź�                                                         
      
      //Modbusͨ�Ŵ���
        if(UsartRecvOk == USART_RECV_TIMER_OK)
        {
            MBASCII_Function();
            UsartRecvOk = 0;                                                    //��������ձ�־��0            
        }
        else
        {
            SensorEvent();                                                      //�������¼��Զ��ϱ�
        }
        GetPCapResult();                                                        //��ȡPCap�ɼ�����
        DataFilterAndOutput();                                                  //PCap�����˲������
        ADC_GetChn_Value();                                                     //ADCͨ�����ݻ�ȡ
        DAC_Chn_Handle();                                                       //DACͨ�����ݴ���
    }
}

//**************************************************************************************************
// ����         	: SystemClockConfiguration()
// ��������     	: 2017-04-26
// ����        	 	: ׯ��Ⱥ
// ����         	: ϵͳʱ������
// �������     	: ��
// �������     	: ��
// ���ؽ��     	: ��
// ע���˵��   	:
// �޸�����     	:
//**************************************************************************************************

void SystemClockConfiguration(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_CFGR_SW_HSI;//RCC_SYSCLKSOURCE_PLLCLK;
  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
  //if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  //{
  //  Error_Handler();
  //}
}