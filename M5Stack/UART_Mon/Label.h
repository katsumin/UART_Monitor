#ifndef _LABEL_H_
#define _LABEL_H_
#include <M5Stack.h>

class Label {
public:
  Label( char *label ) {
    _label = label;
  };
  inline char *getLabel() {
    return _label;
  }
  inline void setX( uint16_t x) {
    _x = x;
  }
  inline void setY( uint16_t y ) {
    _y = y;
  }
  inline uint16_t getX() {
    return _x;
  }
  inline uint16_t getY() {
    return _y;
  }
  void print( uint16_t color );
private:
  char *_label;
  uint16_t _x;
  uint16_t _y;
};

#endif
