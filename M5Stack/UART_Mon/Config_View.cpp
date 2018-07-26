#include "Config_View.h"

int Config_View::updateState() {
  if ( _funcA.getButton()->wasPressed() ) {
    _parameters[_pos_select].print( TFT_WHITE );
    _pos_select = (_pos_select + 1) % PARAMETER_COUNT; 
    _parameters[_pos_select].print( TFT_GREEN );
    return CONFIG_STATE_IDLE;
  }
  if ( _funcB.getButton()->wasPressed() ) {
    printValue( TFT_WHITE );
    _pos_value[_pos_select] = (_pos_value[_pos_select] + 1) % _size_value[_pos_select];
    printValue( TFT_YELLOW );
    return CONFIG_STATE_IDLE;
  }
  if ( _funcC.getButton()->wasPressed() ) {
    return CONFIG_STATE_EXIT;
  }
  return CONFIG_STATE_IDLE;
}

Selecter *Config_View::getSelecter( uint16_t pos ) {
  Selecter *current = NULL;
  switch ( pos ) {
    case 0:
    default:
      current = _bitrate_Selecters;
      break;
    case 1:
      current = _bit_length_Selecters;
      break;
    case 2:
      current = _stop_bit_Selecters;
      break;
    case 3:
      current = _parity_Selecters;
      break;
  }

  return current;
}

void Config_View::printValue( uint16_t color ) {
  Selecter *current = getSelecter( _pos_select );
  if ( current != NULL ) {
    current[ _pos_value[_pos_select] ].print( color );
  }
}

void Config_View::init() {
  _lcd_width = M5.Lcd.width();
  _text_width = M5.Lcd.textWidth(" ");
  _lcd_height = M5.Lcd.height();

  int16_t org_x = (_lcd_width - (VIEW_TEXT_WIDTH + VIEW_TEXT_MARGIN) * _text_width)/2 + 1 * _text_width;
  int16_t org_y = (_lcd_height - (VIEW_TEXT_HEIGHT + VIEW_TEXT_MARGIN) * TEXT_HEIGHT)/2 + 1 * TEXT_HEIGHT;
  int len[2] = {0, 0};
  int16_t x = 0;
  int16_t y = 0;

  // bit_rate
  _parameters[0].setX( org_x );
  _parameters[0].setY( org_y + _offsets[0] * TEXT_HEIGHT );
  for ( int i=0; i<_size_value[0]; i++ ) {
    x = org_x + (len[i % 2] + 12) * _text_width;
    y = org_y + (i % 2) * TEXT_HEIGHT;
    _bitrate_Selecters[i].setX(x);
    _bitrate_Selecters[i].setY(y);
    char *text = _bitrate_Selecters[i].getLabel();
    len[i % 2] += strlen(text) + 1;
  }

  // bit length / stop bit / parity
  for ( int i=1; i<PARAMETER_COUNT; i++ ) {
    _parameters[i].setX( org_x );
    _parameters[i].setY( org_y + _offsets[i] * TEXT_HEIGHT );
    y = org_y + _offsets[i] * TEXT_HEIGHT;
    len[0] = 0;
    Selecter *selecter = getSelecter(i);
    for ( int j=0; j<_size_value[i]; j++ ) {
      int16_t x = org_x + (len[0] + 12) * _text_width;
      selecter[j].setX(x);
      selecter[j].setY(y);
      len[0] += strlen(selecter[j].getLabel()) + 1;
    }
  }
}

void Config_View::open() {
  M5.Lcd.fillRect( 
    (_lcd_width - (VIEW_TEXT_WIDTH + VIEW_TEXT_MARGIN) * _text_width)/2,
    (_lcd_height - (VIEW_TEXT_HEIGHT + VIEW_TEXT_MARGIN) * TEXT_HEIGHT)/2,
    (VIEW_TEXT_WIDTH + VIEW_TEXT_MARGIN) * _text_width,
    (VIEW_TEXT_HEIGHT + VIEW_TEXT_MARGIN) * TEXT_HEIGHT,
    TFT_BLACK);
  M5.Lcd.drawRoundRect(
    (_lcd_width - (VIEW_TEXT_WIDTH + VIEW_TEXT_MARGIN) * _text_width)/2, 
    (_lcd_height - (VIEW_TEXT_HEIGHT + VIEW_TEXT_MARGIN) * TEXT_HEIGHT)/2, 
    (VIEW_TEXT_WIDTH + VIEW_TEXT_MARGIN) * _text_width, 
    (VIEW_TEXT_HEIGHT+VIEW_TEXT_MARGIN) * TEXT_HEIGHT, 
    5, TFT_BLUE);

  for ( uint16_t i=0; i<PARAMETER_COUNT; i++ ) {
    if ( _pos_select == i ) {
      _parameters[i].print(TFT_GREEN);
    } else {
      _parameters[i].print(TFT_WHITE);
    }

    Selecter *selecter = getSelecter(i);
    uint16_t pos = _pos_value[i];
    for ( uint16_t j=0; j<_size_value[i]; j++ ) {
      if ( pos == j ) {
        selecter[j].print(TFT_YELLOW);
      } else {
        selecter[j].print(TFT_WHITE);
      }
    }
  }

  _funcA.set("SELECT");
  _funcB.set("VALUE");
  _funcC.set("CLOSE");
}

void Config_View::close() {
  M5.Lcd.fillRect( 
    (_lcd_width - (VIEW_TEXT_WIDTH + VIEW_TEXT_MARGIN) * _text_width)/2,
    (_lcd_height - (VIEW_TEXT_HEIGHT + VIEW_TEXT_MARGIN) * TEXT_HEIGHT)/2,
    (VIEW_TEXT_WIDTH + VIEW_TEXT_MARGIN) * _text_width,
    (VIEW_TEXT_HEIGHT + VIEW_TEXT_MARGIN) * TEXT_HEIGHT,
    TFT_BLACK);

  print_settings();
}

void Config_View::print_settings() {
  char buf[64];
  snprintf( buf, sizeof(buf), FORMAT, getBitrate(), getBitLength(), getStopBit(), (getParity() == 0) ?"none" :(getParity() == 1) ?"odd " :"even" );
  int16_t lcd_width = M5.Lcd.width();
  int16_t text_width = M5.Lcd.textWidth(buf);
  M5.Lcd.fillRect( lcd_width - text_width, 0, text_width, TEXT_HEIGHT, TFT_BLACK);
  M5.Lcd.setCursor( lcd_width - text_width, 0);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.print(buf);
}

uint32_t Config_View::getConfig() {
  uint16_t _bit_length = getBitLength();
  uint16_t _parity = getParity();
  uint16_t _stop_bit = getStopBit();
  switch ( _bit_length ) {
    case 5:
      switch ( _parity ) {
        case 2: // even
          if ( _stop_bit == 2 ) {
            return SERIAL_5E2;
          } else {
            return SERIAL_5E1;
          }
        case 1: // odd
          if ( _stop_bit == 2 ) {
            return SERIAL_5O2;
          } else {
            return SERIAL_5O1;
          }
        case 0: // none
        default:
          if ( _stop_bit == 2 ) {
            return SERIAL_5N2;
          } else {
            return SERIAL_5N1;
          }
      }
    case 6:
      switch ( _parity ) {
        case 2: // even
          if ( _stop_bit == 2 ) {
            return SERIAL_6E2;
          } else {
            return SERIAL_6E1;
          }
        case 1: // odd
          if ( _stop_bit == 2 ) {
            return SERIAL_6O2;
          } else {
            return SERIAL_6O1;
          }
        case 0: // none
        default:
          if ( _stop_bit == 2 ) {
            return SERIAL_6N2;
          } else {
            return SERIAL_6N1;
          }
      }
    case 7:
      switch ( _parity ) {
        case 2: // even
          if ( _stop_bit == 2 ) {
            return SERIAL_7E2;
          } else {
            return SERIAL_7E1;
          }
        case 1: // odd
          if ( _stop_bit == 2 ) {
            return SERIAL_7O2;
          } else {
            return SERIAL_7O1;
          }
        case 0: // none
        default:
          if ( _stop_bit == 2 ) {
            return SERIAL_7N2;
          } else {
            return SERIAL_7N1;
          }
      }
    case 8:
    default:
      switch ( _parity ) {
        case 2: // even
          if ( _stop_bit == 2 ) {
            return SERIAL_8E2;
          } else {
            return SERIAL_8E1;
          }
        case 1: // odd
          if ( _stop_bit == 2 ) {
            return SERIAL_8O2;
          } else {
            return SERIAL_8O1;
          }
        case 0: // none
        default:
          if ( _stop_bit == 2 ) {
            return SERIAL_8N2;
          } else {
            return SERIAL_8N1;
          }
      }
  }
}

