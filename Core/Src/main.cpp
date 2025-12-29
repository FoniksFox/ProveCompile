#include "main.h"
#include "ST-LIB.hpp"

#include "ST-LIB_LOW/DigitalInput2.hpp"
#include "ST-LIB_LOW/DigitalOutput2.hpp"

using ST_LIB::DigitalInputDomain;
using ST_LIB::DigitalOutputDomain;
using ST_LIB::SPIDomain;

constexpr DigitalOutputDomain::DigitalOutput led1{ST_LIB::PB0};
constexpr DigitalOutputDomain::DigitalOutput led2{ST_LIB::PE1};
constexpr DigitalOutputDomain::DigitalOutput led3{ST_LIB::PB14};




#define TEST_2


#ifdef TEST_0
// Do nothing
int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3>;
  myBoard::init();
  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  Time::register_low_precision_alarm(100, [&]() {
    green_led.toggle();
    yellow_led.toggle();
    gred_led.toggle();
  });

  while (1) {
    STLIB::update();
  }
}
#endif

#ifdef TEST_1
// Declare a master spi
constexpr auto my_spi = SPIDomain::Device(
    SPIDomain::SPIMode::MASTER, SPIDomain::SPIPeripheral::spi2, 1000000,
    ST_LIB::PB13, ST_LIB::PC2, ST_LIB::PC3, ST_LIB::PB12
);

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, my_spi>;
  myBoard::init();
  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  [[maybe_unused]] auto &spi_instance = myBoard::instance_of<my_spi>();
  [[maybe_unused]] auto spi_wrapper = SPIDomain::SPIWrapper<my_spi>(spi_instance);

  Time::register_low_precision_alarm(100, [&]() {
    green_led.toggle();
    yellow_led.toggle();
    gred_led.toggle();
  });

  while (1) {
    STLIB::update();
  }
}
#endif

#ifdef TEST_2
// Declare a slave spi
constexpr auto my_spi = SPIDomain::Device(
    SPIDomain::SPIMode::SLAVE, SPIDomain::SPIPeripheral::spi2, 1000000,
    ST_LIB::PB13, ST_LIB::PC2, ST_LIB::PC3, ST_LIB::PB12
);

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, my_spi>;
  myBoard::init();
  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  [[maybe_unused]] auto &spi_instance = myBoard::instance_of<my_spi>();
  [[maybe_unused]] auto spi_wrapper = SPIDomain::SPIWrapper<my_spi>(spi_instance);

  Time::register_low_precision_alarm(100, [&]() {
    green_led.toggle();
    yellow_led.toggle();
    gred_led.toggle();
  });

  while (1) {
    STLIB::update();
  }
}
#endif

void Error_Handler(void) {
  ErrorHandler("HAL error handler triggered");
  while (1) {
  }
}

extern "C" {
void assert_failed(uint8_t *file, uint32_t line) {
  while (1) {
  }
}
}
