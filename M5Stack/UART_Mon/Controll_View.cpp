#include "Controll_View.h"

void Controll_View::init() {
  for ( int i=0; i<PAGE_COUNT; i++ ) {
    for ( int j=0; j<CONTROL_LINES; j++ ) {
      // LABEL
      M5.Lcd.setTextColor(_colors[j]);
      M5.Lcd.setCursor(0, PAGE_OFFSET_Y + j * TEXT_HEIGHT + i * PAGE_HEIGHT);
      M5.Lcd.print(_labels[j]);
      M5.Lcd.drawFastHLine( 0, PAGE_OFFSET_Y + j * TEXT_HEIGHT + i * PAGE_HEIGHT - 2, M5.Lcd.textWidth(_labels[j]) * 3 / 4 , _colors[j]);

      // GPIO
      pinMode(_gpio_pins[j], INPUT_PULLUP);

      // sprite
      void* p = _page[i][j].createSprite(36 * TEXT_WIDTH, 1 * SPRITE_HEIGHT);
      _page[i][j].setTextColor(_colors[j]);
      _page[i][j].fillSprite(TFT_BLACK);
      _page[i][j].setScrollRect(0, 0, 36 * TEXT_WIDTH, 1 * SPRITE_HEIGHT, TFT_BLACK);
    }
  }

  _buffer = (byte *)malloc( BUFFER_SIZE * sizeof(byte) );

  _pre = _cur = getControlPins();
  setData(_count, _cur._byte);
  _count++;
}

Type Controll_View::getControlPins() {
  Type cur;
  cur._bit.rts = digitalRead(_gpio_pins[RTS_INDEX]);
  cur._bit.cts = digitalRead(_gpio_pins[CTS_INDEX]);
  cur._bit.dtr = digitalRead(_gpio_pins[DTR_INDEX]);
  cur._bit.dsr = digitalRead(_gpio_pins[DSR_INDEX]);
  cur._bit.dcd = digitalRead(_gpio_pins[DCD_INDEX]);

  return cur;
}

boolean Controll_View::scroll(boolean forceUpdate) {
  boolean changed = false;
  _pre = _cur;
  _cur = getControlPins();
  if ( _pre._byte != _cur._byte || forceUpdate ) {
    // 前回サンプリング値と差分あり
    changed = true;
    setData(_count, _cur._byte);

    if ( _count >= WIDTH * PAGE_COUNT ) {
      // scroll
      for ( int i=0; i<PAGE_COUNT; i++ ) {
        for ( int j=0; j<CONTROL_LINES; j++ ) {
          _page[i][j].scroll(-SCROLL_SIZE);
        }
        uint16_t pos = _count + WIDTH * (i - PAGE_COUNT + 1);
        outputData( pos, i, SPRITE_WIDTH * TEXT_WIDTH - SCROLL_SIZE );
      }
    } else {
      // no scroll
      int p = (_count / WIDTH) % 2;
      int x = (_count % WIDTH) * 2;
      outputData( _count, p, x * TEXT_WIDTH );
    }
    _count++;
  }
  return changed;
}

byte Controll_View::getData( int buffer_pos ) {
  if ( buffer_pos < 0 ) {
    buffer_pos += BUFFER_SIZE;
  }
  return _buffer[buffer_pos];
}

void Controll_View::setData( int buffer_pos, byte data ) {
    _count %= BUFFER_SIZE;
    _buffer[_count] = data;
}

void Controll_View::outputData( int buffer_pos, int spritePage, int x ) {
  byte cur_b = getData(buffer_pos);
  byte pre_b = getData(buffer_pos - 1);

  for ( int j=0; j<CONTROL_LINES; j++ ) {
    byte mask = 0x01 << j;
    if ( (pre_b ^ cur_b) & mask ) {
      // state changed 
      _page[spritePage][j].drawFastVLine( x, 0, SPRITE_HEIGHT, _colors[j]);
    }
    
    if ( cur_b & mask ) {
      _page[spritePage][j].drawFastHLine( x, HIGH_LEVEL, SCROLL_SIZE, _colors[j]);
    } else {
      _page[spritePage][j].drawFastHLine( x,  LOW_LEVEL, SCROLL_SIZE, _colors[j]);
    }
  }
}

void Controll_View::pushSprite() {
  for ( int i=0; i<PAGE_COUNT; i++ ) {
    for ( int j=0; j<CONTROL_LINES; j++ ) {
      _page[i][j].pushSprite( PAGE_OFFSET_X, PAGE_OFFSET_Y + j * TEXT_HEIGHT + i * PAGE_HEIGHT);
    }
  }
}

void Controll_View::clear() {
  _count = 0;
  for ( int i=0; i<PAGE_COUNT; i++ ) {
    for ( int j=0; j<CONTROL_LINES; j++ ) {
      _page[i][j].fillSprite(TFT_BLACK);
    }
  }
}

void Controll_View::refresh(int start_pos) {
  for ( int i=0; i<PAGE_COUNT; i++ ) {
    for ( int j=0; j<CONTROL_LINES; j++ ) {
      _page[i][j].fillSprite(TFT_BLACK);
    }
  }
  int pos = start_pos;
  int count = min( getCount() - start_pos, WIDTH * PAGE_COUNT );
  for ( int i=0; i<count; i++ ) {
    int p = (i / WIDTH);
    int x = (i % WIDTH) * 2;
    outputData( pos++, p, x * TEXT_WIDTH );
  }
  pushSprite();
}

