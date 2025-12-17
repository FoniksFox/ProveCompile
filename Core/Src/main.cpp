#include "main.h"
#include "ST-LIB.hpp"

#include "ST-LIB_LOW/DigitalInput2.hpp"
#include "ST-LIB_LOW/DigitalOutput2.hpp"

using ST_LIB::DigitalInputDomain;
using ST_LIB::DigitalOutputDomain;
using ST_LIB::SdDomain;

constexpr DigitalInputDomain::DigitalInput led1{ST_LIB::PB0};


#define TEST_5


#ifdef TEST_0
// No Sd (the gpios are just because it gives errors otherwise)
int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1>;
  myBoard::init();


  while (1) {
    STLIB::update();
  }
}
#endif

#ifdef TEST_1
// Sd, don't use it, only declare it, without cd/wp nor nothing
constexpr auto sd_card = SdDomain::SdCard<1>{
    SdDomain::Peripheral::sdmmc1,
    std::nullopt,
    std::nullopt,
};
int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<sd_card>;
  myBoard::init();

  while (1) {
    STLIB::update();
  }
}
#endif

#ifdef TEST_2
// Sd, initialize the card
constexpr auto sd_card = SdDomain::SdCard<1>{
    SdDomain::Peripheral::sdmmc1,
    std::nullopt,
    std::nullopt,
};
int main(void) {
    STLIB::start();

  using myBoard = ST_LIB::Board<sd_card>;
  myBoard::init();

  auto sd_instance = SdDomain::SdCardWrapper<sd_card>(myBoard::instance_of<sd_card>());
  sd_instance.init_card();

  while (1) {
    STLIB::update();
  }
}
#endif

#ifdef TEST_3
// Sd, initialize the card with cd/wp
constexpr auto sd_card = SdDomain::SdCard<1>{
    SdDomain::Peripheral::sdmmc1,
    std::pair{DigitalInputDomain::DigitalInput{ST_LIB::PG4}, GPIO_PinState::GPIO_PIN_RESET},
    std::pair{DigitalInputDomain::DigitalInput{ST_LIB::PG3}, GPIO_PinState::GPIO_PIN_SET},
};
int main(void) {
    STLIB::start();

  using myBoard = ST_LIB::Board<sd_card>;
  myBoard::init();

  auto sd_instance = SdDomain::SdCardWrapper<sd_card>(myBoard::instance_of<sd_card>());
  sd_instance.init_card();

  while (1) {
    STLIB::update();
  }
}
#endif

#ifdef TEST_4
// Sd, try to write and read 1 block
constexpr auto sd_card = SdDomain::SdCard<2>{
    SdDomain::Peripheral::sdmmc1,
    std::pair{DigitalInputDomain::DigitalInput{ST_LIB::PG4}, GPIO_PinState::GPIO_PIN_RESET},
    std::pair{DigitalInputDomain::DigitalInput{ST_LIB::PG3}, GPIO_PinState::GPIO_PIN_SET},
};
int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<sd_card>;
  myBoard::init();

  auto sd_instance = SdDomain::SdCardWrapper<sd_card>(myBoard::instance_of<sd_card>());
  sd_instance.init_card();

  auto current_buffer = sd_instance.get_current_buffer();
  std::size_t buf_size = current_buffer->size();
  // Fill buffer with some data
  for (size_t i = 0; i < buf_size; i++) {
      (*current_buffer)[i] = 0xFFFF;
  }
  bool read_complete = false;
  bool write_complete = false;
  while (sd_instance.is_busy()) {}
  sd_instance.write_blocks(0, 2, &write_complete);
  while (!write_complete) {
      STLIB::update();
  }
  current_buffer = sd_instance.get_current_buffer();
  while (sd_instance.is_busy()) {}
  sd_instance.read_blocks(0, 2, &read_complete);
  while (!read_complete) {
      STLIB::update();
  }
  

  while (1) {
    STLIB::update();
  }
}
#endif

#ifdef TEST_5
// Write, Read, Write, Read multiple times
constexpr auto sd_card = SdDomain::SdCard<15>{
    SdDomain::Peripheral::sdmmc1,
    std::pair{DigitalInputDomain::DigitalInput{ST_LIB::PG4}, GPIO_PinState::GPIO_PIN_RESET},
    std::pair{DigitalInputDomain::DigitalInput{ST_LIB::PG3}, GPIO_PinState::GPIO_PIN_SET},
};
int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<sd_card>;
  myBoard::init();

  auto sd_instance = SdDomain::SdCardWrapper<sd_card>(myBoard::instance_of<sd_card>());
  sd_instance.init_card();

  auto current_buffer = sd_instance.get_current_buffer();
  // Fill buffer with some data
  for (size_t i = 0; i < current_buffer->size(); i++) {
    (*current_buffer)[i] = i;
  }

  for (uint32_t iteration = 0; iteration < 100; iteration++) {
    bool read_complete = false;
    bool write_complete = false;
    while (sd_instance.is_busy()) {}
    while(!sd_instance.write_blocks(0, 15, &write_complete)) {}
    while (!write_complete) {
        STLIB::update();
    }
    // Clean the buffer
    for (size_t i = 0; i < current_buffer->size(); i++) {
      (*current_buffer)[i] = 0;
    } 

    current_buffer = sd_instance.get_current_buffer();
    for (size_t i = 0; i < current_buffer->size(); i++) {
      (*current_buffer)[i] = 0xFF;
    }
    while (sd_instance.is_busy()) {}
    while(!sd_instance.read_blocks(0, 15, &read_complete)) {}
    while (!read_complete) {
      STLIB::update();
    }

    auto new_buffer = sd_instance.get_current_buffer();
    // Verify data
    for (size_t i = 0; i < current_buffer->size(); i++) {
      if ((*current_buffer)[i] != i) {
        ErrorHandler("SD Card data mismatch after first read");
      }
      (*new_buffer)[i] = (*current_buffer)[i];
      (*current_buffer)[i] = 0;
    }
    current_buffer = new_buffer;
  }
  

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
