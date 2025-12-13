#include "main.h"
#include "ST-LIB.hpp"

#include "ST-LIB_LOW/DigitalInput2.hpp"
#include "ST-LIB_LOW/DigitalOutput2.hpp"

using ST_LIB::DigitalInputDomain;
using ST_LIB::DigitalOutputDomain;

constexpr DigitalOutputDomain::DigitalOutput led1{ST_LIB::PB0};
constexpr DigitalOutputDomain::DigitalOutput led2{ST_LIB::PE1};
constexpr DigitalOutputDomain::DigitalOutput led3{ST_LIB::PB14};




#define TEST_0


#ifdef TEST_0
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
// Basic test with a buffer in D2
constexpr auto my_uint32_t = MPUDomain::Buffer<uint32_t>();

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, my_uint32_t>;
  myBoard::init();
  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  [[maybe_unused]] auto my_buffer = myBoard::instance_of<my_uint32_t>().template as<my_uint32_t>();

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
// Basic test with a buffer in D1
constexpr auto my_uint32_t = MPUDomain::Buffer<uint32_t>(MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D1);

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, my_uint32_t>;
  myBoard::init();
  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  [[maybe_unused]] auto my_buffer = myBoard::instance_of<my_uint32_t>().template as<my_uint32_t>();

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

#ifdef TEST_3
// Basic test with a buffer in D3
constexpr auto my_buff = MPUDomain::Buffer<uint32_t>(MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D3);

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, my_buff>;
  myBoard::init();
  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  [[maybe_unused]] auto my_buffer = myBoard::instance_of<my_buff>().template as<my_buff>();

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

#ifdef TEST_4
// Fail test (too much memory requested)
constexpr auto my_buff = MPUDomain::Buffer<uint32_t[100000]>(MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D3);

int main(void) {
  

  using myBoard = ST_LIB::Board<led1, led2, led3, my_buff>;
  myBoard::init();
  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  [[maybe_unused]] auto my_buffer = myBoard::instance_of<my_buff>().template as<my_buff>();

  Time::register_low_precision_alarm(100, [&]() {
    green_led.toggle();
    yellow_led.toggle();
    gred_led.toggle();
  });

  STLIB::start();

  while (1) {
    STLIB::update();
  }
}
#endif

#ifdef TEST_5
// Cannot request any type of buffer other than the one defined
constexpr auto my_buff = MPUDomain::Buffer<uint32_t>();

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, my_buff>;
  myBoard::init();
  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  [[maybe_unused]] auto my_buffer = myBoard::instance_of<my_buff>().template as<uint32_t>();

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

#ifdef TEST_6
// Ask for non-cached and cached memory on the same domain
constexpr auto my_buff = MPUDomain::Buffer<uint32_t[100]>();
constexpr auto my_buff2 = MPUDomain::Buffer<uint32_t[200]>(MPUDomain::MemoryType::Cached);

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, my_buff, my_buff2>;
  myBoard::init();
  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  [[maybe_unused]] auto my_buffer = myBoard::instance_of<my_buff>().template as<my_buff>();
  [[maybe_unused]] auto my_buffer2 = myBoard::instance_of<my_buff2>().template as<my_buff2>();

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

#ifdef TEST_7
// Ask for different alignment buffers
constexpr auto my_buff = MPUDomain::Buffer<uint8_t[100]>();
constexpr auto my_buff2 = MPUDomain::Buffer<uint32_t[200]>();

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, my_buff, my_buff2>;
  myBoard::init();
  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  [[maybe_unused]] auto my_buffer = myBoard::instance_of<my_buff>().template as<my_buff>();
  [[maybe_unused]] auto my_buffer2 = myBoard::instance_of<my_buff2>().template as<my_buff2>();

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

#ifdef TEST_8
// Request a non-POD type fails
constexpr auto my_buff = MPUDomain::Buffer<std::vector<int>>();

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, my_buff>;
  myBoard::init();
  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  [[maybe_unused]] auto my_buffer = myBoard::instance_of<my_buff>().template as<my_buff>();

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
