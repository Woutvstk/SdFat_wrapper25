#ifndef _SDFAT_WRAPPER25_
#define _SDFAT_WRAPPER25_

#define SPI_DRIVER_SELECT 1 // The standard library driver is always used.

// reduce flash usage
#define USE_LONG_FILE_NAMES 0
#define SDFAT_FILE_TYPE 1         // only support FAT16/FAT32, not exFAT
#define CHECK_FLASH_PROGRAMMING 0 // this reduces flash usage but may cause higher power usage when sd card is idle TODO:Check if power usage is higher

#ifdef ENABLE_DEDICATED_SPI
// cout
#endif

#include "SdFat/SdFatConfig.h"

#include "SdFat/SdFat.h"

#include "SdFat/FreeStack.h"
#include "SdFat/sdios.h"
#include "SPI.h"
#include <Arduino.h>

#include "sd_defines.h"

class SdFatWrapper25
{

public:
//initialise underlaying sd objects, SPI bus must already be initialised
        bool begin(uint8_t ssPin = SS, SPIClass &spi = SPI, uint32_t frequency = 4000000,
                   const char *mountpoint = "/sd", uint8_t max_files = 5, bool format_if_empty = false);

                   //return total card size in bytes
        uint64_t cardSize();
        uint64_t totalBytes();
        uint64_t usedBytes();
        uint8_t cardType();

private:
        SdFat sd;
        File file;
        csd_t csd;
};



#endif