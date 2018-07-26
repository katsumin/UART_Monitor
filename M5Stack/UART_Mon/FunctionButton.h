#ifndef _FUNC_BTN_H_
#define _FUNC_BTN_H_
#include <M5Stack.h>

#define TEXT_HEIGHT (15)
#define WIDTH (60)
#define POS_A_X (36)
#define POS_B_X (130)
#define POS_C_X (224)

class FunctionButton {
  private:
    Button  *_button;
  public:
    FunctionButton( Button *button );
    void set( char *label );
    inline Button *getButton() {
      return _button;
    }
};

#endif
