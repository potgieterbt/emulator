// 2 KiB RAM for CPU 0x0000 - 0x0800
// *  0x0000 - 0x00FF - Zero page
// *  0x0100 - 0x01FF - Stack, push decriments, pop incriments.
// PPU IO Ports 0x2000 - 0x4000
// APU/Controller IO Ports 0x4000 - 0x6000
// 8 KB Cartridge RAM (WRAM) 0x6000 - 0x8000
// 32 KB Cartridge ROM 0x8000 - 0xFFFA
// *  0x6000 - 0x7FFF - Working RAM
// *  0x8000 - 0xFFFF - ROM and mapper register
// NMI/RESET/IRQ vectors 0xFFFA - 0xFFFF
//

