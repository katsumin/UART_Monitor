#include "FunctionButton.h"

FunctionButton::FunctionButton( Button *button ) {
  _button = button;
}

void FunctionButton::set( char *label ) {
  uint16_t x_pos;
  uint16_t y_pos = M5.Lcd.height() - TEXT_HEIGHT;
  if ( _button == &M5.BtnA ) {
    x_pos = POS_A_X;
  } else if ( _button == &M5.BtnB ) {
    x_pos = POS_B_X;
  } else if ( _button == &M5.BtnC ) {
    x_pos = POS_C_X;
  }
  M5.Lcd.fillRect( x_pos, y_pos - 1, WIDTH, TEXT_HEIGHT, BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor( x_pos + WIDTH / 2 - M5.Lcd.textWidth(label) / 2 , y_pos + 3);
  M5.Lcd.print(label);
  M5.Lcd.drawRoundRect( x_pos, y_pos - 1, WIDTH, TEXT_HEIGHT, 2, WHITE);
}

