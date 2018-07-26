#ifndef _DATA_VIEW_H_
#define _DATA_VIEW_H_
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
#define PAGE_OFFSET_Y (TEXT_HEIGHT * 1)
#define MODE_BINARY (0)
#define MODE_CHARACTOR (1)

class Data_View {
  public:
    void init(uint16_t offset_y, uint16_t textColor, char *label);
    void scroll(int data);
    void pushSprite();
    void clear();
    inline uint16_t getCount() {
      return _count;
    }
    inline int getMode() {
      return _mode;
    }
    inline void setMode( int mode ) {
      _mode = mode;
    }
    void changeMode();
    void refresh(int start_pos);
  private:
    int         _mode = MODE_BINARY;
    uint16_t    _offset_y;
    uint16_t    _textColor;
    uint16_t    _count = 0;
    short       *_buffer;
    TFT_eSprite _page[PAGE_COUNT] = { TFT_eSprite(&M5.Lcd), TFT_eSprite(&M5.Lcd) };

    void outputData( int buffer_pos, int spritePage, int x );
};

#endif
