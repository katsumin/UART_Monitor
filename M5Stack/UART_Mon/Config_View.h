#ifndef _CONFIG_VIEW_H_
#define _CONFIG_VIEW_H_
#include <M5Stack.h>
#include "Selecter.h"
#include "FunctionButton.h"

#define TEXT_WIDTH (8)
#define TEXT_HEIGHT (15)
#define FORMAT "(UART: %6dbps %dbit stop%d %s)"
#define VIEW_TEXT_WIDTH (38)
#define VIEW_TEXT_HEIGHT (8)
#define VIEW_TEXT_MARGIN (2)

#define CONFIG_STATE_IDLE (0)
#define CONFIG_STATE_EXIT (1)
#define SIZE_BITRATES (10)
#define SIZE_BIT_LENGTHS (4)
#define SIZE_STOP_BITS (2)
#define SIZE_PARITIES (3)
#define PARAMETER_COUNT (4)

class Config_View {
  public:
    inline uint32_t getBitrate() {
      return (uint32_t)_bitrate_Selecters[ _pos_value[0] ].getValue() * 100;
    }
    inline uint16_t getBitLength() {
      return _bit_length_Selecters[ _pos_value[1] ].getValue();
    }
    inline uint16_t getStopBit() {
      return _stop_bit_Selecters[ _pos_value[2] ].getValue();
    }
    inline uint16_t getParity() {
      return _parity_Selecters[ _pos_value[3] ].getValue();
    }
    uint32_t getConfig();
    void print_settings();
    void init();
    void open();
    void close();
    int updateState();
    void printValue( uint16_t color );
  private:
    int16_t _lcd_width;
    int16_t _text_width;
    int16_t _lcd_height;
    uint16_t _pos_select = 0;
    uint16_t _pos_value[PARAMETER_COUNT] = { 9, 3, 0, 0, };
    uint16_t _offsets[PARAMETER_COUNT] = { 0, 3, 5, 7, };
    uint16_t _size_value[PARAMETER_COUNT] = { SIZE_BITRATES, SIZE_BIT_LENGTHS, SIZE_STOP_BITS, SIZE_PARITIES, };
    Label _parameters[PARAMETER_COUNT] = { Label("bit rate:"), Label("bit length:"), Label("stop bits:"), Label("parity:"), };
    Selecter _bitrate_Selecters[SIZE_BITRATES] = { Selecter("300", 3), Selecter("600", 6), Selecter("1200", 12), Selecter("2400", 24), Selecter("4800", 48), Selecter("9600", 96), Selecter("19200", 192), Selecter("38400", 384), Selecter("57600", 576), Selecter("115200", 1152), };
    Selecter _bit_length_Selecters[SIZE_BIT_LENGTHS] = { Selecter("5bit", 5), Selecter("6bit", 6), Selecter("7bit", 7), Selecter("8bit", 8), };
    Selecter _stop_bit_Selecters[SIZE_STOP_BITS] = { Selecter("1bit", 1), Selecter("2bit", 2), };
    Selecter _parity_Selecters[SIZE_PARITIES] = { Selecter("none", 0), Selecter("odd", 1), Selecter("even", 2), };
    FunctionButton _funcA = FunctionButton( &M5.BtnA );
    FunctionButton _funcB = FunctionButton( &M5.BtnB );
    FunctionButton _funcC = FunctionButton( &M5.BtnC );
    Selecter *getSelecter( uint16_t pos );
};

#endif
