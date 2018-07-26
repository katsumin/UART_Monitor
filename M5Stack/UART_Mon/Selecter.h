#ifndef _SELECTER_H_
#define _SELECTER_H_
#include <M5Stack.h>
#include "Label.h"

class Selecter : public Label {
public:
  Selecter( char *label, uint16_t value ) : Label(label) {
    _value = value;
  };
  inline uint16_t getValue() {
    return _value;
  }
private:
  uint16_t _value;
};

#endif
