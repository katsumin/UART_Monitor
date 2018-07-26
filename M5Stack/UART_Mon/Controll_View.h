#ifndef _CTRL_VIEW_H_
#define _CTRL_VIEW_H_
#include <M5Stack.h>

#define PAGE_COUNT (2)
#define BUFFER_SIZE (16384)
#define TEXT_WIDTH (8)
#define TEXT_HEIGHT (15)
#define SPRITE_WIDTH (36)
#define SPRITE_HEIGHT (8)
#define SCROLL_SIZE (TEXT_WIDTH * 2)
#define WIDTH (SPRITE_WIDTH / 2)
#define KIND_COUNT (7)
#define PAGE_HEIGHT (KIND_COUNT * TEXT_HEIGHT)
#define PAGE_OFFSET_X (TEXT_WIDTH * 4)
#define PAGE_OFFSET_Y (TEXT_HEIGHT * 3)
#define HIGH_LEVEL (0)
#define LOW_LEVEL (SPRITE_HEIGHT - 1)
#define RTS_INDEX (0)
#define CTS_INDEX (1)
#define DTR_INDEX (2)
#define DSR_INDEX (3)
#define DCD_INDEX (4)
#define CONTROL_LINES (5)

union Type {
  struct {
    int rts:1;
    int cts:1;
    int dtr:1;
    int dsr:1;
    int dcd:1;
  } _bit;
  byte _byte;
};

class Controll_View {
  public:
    void init();
    boolean scroll(boolean forceUpdate);
    void pushSprite();
    void clear();
    inline uint16_t getCount() {
      return _count;
    }
    void refresh(int start_pos);
  private:
    uint16_t  _count = 0;
    Type      _pre;
    Type      _cur;
    byte       *_buffer;
    TFT_eSprite _page[PAGE_COUNT][CONTROL_LINES] = { 
      { TFT_eSprite(&M5.Lcd), TFT_eSprite(&M5.Lcd), TFT_eSprite(&M5.Lcd), TFT_eSprite(&M5.Lcd), TFT_eSprite(&M5.Lcd) },
      { TFT_eSprite(&M5.Lcd), TFT_eSprite(&M5.Lcd), TFT_eSprite(&M5.Lcd), TFT_eSprite(&M5.Lcd), TFT_eSprite(&M5.Lcd) },
    };
    char      *_labels[CONTROL_LINES] = {"RTS:", "CTS:", "DTR:", "DSR:", "DCD:", };
    uint16_t  _colors[CONTROL_LINES] = { TFT_YELLOW, TFT_GREEN, TFT_YELLOW, TFT_GREEN, TFT_GREEN, };
    int       _gpio_pins[CONTROL_LINES] = {13,15, 2, 5,0};

    void outputData( int buffer_pos, int spritePage, int x );
    Type getControlPins();
    byte getData( int buffer_pos );
    void setData( int buffer_pos, byte data );
};

#endif
