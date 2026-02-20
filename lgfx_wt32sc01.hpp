#pragma once
#include <LovyanGFX.hpp>

// WT32-SC01 Display Configuration
// 3.5" TFT LCD with ST7796 driver and FT6336 capacitive touch
// Based on working TFT_eSPI User_Setup.h configuration

class LGFX : public lgfx::LGFX_Device {
public:
  LGFX() {

    // -----------------------------
    //  SPI BUS for TFT (HSPI)
    // -----------------------------
    {
      auto cfg = _bus.config();

      cfg.spi_host = HSPI_HOST;  // HSPI (same as USE_HSPI_PORT)
      cfg.spi_mode = 0;
      cfg.freq_write = 27000000;  // 27MHz as in working config
      cfg.freq_read  = 20000000;  // SPI_READ_FREQUENCY

      // WT32-SC01 TFT SPI pins (from working config)
      cfg.pin_sclk = 14;   // TFT_SCLK
      cfg.pin_mosi = 13;   // TFT_MOSI
      cfg.pin_miso = 12;   // TFT_MISO

      cfg.pin_dc   = 21;   // TFT_DC

      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    // -----------------------------
    //  TFT PANEL (ST7796)
    // -----------------------------
    {
      auto cfg = _panel.config();

      cfg.pin_cs   = 15;   // TFT_CS
      cfg.pin_rst  = 22;   // TFT_RST
      cfg.pin_busy = -1;

      // WT32-SC01 has 480x320 display
      cfg.panel_width  = 320;
      cfg.panel_height = 480;

      cfg.offset_x = 0;
      cfg.offset_y = 0;

      cfg.offset_rotation = 0;

      // Set to true if colors are inverted
      cfg.rgb_order = false;

      _panel.config(cfg);
    }

    // -----------------------------
    //  BACKLIGHT
    // -----------------------------
    {
      auto cfg = _light.config();
      cfg.pin_bl = 23;     // TFT_BL
      cfg.invert = false;
      cfg.freq   = 4000;
      cfg.pwm_channel = 7;
      _light.config(cfg);
      _panel.setLight(&_light);
    }

    // -----------------------------
    //  TOUCH (FT6336 I2C Capacitive)
    // -----------------------------
    {
      auto cfg = _touch.config();

      // I2C configuration for FT6336
      cfg.i2c_addr = 0x38;  // FT6336 I2C address
      cfg.i2c_port = 1;     // I2C port 1
      cfg.pin_sda  = 18;    // Touch SDA (PIN_SDA from working config)
      cfg.pin_scl  = 19;    // Touch SCL (PIN_SCL from working config)
      cfg.pin_int  = 38;    // Touch INT
      cfg.freq     = 400000;

      // Touch calibration for WT32-SC01
      cfg.x_min = 0;
      cfg.x_max = 320;
      cfg.y_min = 0;
      cfg.y_max = 480;

      _touch.config(cfg);
      _panel.setTouch(&_touch);
    }

    setPanel(&_panel);
  }

private:
  lgfx::Bus_SPI        _bus;
  lgfx::Panel_ST7796   _panel;  // ST7796 for WT32-SC01 3.5" display
  lgfx::Light_PWM      _light;
  lgfx::Touch_FT5x06   _touch;  // FT6336 is part of FT5x06 family
};
