#include "main.h"
#include "ST-LIB.hpp"

#include "ST-LIB_LOW/DigitalInput2.hpp"
#include "ST-LIB_LOW/DigitalOutput2.hpp"

using ST_LIB::DigitalInputDomain;
using ST_LIB::DigitalOutputDomain;
using ST_LIB::SPIDomain;
using ST_LIB::DMA_Domain;

#define TEST_1


#ifdef TEST_0
// Do nothing
int main(void) {
  STLIB::start();

  using myBoard = ST_LIB::Board<>;
  myBoard::init();

  while (1) {
    STLIB::update();
  }
}
#endif

#ifdef TEST_1
// Declare master and slave spi
constexpr auto my_spi_1 = SPIDomain::Device<DMA_Domain::Stream::dma1_stream0, DMA_Domain::Stream::dma1_stream1>(
    SPIDomain::SPIMode::SLAVE, SPIDomain::SPIPeripheral::spi1, 1000000,
    ST_LIB::PB3, ST_LIB::PB4, ST_LIB::PB5, ST_LIB::PA4);

constexpr auto my_spi_3 = SPIDomain::Device<DMA_Domain::Stream::dma2_stream0, DMA_Domain::Stream::dma2_stream1>(
    SPIDomain::SPIMode::MASTER, SPIDomain::SPIPeripheral::spi3, 1000000,
    ST_LIB::PC10, ST_LIB::PC11, ST_LIB::PC12, ST_LIB::PA15);

D1_NC uint32_t spi1_rx_buffer[8];
D1_NC uint32_t spi3_tx_buffer[8];

int main(void) {
  STLIB::start();

  spi3_tx_buffer[0] = 0xAAAAAAAA;
  spi3_tx_buffer[1] = 0xBBBBBBBB;
  spi3_tx_buffer[2] = 0xCCCCCCCC;
  spi3_tx_buffer[3] = 0xDDDDDDDD;
  spi3_tx_buffer[4] = 0xEEEEEEEE;
  spi3_tx_buffer[5] = 0xFFFFFFFF;
  spi3_tx_buffer[6] = 0x12345678;
  spi3_tx_buffer[7] = 0x9ABCDEF0;

  spi1_rx_buffer[0] = 0;
  spi1_rx_buffer[1] = 0;
  spi1_rx_buffer[2] = 0;
  spi1_rx_buffer[3] = 0;
  spi1_rx_buffer[4] = 0;
  spi1_rx_buffer[5] = 0;
  spi1_rx_buffer[6] = 0;
  spi1_rx_buffer[7] = 0;

  using myBoard = ST_LIB::Board<my_spi_1, my_spi_3>;
  myBoard::init();

  [[maybe_unused]] auto &spi_instance_1 = myBoard::instance_of<my_spi_1>();
  [[maybe_unused]] auto spi_wrapper_1 = SPIDomain::SPIWrapper<my_spi_1>(spi_instance_1);

  [[maybe_unused]] auto &spi_instance_3 = myBoard::instance_of<my_spi_3>();
  [[maybe_unused]] auto spi_wrapper_3 = SPIDomain::SPIWrapper<my_spi_3>(spi_instance_3);

  volatile bool spi1_operation_complete = false;
  volatile bool spi3_operation_complete = false;
  if (!spi_wrapper_1.listen(std::span<uint8_t>(reinterpret_cast<uint8_t*>(spi1_rx_buffer), sizeof(spi1_rx_buffer)), &spi1_operation_complete)) {
    ErrorHandler("Failed to start SPI1 listen");
  }
  if (!spi_wrapper_3.send_DMA(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(spi3_tx_buffer), sizeof(spi3_tx_buffer)), &spi3_operation_complete)) {
    ErrorHandler("Failed to start SPI3 send");
  }
  while (!spi1_operation_complete || !spi3_operation_complete);

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
