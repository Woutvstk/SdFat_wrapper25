#ifndef _SDFAT_WRAPPER25_
#define _SDFAT_WRAPPER25_

#define SPI_DRIVER_SELECT 1 // The standard library driver is always used.

// reduce flash usage
#define USE_LONG_FILE_NAMES 0
#define SDFAT_FILE_TYPE 1         // only support FAT16/FAT32, not exFAT
#define CHECK_FLASH_PROGRAMMING 0 // this reduces flash usage but may cause higher power usage when sd card is idle TODO:Check if power usage is higher


#include "SdFat/SdFatConfig.h"

#include "SdFat/SdFat.h"

#include "SdFat/FreeStack.h"
#include "SdFat/sdios.h"
#include "SPI.h"
#include <Arduino.h>

#include "sd_defines.h"

class SdFatWrapper25;
class SdFatFileWrapper25;

// this is the filesystem wrapper class for the underlaying 'SdFat'
class SdFatWrapper25
{


public:
        // initialise underlaying sd objects, SPI bus must already be initialised
        bool begin(uint8_t ssPin = SS, SPIClass &spi = SPI, uint32_t frequency = 4000000,
                   const char *mountpoint = "/sd", uint8_t max_files = 5, bool format_if_empty = false);

        // return total card size in bytes
        uint64_t cardSize();
        uint64_t totalBytes();
        uint64_t usedBytes();
        uint8_t cardType();
        SdFatFileWrapper25 open(const String &path, const char *mode, const bool create);
        void errorPrint();
        bool exists(const char *path);
        bool mkdir(const char *path);
        bool remove(const char *path);
        bool rmdir(const char *path);
        bool rename(const char *pathFrom, const char *pathTo);
        

private:
        SdFat sd;
        csd_t csd;
};



// this is the file wrapper class for the underlaying 'File'

class SdFatFileWrapper25
{
public:
        SdFatFileWrapper25() {}
        size_t write(const uint8_t *buf, size_t size);
        size_t read(uint8_t *buf, size_t size);
        bool close();
        size_t print(const char *content);
        int available();
        int read();

protected:
        bool init(const String &path, oflag_t oflag);

private:
        File file;


friend class SdFatWrapper25;
};

#endif
