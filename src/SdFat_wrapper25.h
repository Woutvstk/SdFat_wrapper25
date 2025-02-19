#ifndef _SDFAT_WRAPPER25_
#define _SDFAT_WRAPPER25_

#define SPI_DRIVER_SELECT 1    // The standard library driver is always used.
#define ENABLE_DEDICATED_SPI 0  // share the spi bus with others

// reduce flash usage
#define USE_LONG_FILE_NAMES 0
#define SDFAT_FILE_TYPE 1         // only support FAT16/FAT32, not exFAT
#define CHECK_FLASH_PROGRAMMING 0 // this reduces flash usage but may cause higher power usage when sd card is idle TODO:Check if power usage is higher

#ifdef ENABLE_DEDICATED_SPI
//cout
#endif



#include "SdFat/SdFatConfig.h"

#include "SdFat/SdFat.h"

#include "SdFat/FreeStack.h"
#include "SdFat/sdios.h"
#include "SPI.h"
#include <Arduino.h>

        class SdFatWrapper25
        {

        public:
                bool begin(uint8_t ssPin = SS, SPIClass &spi = SPI, uint32_t frequency = 4000000,
                         const char *mountpoint = "/sd", uint8_t max_files = 5, bool format_if_empty = false);


        private:

        SdFat sd;
        File file;

        };


//
//
// extern SD
// typedef
// define

#endif