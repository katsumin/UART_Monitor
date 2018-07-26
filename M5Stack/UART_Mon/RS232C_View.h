#ifndef _RS232C_View_H_
#define _RS232C_View_H_
#include <M5Stack.h>
#include "Data_View.h"
#include "Controll_View.h"
#include "Config_View.h"
#include "FunctionButton.h"

#define PAGE_COUNT (2)
#define KIND_COUNT (7)
#define SPRITE_HEIGHT (8)
#define SPRITE_WIDTH (36)
#define TEXT_HEIGHT (15)
#define TEXT_WIDTH (8)
#define PAGE_HEIGHT (KIND_COUNT * TEXT_HEIGHT)
#define TXD_POS (0)
#define RXD_POS (1)
#define RTS_POS (2)
#define CTS_POS (3)
#define DTR_POS (4)
#define DSR_POS (5)
#define DCD_POS (6)
#define HIGH_LEVEL (0)
#define LOW_LEVEL (SPRITE_HEIGHT - 1)
#define SCROLL_SIZE (TEXT_WIDTH * 2)
#define PAGE_OFFSET_X (TEXT_WIDTH * 4)
#define PAGE_OFFSET_Y (TEXT_HEIGHT * 1)
#define WIDTH (SPRITE_WIDTH / 2)

#define BUFFER_SIZE (16384)
#define TXD_INDEX (0)
#define RXD_INDEX (1)
#define CTL_INDEX (2)

#define RS232C_STATE_IDLE (0)
#define RS232C_STATE_RUNNING (1)
#define RS232C_STATE_CAPTURED (2)
#define RS232C_STATE_CONFIG (3)

#define TxDPin 17
#define RxDPin 16

class RS232C_View {
  public:
    void init();
    void scroll(int txd, int rxd);
    void pushSprite();
    void clear();
    int updateState();
  private:
    int _disp_pos = -1;
    int _state = RS232C_STATE_IDLE;
    Data_View   dataView[2];
    Controll_View ctrlView;
    Config_View configView;
    HardwareSerial _serialTxd = HardwareSerial(1);
    HardwareSerial _serialRxd = HardwareSerial(2);
    FunctionButton _funcA = FunctionButton( &M5.BtnA );
    FunctionButton _funcB = FunctionButton( &M5.BtnB );
    FunctionButton _funcC = FunctionButton( &M5.BtnC );
};

#endif
