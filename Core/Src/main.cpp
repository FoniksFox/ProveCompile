#include "main.h"
#include "ST-LIB.hpp"

#include "ST-LIB_LOW/DigitalInput2.hpp"
#include "ST-LIB_LOW/DigitalOutput2.hpp"

using ST_LIB::DigitalInputDomain;
using ST_LIB::DigitalOutputDomain;

constexpr DigitalOutputDomain::DigitalOutput led1{ST_LIB::PB0};
constexpr DigitalOutputDomain::DigitalOutput led2{ST_LIB::PE1};
constexpr DigitalOutputDomain::DigitalOutput led3{ST_LIB::PB14};




#define TEST_6


#ifdef TEST_0
// No packet, just test that it doesn't break anything
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
// 1 packet with 1 variable
constexpr auto my_packet_req = MdmaPacketDomain::MdmaPacket<uint16_t>::Request{};
constexpr auto to_build_buffer_req = MPUDomain::Buffer<uint8_t[4]>(
    MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D1);
constexpr auto my_value_req = MPUDomain::Buffer<uint16_t>(
    MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D1);

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, my_packet_req, to_build_buffer_req, my_value_req>;
  myBoard::init();

  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  auto my_value = myBoard::instance_of<my_value_req>().as<my_value_req>();
  *my_value = 0xABCD;
  [[maybe_unused]] auto my_packet = MdmaPacketDomain::MdmaPacket<uint16_t>(myBoard::instance_of<my_packet_req>(), 0x01, my_value);

  [[maybe_unused]] auto to_build_buffer =
      myBoard::instance_of<to_build_buffer_req>().as<to_build_buffer_req>();

  bool flag = false;
  my_packet.build(&flag
    , reinterpret_cast<uint8_t*>(to_build_buffer)
  );

  Time::register_low_precision_alarm(100, [&]() {
    green_led.toggle();
    yellow_led.toggle();
    gred_led.toggle();
  });

  while (1) {
    STLIB::update();
    if (flag) {
      flag = false;
    }
  }
}
#endif

#ifdef TEST_2
// Packet with uint8_t, external destination in D2 (larger buffer than packet)
constexpr auto my_packet2_req = MdmaPacketDomain::MdmaPacket<uint8_t>::Request{};
constexpr auto to_build_buffer2_req = MPUDomain::Buffer<uint8_t[8]>(
    MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D2);

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, my_packet2_req, to_build_buffer2_req>;
  myBoard::init();

  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  uint8_t my_value = 0x7E;
  [[maybe_unused]] auto my_packet = MdmaPacketDomain::MdmaPacket<uint8_t>(myBoard::instance_of<my_packet2_req>(), 0x10, &my_value);

  [[maybe_unused]] auto to_build_buffer2 = myBoard::instance_of<to_build_buffer2_req>().as<to_build_buffer2_req>();

  bool flag2 = false;
  my_packet.build(&flag2, reinterpret_cast<uint8_t*>(to_build_buffer2));

  Time::register_low_precision_alarm(100, [&]() {
    green_led.toggle();
    yellow_led.toggle();
    gred_led.toggle();
  });

  while (1) {
    STLIB::update();
    if (flag2) {
      flag2 = false;
    }
  }
}
#endif

#ifdef TEST_3
// Packet with two variables (uint8_t, uint16_t) built into internal buffer
constexpr auto my_packet3_req = MdmaPacketDomain::MdmaPacket<uint8_t, uint16_t>::Request{};
constexpr auto val1_req = MPUDomain::Buffer<uint8_t>(MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D1);
constexpr auto val2_req = MPUDomain::Buffer<uint16_t>(MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D1);

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, my_packet3_req, val1_req, val2_req>;
  myBoard::init();

  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  auto v1 = myBoard::instance_of<val1_req>().as<val1_req>();
  auto v2 = myBoard::instance_of<val2_req>().as<val2_req>();
  *v1 = 0x5A;
  *v2 = 0x1234;

  [[maybe_unused]] auto my_packet = MdmaPacketDomain::MdmaPacket<uint8_t, uint16_t>(myBoard::instance_of<my_packet3_req>(), 0x20, v1, v2);

  // Build into internal buffer
  my_packet.build();

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
// Two packets built sequentially to the same external D1 buffer
constexpr auto p1_req = MdmaPacketDomain::MdmaPacket<uint8_t>::Request{};
constexpr auto p2_req = MdmaPacketDomain::MdmaPacket<uint16_t>::Request{};
constexpr auto shared_buf_req = MPUDomain::Buffer<uint8_t[16]>(MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D1);

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, p1_req, p2_req, shared_buf_req>;
  myBoard::init();

  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  uint8_t v1 = 0xAA;
  uint16_t v2 = 0xBEEF;

  [[maybe_unused]] auto packet1 = MdmaPacketDomain::MdmaPacket<uint8_t>(myBoard::instance_of<p1_req>(), 0x31, &v1);
  [[maybe_unused]] auto packet2 = MdmaPacketDomain::MdmaPacket<uint16_t>(myBoard::instance_of<p2_req>(), 0x32, &v2);

  [[maybe_unused]] auto shared_buf = myBoard::instance_of<shared_buf_req>().as<shared_buf_req>();

  bool f1 = false;
  bool f2 = false;
  packet1.build(&f1, reinterpret_cast<uint8_t*>(shared_buf));
  while (!f1) { STLIB::update(); }
  packet2.build(&f2, reinterpret_cast<uint8_t*>(shared_buf));
  while (!f2) { STLIB::update(); }

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

#ifdef TEST_5
// Parse test 1: build from internal buffer, parse into separate non-cached destination
constexpr auto parse_src_packet_req = MdmaPacketDomain::MdmaPacket<uint16_t>::Request{};
constexpr auto parse_dst_packet_req = MdmaPacketDomain::MdmaPacket<uint16_t>::Request{};
constexpr auto parse_src_val_req = MPUDomain::Buffer<uint16_t>(MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D1);
constexpr auto parse_dst_val_req = MPUDomain::Buffer<uint16_t>(MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D1);

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, parse_src_packet_req, parse_dst_packet_req, parse_src_val_req, parse_dst_val_req>;
  myBoard::init();

  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  auto src_val = myBoard::instance_of<parse_src_val_req>().as<parse_src_val_req>();
  auto dst_val = myBoard::instance_of<parse_dst_val_req>().as<parse_dst_val_req>();
  *src_val = 0x4242;
  *dst_val = 0x0;

  // Packet A: built from src_val into internal buffer
  [[maybe_unused]] auto packetA = MdmaPacketDomain::MdmaPacket<uint16_t>(myBoard::instance_of<parse_src_packet_req>(), 0x41, src_val);
  packetA.build();

  // Packet B: will parse into dst_val from packetA.buffer
  [[maybe_unused]] auto packetB = MdmaPacketDomain::MdmaPacket<uint16_t>(myBoard::instance_of<parse_dst_packet_req>(), 0x42, dst_val);
  // Blocking parse
  packetB.parse(packetA.buffer);

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
// Parse test 2: build into external non-cached buffer, then parse from that external buffer
constexpr auto parse_src_packet2_req = MdmaPacketDomain::MdmaPacket<uint8_t, uint16_t>::Request{};
constexpr auto parse_dst_packet2_req = MdmaPacketDomain::MdmaPacket<uint8_t, uint16_t>::Request{};
constexpr auto parse_src1_req = MPUDomain::Buffer<uint8_t>(MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D1);
constexpr auto parse_src2_req = MPUDomain::Buffer<uint16_t>(MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D1);
constexpr auto parse_dst1_req = MPUDomain::Buffer<uint8_t>(MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D1);
constexpr auto parse_dst2_req = MPUDomain::Buffer<uint16_t>(MPUDomain::MemoryType::NonCached, MPUDomain::MemoryDomain::D1);

int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<led1, led2, led3, parse_src_packet2_req, parse_dst_packet2_req, parse_src1_req, parse_src2_req, parse_dst1_req, parse_dst2_req>;
  myBoard::init();

  auto &green_led = myBoard::instance_of<led1>();
  auto &yellow_led = myBoard::instance_of<led2>();
  auto &gred_led = myBoard::instance_of<led3>();

  auto s1 = myBoard::instance_of<parse_src1_req>().as<parse_src1_req>();
  auto s2 = myBoard::instance_of<parse_src2_req>().as<parse_src2_req>();
  auto d1 = myBoard::instance_of<parse_dst1_req>().as<parse_dst1_req>();
  auto d2 = myBoard::instance_of<parse_dst2_req>().as<parse_dst2_req>();

  *s1 = 0x0A;
  *s2 = 0x55AA;
  *d1 = 0;
  *d2 = 0;

  // Build packet into external buffer using packetA
  [[maybe_unused]] auto packetA = MdmaPacketDomain::MdmaPacket<uint8_t, uint16_t>(myBoard::instance_of<parse_src_packet2_req>(), 0x51, s1, s2);
  // allocate external via MPUManager and build into it
  void* ext = MPUManager::allocate_non_cached_memory(static_cast<uint32_t>(packetA.get_size()));
  bool done = false;
  packetA.build(&done, reinterpret_cast<uint8_t*>(ext));
  while (!done) { STLIB::update(); }

  // Packet B: parse from external buffer into d1,d2
  [[maybe_unused]] auto packetB = MdmaPacketDomain::MdmaPacket<uint8_t, uint16_t>(myBoard::instance_of<parse_dst_packet2_req>(), 0x52, d1, d2);
  packetB.parse(reinterpret_cast<uint8_t*>(ext));

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
