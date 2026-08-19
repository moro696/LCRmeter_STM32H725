

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stdbool.h"
#include "quadrature_encoder.h"
#include "measure_freq.h"
#include "UTFT.h"
#include "DefaultFonts.h"
#include "select_resistor.h"
#include "sin_cos.h"
#include "m25p16.h"
#include "AD9833_driver.h"
#include "dual_adc.h"
#include "settings.h"
#include "interface.h"
#include "colors.h"
#include "font_condensed30.h"
#include "font_condensed59.h"
#include "data_processing.h"
#include "task.h"
#include "scene_single_freq.h"

void Error_Handler(void);


#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA

#define ST25V016





#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
