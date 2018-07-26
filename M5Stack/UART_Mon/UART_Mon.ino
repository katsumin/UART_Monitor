
#include <M5Stack.h>
#include "RS232C_View.h"

RS232C_View view = RS232C_View();

#define TITLE "LINE MONITOR"

void setup() {
  M5.begin();

  // LCD
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setCursor( 3 * TEXT_WIDTH, 0 * TEXT_HEIGHT);
  M5.Lcd.print(TITLE);

  // Serial
  Serial.begin(115200);

  // View
  view.init();
}

void loop() {
  view.updateState();
  delay(50);
  M5.update();
}
