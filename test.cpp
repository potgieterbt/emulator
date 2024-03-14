#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int test();

int main(int argc, char *argv[]) {
  /* std::ifstream input("/home/user/Downloads/cpu_dummy_reads.nes",

                      std::ios::binary);
  std::vector<uint8_t> buffer;
  std::copy(std::istream_iterator<char>(input), std::istream_iterator<char>(),
            std::back_inserter(buffer));
  for (uint8_t i : buffer) {
    printf("%.2X", i);
  } */
  test();
  return 0;
}

int test() {

  enum NameTableMirroring {
    Horizontal = 0,
    Vertical = 1,
    FourScreen = 8,
    OneScreenLower,
    OneScreenHigher,
  };

  bool m_extendedRAM;
  uint8_t m_mapperNumber;
  uint8_t m_nameTableMirroring;
  std::vector<uint8_t> m_PRG_ROM;
  std::vector<uint8_t> m_CHR_ROM;
  std::string path = "/home/user/Downloads/cpu_dummy_reads.nes";
  std::ifstream romFile(path, std::ios_base::binary | std::ios_base::in);
  if (!romFile) {
    std::cout << "Could not open ROM file from path: " << path << std::endl;
    return false;
  }

  std::vector<uint8_t> header;
  std::cout << "Reading ROM from path: " << path << std::endl;

  // Header
  header.resize(0x10);
  if (!romFile.read(reinterpret_cast<char *>(&header[0]), 0x10)) {
    std::cout << "Reading iNES header failed." << std::endl;
    return false;
  }
  if (std::string{&header[0], &header[4]} != "NES\x1A") {
    std::cout << "Not a valid iNES image. Magic number: " << std::hex
              << header[0] << " " << header[1] << " " << header[2] << " "
              << int(header[3]) << std::endl
              << "Valid magic number : N E S 1a" << std::endl;
    return false;
  }

  std::cout << "Reading header, it dictates: \n";

  uint8_t banks = header[4];
  std::cout << "16KB PRG-ROM Banks: " << +banks << std::endl;
  if (!banks) {
    std::cout << "ROM has no PRG-ROM banks. Loading ROM failed." << std::endl;
    return false;
  }

  uint8_t vbanks = header[5];
  std::cout << "8KB CHR-ROM Banks: " << +vbanks << std::endl;

  if (header[6] & 0x8) {
    m_nameTableMirroring = NameTableMirroring::FourScreen;
    std::cout << "Name Table Mirroring: "
              << "FourScreen" << std::endl;
  } else {
    m_nameTableMirroring = header[6] & 0x1;
    std::cout << "Name Table Mirroring: "
              << (m_nameTableMirroring == 0 ? "Horizontal" : "Vertical")
              << std::endl;
  }

  m_mapperNumber = ((header[6] >> 4) & 0xf) | (header[7] & 0xf0);
  std::cout << "Mapper #: " << +m_mapperNumber << std::endl;

  m_extendedRAM = header[6] & 0x2;
  std::cout << "Extended (CPU) RAM: " << std::boolalpha << m_extendedRAM
            << std::endl;

  if (header[6] & 0x4) {
    std::cout << "Trainer is not supported." << std::endl;
    return false;
  }

  if ((header[0xA] & 0x3) == 0x2 || (header[0xA] & 0x1)) {
    std::cout << "PAL ROM not supported." << std::endl;
    return false;
  } else
    std::cout << "ROM is NTSC compatible.\n";

  // PRG-ROM 16KB banks
  m_PRG_ROM.resize(0x4000 * banks);
  if (!romFile.read(reinterpret_cast<char *>(&m_PRG_ROM[0]), 0x4000 * banks)) {
    std::cout << "Reading PRG-ROM from image file failed." << std::endl;
    return false;
  }


  for (uint8_t i : m_PRG_ROM) {
    printf("%.2X", i);
  }

  // CHR-ROM 8KB banks
  if (vbanks) {
    m_CHR_ROM.resize(0x2000 * vbanks);
    if (!romFile.read(reinterpret_cast<char *>(&m_CHR_ROM[0]),
                      0x2000 * vbanks)) {
      std::cout << "Reading CHR-ROM from image file failed." << std::endl;
      return false;
    }
  } else
    std::cout << "Cartridge with CHR-RAM." << std::endl;
  return true;
}
