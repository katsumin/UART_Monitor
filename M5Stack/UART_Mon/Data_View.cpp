#include "Data_View.h"

void Data_View::changeMode() {
  _mode = (_mode + 1) % 2;
  int pos = getCount() - WIDTH * PAGE_COUNT;
  if ( pos < 0 ) {
    pos = 0;
  }
  refresh(pos);
}

void Data_View::init(uint16_t offset_y, uint16_t textColor, char *label) {
  _offset_y = offset_y;
  _textColor = textColor;
  M5.Lcd.setTextColor(_textColor);
  for ( int i=0; i<PAGE_COUNT; i++ ) {
    M5.Lcd.setCursor(0, PAGE_OFFSET_Y + _offset_y * TEXT_HEIGHT + i * PAGE_HEIGHT);
    M5.Lcd.print(label);

    void* p = _page[i].createSprite(36 * TEXT_WIDTH, 1 * SPRITE_HEIGHT);
    _page[i].setTextColor(_textColor);
    _page[i].fillSprite(TFT_BLACK);
    _page[i].setScrollRect(0, 0, 36 * TEXT_WIDTH, 1 * SPRITE_HEIGHT, TFT_BLACK);
  }

  _buffer = (short *)malloc( BUFFER_SIZE * sizeof(short) );
}

void Data_View::outputData( int buffer_pos, int spritePage, int x ) {
  if ( buffer_pos < 0 ) {
    buffer_pos += BUFFER_SIZE;
  }
  short d = _buffer[buffer_pos];
  if ( d >= 0 ) {
    char buf[16];
    if ( getMode() == MODE_BINARY ) {
      snprintf( buf, sizeof(buf), "%02X", d & 0xff );
    } else {
      snprintf( buf, sizeof(buf), " %c", d & 0xff );
    }
    _page[spritePage].drawString( buf, x * TEXT_WIDTH * 2, 0, 1);
  }
}

void Data_View::refresh(int start_pos) {
  for ( int i=0; i<PAGE_COUNT; i++ ) {
    _page[i].fillSprite(TFT_BLACK);
  }
  int pos = start_pos;
  int count = min( getCount() - start_pos, WIDTH * PAGE_COUNT );
  for ( int i=0; i<count; i++ ) {
    int p = (i / WIDTH);
    int x = (i % WIDTH);
    outputData( pos++, p, x );
  }
  pushSprite();
}

void Data_View::scroll(int data) {
  _count %= BUFFER_SIZE;
  _buffer[_count] = data;

  if ( _count >= WIDTH * PAGE_COUNT ) {
    // scroll
    for ( int i=0; i<PAGE_COUNT; i++ ) {
      _page[i].scroll(-TEXT_WIDTH * 2);

      uint16_t pos = _count + WIDTH * (i - PAGE_COUNT + 1);
      outputData( pos, i, WIDTH - 1 );
    }
  } else {
    // no scroll
    int p = (_count / WIDTH) % PAGE_COUNT;
    int x = (_count % WIDTH);
    outputData( _count, p, x );
  }

  _count++;
}

void Data_View::pushSprite() {
  for ( int i=0; i<PAGE_COUNT; i++ ) {
    _page[i].pushSprite( PAGE_OFFSET_X, PAGE_OFFSET_Y + _offset_y * TEXT_HEIGHT + i * PAGE_HEIGHT);
  }
}

void Data_View::clear() {
  _count = 0;
  for ( int i=0; i<PAGE_COUNT; i++ ) {
    _page[i].fillSprite(TFT_BLACK);
  }
}
