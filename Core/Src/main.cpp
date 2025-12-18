#include "main.h"
#include "ST-LIB.hpp"

#include "ST-LIB_LOW/DigitalInput2.hpp"
#include "ST-LIB_LOW/DigitalOutput2.hpp"

using ST_LIB::DigitalInputDomain;
using ST_LIB::DigitalOutputDomain;
using ST_LIB::SdDomain;

// constexpr DigitalOutputDomain::DigitalOutput led1{ST_LIB::PB0};
// constexpr DigitalOutputDomain::DigitalOutput led2{ST_LIB::PE1};
// constexpr DigitalOutputDomain::DigitalOutput led3{ST_LIB::PB14};




#define TEST_2


#ifdef TEST_0
// No Buffers requested
int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3>;
  myBoard::init();
  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  while (1) {
    STLIB::update();
  }
}
#endif

#ifdef TEST_1
constexpr auto my_sd_card = SdDomain::SdCard<4>{
  SdDomain::Peripheral::sdmmc1,
  std::pair{DigitalInputDomain::DigitalInput{ST_LIB::PG4}, GPIO_PinState::GPIO_PIN_RESET},
  std::pair{DigitalInputDomain::DigitalInput{ST_LIB::PG3}, GPIO_PinState::GPIO_PIN_SET},
};

constexpr auto my_var_0 = MPUDomain::Buffer<uint8_t>{};
constexpr auto my_var_1 = MPUDomain::Buffer<uint16_t>{};

constexpr auto my_var_2 = MPUDomain::Buffer<uint32_t>{};
constexpr auto my_var_3 = MPUDomain::Buffer<std::array<uint8_t, 128>>{};

constexpr auto my_var_4 = MPUDomain::Buffer<std::array<uint16_t, 64>>{};
constexpr auto my_var_5 = MPUDomain::Buffer<std::array<uint32_t, 32>>{};

constexpr auto my_var_6 = MPUDomain::Buffer<std::array<uint8_t, 256>>{};
constexpr auto my_var_7 = MPUDomain::Buffer<std::array<uint16_t, 128>>{};

constexpr auto my_packet_0 = MdmaPacketDomain::MdmaPacket<uint8_t, uint16_t>::Request{};
constexpr auto my_packet_1 = MdmaPacketDomain::MdmaPacket<uint32_t, std::array<uint8_t, 128>>::Request{};
constexpr auto my_packet_2 = MdmaPacketDomain::MdmaPacket<std::array<uint16_t, 64>, std::array<uint32_t, 32>>::Request{};
constexpr auto my_packet_3 = MdmaPacketDomain::MdmaPacket<std::array<uint8_t, 256>, std::array<uint16_t, 128>>::Request{};
int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<my_sd_card,
                                my_var_0, my_var_1, my_var_2, my_var_3,
                                my_var_4, my_var_5, my_var_6, my_var_7,
                                my_packet_0, my_packet_1, my_packet_2, my_packet_3>;
  myBoard::init();

  auto sd = SdDomain::SdCardWrapper<my_sd_card>(myBoard::instance_of<my_sd_card>());

  auto var0 = myBoard::instance_of<my_var_0>().as<my_var_0>();
  auto var1 = myBoard::instance_of<my_var_1>().as<my_var_1>();
  auto var2 = myBoard::instance_of<my_var_2>().as<my_var_2>();
  auto var3 = myBoard::instance_of<my_var_3>().as<my_var_3>();
  auto var4 = myBoard::instance_of<my_var_4>().as<my_var_4>();
  auto var5 = myBoard::instance_of<my_var_5>().as<my_var_5>();
  auto var6 = myBoard::instance_of<my_var_6>().as<my_var_6>();
  auto var7 = myBoard::instance_of<my_var_7>().as<my_var_7>();

  auto packet0 = MdmaPacketDomain::MdmaPacket<uint8_t, uint16_t>(
      myBoard::instance_of<my_packet_0>(), 1, var0, var1);
  auto packet1 = MdmaPacketDomain::MdmaPacket<uint32_t, std::array<uint8_t, 128>>(
      myBoard::instance_of<my_packet_1>(), 2, var2, var3);
  auto packet2 = MdmaPacketDomain::MdmaPacket<std::array<uint16_t, 64>, std::array<uint32_t, 32>>(
      myBoard::instance_of<my_packet_2>(), 3, var4, var5);
  auto packet3 = MdmaPacketDomain::MdmaPacket<std::array<uint8_t, 256>, std::array<uint16_t, 128>>(
      myBoard::instance_of<my_packet_3>(), 4, var6, var7);
  
  SdLogger sd_logger(sd);
  auto p0 = sd_logger.add_packet(&packet0);
  auto p1 = sd_logger.add_packet(&packet1);
  auto p2 = sd_logger.add_packet(&packet2);
  auto p3 = sd_logger.add_packet(&packet3);

  uint32_t counter0 = 0;
  uint32_t counter1 = 0;
  uint32_t counter2 = 0;
  uint32_t counter3 = 0;


  
  uint32_t iterations = 1000000;
  while (iterations--) {
    STLIB::update();
    sd_logger.update();

    if (counter0++ % 100000 == 0) {
      *var0 = (*var0 + 1) % 256;
      *var1 = (*var1 + 2) % 65536;
      sd_logger.log(p0);
    }
    if (counter1++ % 150000 == 0) {
      *var2 = (*var2 + 3);
      for (size_t i = 0; i < var3->size(); i++) {
        (*var3)[i] = i + (*var3)[i];
      }
      sd_logger.log(p1);
    }
    if (counter2++ % 200000 == 0) {
      for (size_t i = 0; i < var4->size(); i++) {
        (*var4)[i] = i * 2;
      }
      for (size_t i = 0; i < var5->size(); i++) {
        (*var5)[i] = i * 3;
      }
      sd_logger.log(p2);
    }
    if (counter3++ % 250000 == 0) {
      for (size_t i = 0; i < var6->size(); i++) {
        (*var6)[i] = i * 4;
      }
      for (size_t i = 0; i < var7->size(); i++) {
        (*var7)[i] = i * 5;
      }
      sd_logger.log(p3);
    }
  }

  [[maybe_unused]] auto current_buffer = sd.get_current_buffer();
  bool read_complete_flag = false;
  for (size_t i = 0; i < (sd_logger.get_current_block() / 8); i++) {
    while (!sd.read_blocks(i, 4, &read_complete_flag));
    while (!read_complete_flag);
    read_complete_flag = false;
    while (!sd.read_blocks(i, 4, &read_complete_flag));
    while (!read_complete_flag);
    read_complete_flag = false;
  }

  while (1) {
    STLIB::update();
    sd_logger.update();
  }
}
#endif

#ifdef TEST_2
constexpr auto my_sd_card = SdDomain::SdCard<1>{
  SdDomain::Peripheral::sdmmc1,
  std::pair{DigitalInputDomain::DigitalInput{ST_LIB::PG4}, GPIO_PinState::GPIO_PIN_RESET},
  std::pair{DigitalInputDomain::DigitalInput{ST_LIB::PG3}, GPIO_PinState::GPIO_PIN_SET},
};

constexpr auto my_var_0 = MPUDomain::Buffer<uint8_t>{};
constexpr auto my_var_1 = MPUDomain::Buffer<uint16_t>{};

constexpr auto my_packet_0 = MdmaPacketDomain::MdmaPacket<uint8_t>::Request{};
constexpr auto my_packet_1 = MdmaPacketDomain::MdmaPacket<uint16_t>::Request{};
int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<my_sd_card,
                                my_var_0, my_var_1,
                                my_packet_0, my_packet_1>;
  myBoard::init();
  
  auto sd = SdDomain::SdCardWrapper<my_sd_card>(myBoard::instance_of<my_sd_card>());
  auto var0 = myBoard::instance_of<my_var_0>().as<my_var_0>();
  auto var1 = myBoard::instance_of<my_var_1>().as<my_var_1>();
  auto packet0 = MdmaPacketDomain::MdmaPacket<uint8_t>(
      myBoard::instance_of<my_packet_0>(), 1, var0);
  auto packet1 = MdmaPacketDomain::MdmaPacket<uint16_t>(
      myBoard::instance_of<my_packet_1>(), 2, var1);
  
  SdLogger sd_logger(sd);
  auto p0 = sd_logger.add_packet(&packet0);
  auto p1 = sd_logger.add_packet(&packet1);

  *var0 = 0xFF;
  *var1 = 0xABAB;

  uint32_t iterations = 10000;
  while (iterations--) {
    STLIB::update();
    sd_logger.update();

    sd_logger.log(p0);

    sd_logger.log(p1);
  }

  [[maybe_unused]] auto current_buffer = sd.get_current_buffer();
  bool read_complete_flag = false;
  for (size_t i = 0; i < (sd_logger.get_current_block() / 2); i++) {
    while (!sd.read_blocks(i, 1, &read_complete_flag));
    while (!read_complete_flag);
    read_complete_flag = false;
    while (!sd.read_blocks(i, 1, &read_complete_flag));
    while (!read_complete_flag);
    read_complete_flag = false;
  }

  while (1) {
    STLIB::update();
    sd_logger.update();
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
