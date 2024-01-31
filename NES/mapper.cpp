#include "mapper.h"
#include <cstdint>
#include <vector>

/* Mapper::Mapper(uint8_t type, std::vector<uint8_t> &prgCode, std::vector<uint8_t> &chrROM,
         int mirroring): _prgCode(prgCode){
} */
Mapper::Mapper(uint8_t type){}

const std::vector<uint8_t> &Mapper::getPrg(){
  return Mapper::_prgCode;
}
