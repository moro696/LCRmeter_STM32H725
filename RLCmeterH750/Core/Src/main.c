
#include "main.h"
#include "usb_device.h"
#include "delay.h"
#include "m25p16.h"

volatile int64_t g_sum;
extern volatile SPI_HandleTypeDef SpiHandle;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{


 SCB_EnableICache();
  SCB_EnableDCache();
  HAL_Init();
  SystemClock_Config();
  DelayInit();
  //HAL_PWREx_EnableUSBReg();
  HAL_Delay(100);
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  QuadEncInit();
  SinCosInit();
  ResistorSelectorInit();
  UTFT_InitLCD(UTFT_LANDSCAPE2);
  UTFT_fillScrW(VGA_BLACK);
  UTFT_setColorW(VGA_GREEN);
  UTFT_fillRect(10,10, 20,20);

  UTFT_setFont(BigFont);
  UTFT_setColorW(VGA_WHITE);
  AD9833_Init();
  UTFT_print("AD9833_Init", 20, 30);

  m25p16_init();
  HAL_Delay(10);



  if(!m25p16_read_ram_id_and_check())
  {
         UTFT_print("Flash check FAIL", 20, 30);
         Error_Handler();
  }

  UTFT_print("Flash OK", 20, 30);
  DualAdcInitAndStart();

  UTFT_print("ADC Started ", 20, 30);

  LoadSettings();
  CorrectionLoad(g_settings.correction_index);

  InterfaceStart();
  TaskSetFreq(StandartFreq(g_settings.single_freq_index)); // get frequency for g_freq !!!
  //TaskSetFreq(320);
  TaskStartConvolution();
  SceneSingleFreqStart();

 while (1)
  {

      HAL_Delay(1);
      TaskQuant();

      if(IsUsbCommand())
      {
          continue;
      }

      InterfaceQuant();
      if(SaveSettingsIfChangedAndTimeUp())
      {

       //   UTFT_setFont(FONT8x15);
       //   UTFT_print("Settings Saved", 20, 30);
       //   HAL_Delay(500);
      //    UTFT_print("              ", 20, 30);

      }
  }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;// RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 10;//5;
  RCC_OscInitStruct.PLL.PLLN = 200;//160;
  RCC_OscInitStruct.PLL.PLLP = 1;//2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }


  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_SPI3;
  PeriphClkInitStruct.PLL2.PLL2M = 25;
  PeriphClkInitStruct.PLL2.PLL2N = 176;//  176-for 2.75Msps ; 256 - for 2Msps
  PeriphClkInitStruct.PLL2.PLL2P = 2;  // 2; - for 2.75Msps, 4 for 2Msps
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;


  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = 192;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 4;
  PeriphClkInitStruct.PLL3.PLL3R = 2;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL2;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
          Error_Handler();
  }



}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{


    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();



  GPIO_InitTypeDef  gpio1 = {};

    gpio1.Mode      = GPIO_MODE_OUTPUT_PP;
    gpio1.Alternate = 0;
    gpio1.Pull      = GPIO_NOPULL;
    gpio1.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio1.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOC, &gpio1);

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,1);
    //HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_15);




}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


