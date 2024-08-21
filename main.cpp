#include "mbed.h"
#include <cstdint>
#include <ctime>

RawSerial pc(USBTX,USBRX,9600);
DigitalOut myled(LED1);

I2C fs3000(I2C_SDA, I2C_SCL);

#define FS3000_DEVICE_ADDRESS 0x28 << 1
#define FS3000_DATA_LENGTH    5

#define FS3000_HIGH_BYTE_MASK 0b00001111

uint16_t read_data();
float get_wind_velocity();

int main() {
  while(1) {
    float wind_speed = get_wind_velocity();
    pc.printf("wind speed: %.2f [m/s]\r\n", wind_speed);
    wait(1);
  }
}

uint16_t read_raw_data() {
  char rx_buf[FS3000_DATA_LENGTH] = {0x00};
  const event_callback_t callback;

  fs3000.read(FS3000_DEVICE_ADDRESS, rx_buf, FS3000_DATA_LENGTH, callback);

  uint8_t high_byte = rx_buf[1];
  uint8_t low_byte = rx_buf[2] & FS3000_HIGH_BYTE_MASK;

  return ((high_byte << 8) | low_byte);
}

float get_wind_velocity() {
  uint16_t raw_val = read_raw_data();

  // pc.printf("wind speed: %d\r\n", raw_val);

  float wind_speed = raw_val * 0.0022 - 1.1685;

  if(wind_speed > 0) {
    return wind_speed;
  } else {
    return 0;
  }
}