#include <stdio.h>
#include <Arduino.h>


class MAX6954{
  private:
    int _data;
    int _clk;
    int _cs;
    void chip_select_low(void) {
      digitalWrite(_cs, LOW);
    };
    void chip_select_high(void) {
      digitalWrite(_cs, HIGH);
    };
    void sclk() {
      digitalWrite(_clk, HIGH);
      digitalWrite(_clk, LOW);
    };
    void send_first_8(char data) {
      char abit;
      char index;
      chip_select_low();
      for (index = 7; index >= 0; index--) {
        abit = ((data >> index) & 0x01);
        if (abit == 1)
        {
          digitalWrite(_data, HIGH);
          sclk();
        } else {
          digitalWrite(_data, LOW);
          sclk();
        }
      }
    };
    void send_second_8(char data) {
      char abit;
      char index;
      for (index = 7; index >= 0; index--) {
        abit = ((data >> index) & 0x01);
        if (abit == 1)
        {
          digitalWrite(_data, HIGH);
        } else {
          digitalWrite(_data, LOW);
        }
        if (index == 0) {
          digitalWrite(_clk, HIGH);
        } else {
          sclk();
        }
      }
      chip_select_high();
      digitalWrite(_clk, LOW);
    };
  public:
    MAX6954(int data, int clk, int cs){
      _data = data;
      _clk = clk;
      _cs = cs;
      pinMode(_data, OUTPUT);
      pinMode(_clk, OUTPUT);
      pinMode(_cs, OUTPUT);
    };
    void set_char(byte addr, char c){
      send_first_8(addr);
      send_second_8(c);
    };
  
};
