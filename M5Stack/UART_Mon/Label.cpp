#include "Label.h"

void Label::print( uint16_t color ) {
  M5.Lcd.setCursor( getX(), getY() );
  M5.Lcd.setTextColor(color);
  M5.Lcd.print(getLabel());
}

