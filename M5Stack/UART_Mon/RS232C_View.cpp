#include "RS232C_View.h"

void RS232C_View::init() {
  //
  dataView[0].init( 0, TFT_YELLOW, "TxD:" );
  dataView[1].init( 1, TFT_GREEN, "RxD:" );
  ctrlView.init();

  configView.print_settings();
  configView.init();

  _funcA.set("START");
  _funcB.set("");
  _funcC.set("CONFIG");
}

void RS232C_View::scroll(int txd, int rxd) {
  boolean changed = ctrlView.scroll( txd > 0 || rxd > 0 );
  if ( changed ) {
    dataView[0].scroll(txd);
    dataView[1].scroll(rxd);
  }
}

void RS232C_View::pushSprite() {
  dataView[0].pushSprite();
  dataView[1].pushSprite();
  ctrlView.pushSprite();
}

void RS232C_View::clear() {
  Serial.println("clear !");
  dataView[0].clear();
  dataView[1].clear();
  ctrlView.clear();
}

int RS232C_View::updateState() {
  int txd = -1;
  int rxd = -1;
  int count = -1;
  switch ( _state ) {
    case RS232C_STATE_IDLE:
      if ( _funcA.getButton()->wasPressed() ) {
        // モニタ開始
        uint32_t bitrate = configView.getBitrate();
        uint32_t rs_config = configView.getConfig();
        configView.print_settings();
        pinMode(TxDPin, INPUT_PULLUP);
        _serialTxd.begin(bitrate, rs_config, TxDPin, -1);
        pinMode(RxDPin, INPUT_PULLUP);
        _serialRxd.begin(bitrate, rs_config, RxDPin, -1);
        clear();
        _funcA.set("STOP");
        if ( dataView[0].getMode() == MODE_BINARY ) {
          _funcB.set("CHARACTOR");
        } else {
          _funcB.set("BINARY");
        }
        _funcC.set("");
        _state = RS232C_STATE_RUNNING;
        _disp_pos = -1;
      }
      if ( _funcC.getButton()->wasPressed() ) {
        _serialTxd.end();
        _serialRxd.end();
        configView.open();
        _state = RS232C_STATE_CONFIG;
      }
      break;
    case RS232C_STATE_RUNNING:
      if ( _serialTxd.available() ) {
        txd = _serialTxd.read();
      }
      if ( _serialRxd.available() ) {
        rxd = _serialRxd.read();
      }
      pushSprite();
      scroll( txd, rxd );
      if ( _funcA.getButton()->wasPressed() ) {
        count = dataView[0].getCount();
        if ( count > 0 ) {
          // データ閲覧モード
          _funcA.set("CLEAR");
          if ( count > WIDTH * PAGE_COUNT ) {
            // スクロール済みなら、前ページに戻れるように
            _funcB.set("REVERSE");
            _disp_pos = count - WIDTH * PAGE_COUNT;
          } else {
            _funcB.set("");
          }
          _funcC.set(""); // 停止直後は、バッファの末端を表示しているはず
          _state = RS232C_STATE_CAPTURED;
        } else {
          // 停止
          _funcA.set("START");
          _funcB.set("");
          _funcC.set("CONFIG");
          _state = RS232C_STATE_IDLE;
        }
      }
      if ( _funcB.getButton()->wasPressed() ) {
        // 表示モード変更
        dataView[0].changeMode();
        dataView[1].changeMode();
        int mode = dataView[0].getMode();
        if ( mode == MODE_BINARY ) {
          _funcB.set("CHARACTOR");
        } else {
          _funcB.set("BINARY");
        }
      }
      break;
    case RS232C_STATE_CAPTURED:
      if ( _funcA.getButton()->wasPressed() ) {
        // クリア
        _funcA.set("START");
        _funcB.set("");
        _funcC.set("CONFIG");
        clear();
        pushSprite();
        _state = RS232C_STATE_IDLE;
      }
      count = dataView[0].getCount();
      if ( 0 < _disp_pos && _funcB.getButton()->wasPressed() ) {
        // 前ページ
        _disp_pos -= WIDTH * PAGE_COUNT;
        if ( _disp_pos <= 0 ) {
          _disp_pos = 0;
          _funcB.set("");
        }
        dataView[0].refresh( _disp_pos );
        dataView[1].refresh( _disp_pos );
        ctrlView.refresh( _disp_pos );
        _funcC.set("FORWARD");
      }
      if ( 0 <= _disp_pos && _disp_pos < count - WIDTH * PAGE_COUNT && _funcC.getButton()->wasPressed() ) {
        // 次ページ
        _disp_pos += WIDTH * PAGE_COUNT;
        if ( _disp_pos >= count - WIDTH * PAGE_COUNT ) {
          _funcC.set("");
        }
        dataView[0].refresh( _disp_pos );
        dataView[1].refresh( _disp_pos );
        ctrlView.refresh( _disp_pos );
        _funcB.set("REVERSE");
      }
      break;
    case RS232C_STATE_CONFIG:
      if ( configView.updateState() == CONFIG_STATE_EXIT ) {
        configView.close();
        _funcA.set("START");
        _funcB.set("");
        _funcC.set("CONFIG");
        _state = RS232C_STATE_IDLE;
      }
      break;
  }
  return _state;
}

