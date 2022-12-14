#ifndef CFG_H
#define CFG_H

#include <tuple>
#include <cstdint>
#include <cassert>

#include <stm32h7xx_hal.h>
#include <stm32h743i_eval_sd.h>
#include <FreeRTOS.h>
#include <semphr.h>

#include "defs.h"
#include "audio_play.h"

#define VSYNC_FREQ_Pin GPIO_PIN_10
#define VSYNC_FREQ_GPIO_Port GPIOF
#define LCD_RESET_Pin GPIO_PIN_2
#define LCD_RESET_GPIO_Port GPIOA
#define LCD_BL_CTRL_Pin GPIO_PIN_6
#define LCD_BL_CTRL_GPIO_Port GPIOA
#define RENDER_TIME_Pin GPIO_PIN_4
#define RENDER_TIME_GPIO_Port GPIOA


class cfg_t;
// ????? ??? ?????? ??????? ????????????? cfg_t ? ?????? ????????????? cfg_t.
// ?????, ?????? ??? ????????? ?????? ???????????????? ? ?????? ????????????? cfg_t ? ? ??? ????? ??? ??????
// ???? ??????? ????????? ???????, ?????, ??? cfg_t::gpio_init
class cfg_initializer_t
{
private:
  cfg_initializer_t(cfg_t* a_cfg);
  friend class cfg_t;
};

class cfg_t
{
private:
  std::tuple<uint32_t, GPIO_TypeDef*> m_lcd_vsync_pin_port;
  std::tuple<uint32_t, GPIO_TypeDef*> m_lcd_reset_pin_port;
  std::tuple<uint32_t, GPIO_TypeDef*> m_lcd_bl_ctrl_pin_port;
  std::tuple<uint32_t, GPIO_TypeDef*> m_lcd_render_time_pin_port;

  LTDC_HandleTypeDef m_ltd_handle;
  DMA2D_HandleTypeDef m_dma2d_handle;
  QSPI_HandleTypeDef m_qspi_handle;
  UART_HandleTypeDef m_uart_handle;

  bool sd_detected;

  SemaphoreHandle_t m_uart_mutex;

  cfg_t();
  cfg_initializer_t cfg_initializer;
  friend class cfg_initializer_t;

  void gpio_init();
  void crc_init();
  void uart_init();
  void ltdc_init();
  void fmc_init();
  void dma2d_init();
  void qspi_init();
  void sd_init();
public:
  static cfg_t &instance() {
    static cfg_t cfg_instance;
    return cfg_instance;
  }

  audio_play_t audio_player;

  LTDC_HandleTypeDef* ltdc_handle();
  DMA2D_HandleTypeDef* dma2d_handle();
  bool is_sd_detected() { return sd_detected; };
  void uart_send_char(uint8_t a_symbol);
};

#endif // CFG_H
