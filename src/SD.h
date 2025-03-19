#ifndef _SDFAT_WRAPPER25_
#define _SDFAT_WRAPPER25_

#define FILE_READ       "r"
#define FILE_WRITE      "w"
#define FILE_APPEND     "a"

/*
** These settings can be added to the variant file of the board, for others, see SdFatConfig.h
** These are some settings for the SdFat library to reduce flash usage and maintain cluster count
* #define USE_LONG_FILE_NAMES 0 //reduces flash usage, ##ATTENTION## max filename length is 8 (see 8.3 filenames)
* #define SDFAT_FILE_TYPE 1         // only support FAT16/FAT32, not exFAT
* #define CHECK_FLASH_PROGRAMMING 0 // this reduces flash usage but may cause higher power usage when sd card is idle TODO:Check if power usage is higher
* #define MAINTAIN_FREE_CLUSTER_COUNT 1 // maintain free cluster count
*/

#include "SdFat/SdFatConfig.h"
#include "SdFat/SdFat.h"
#include "SdFat/sdios.h"
#include "SPI.h"
#include <Arduino.h>
#include "sd_defines.h"
namespace SdFatWrapper25_ns
{




class SdFatWrapper25;
class SdFatFileWrapper25;

// Filesystem wrapper class for the underlaying 'SdFat' SD filesystem class
class SdFatWrapper25
{

public:
        /// @brief Initialise underlaying sd objects, SPI bus must already be initialised
        /// @param ssPin Chip select
        /// @param spi SPIClass object to use for Sd Card
        /// @param frequency SPI frequency to use
        /// @return True for succes
        bool begin(uint8_t ssPin = SS, SPIClass &spi = SPI, uint32_t frequency = 4000000);

        /// @return Total card size in bytes (from csd)
        uint64_t cardSize();
        /// @return Total amount of available bytes (from cluster count/sectors per cluster)
        uint64_t totalBytes();

        /// @return Used bytes out of total bytes. (By counting free clusters)
        uint64_t usedBytes();

        /// @return Free bytes out of total bytes. (By counting free clusters)
        uint64_t freeBytes();

        /*
         *Maps SdFat library Sd card types to esp32 sd library card types
         *
         * \return 0: No_card/error, 2: SD v1/v2, 3:SDHC
         */
        uint8_t cardType();

        /// @brief Creates an internal SdFat file object
        /// @param path Path to the file
        /// @param mode "w", "r", "a". Write, read and append mode
        /// @param create true to create file if non existent
        /// @return SdFatFileWrapper25 file object
        SdFatFileWrapper25 open(const String &path, const char *mode, const bool create = false);
        /// @brief Prints last SdFat error
        void errorPrint();
        bool exists(const char *path);
        bool mkdir(const char *path);
        bool remove(const char *path);
        bool rmdir(const char *path);
        bool rename(const char *pathFrom, const char *pathTo);

private:
SdFat_ns::SdFat sd;
        csd_t csd;
};

// This is the file wrapper class for the underlaying 'File'
class SdFatFileWrapper25
{
public:
        SdFatFileWrapper25() {}

        /** Write data to an open file.
         *
         * \note Data is moved to the cache but may not be written to the
         * storage device until sync() is called.
         *
         * \param[in] buffer Pointer to the location of the data to be written.
         *
         * \param[in] size Number of bytes to write.
         *
         * \return For success write() returns the number of bytes written, always
         * \a size.
         */
        size_t write(const uint8_t *buf, size_t size);
        /** Read data from a file starting at the current position.
         *
         * \param[out] buf Pointer to the location that will receive the data.
         *
         * \param[in] count Maximum number of bytes to read.
         *
         * \return For success read() returns the number of bytes read.
         * A value less than \a nbyte, including zero, will be returned
         * if end of file is reached.
         * If an error occurs, read() returns -1.
         */
        size_t read(uint8_t *buf, size_t size);

        /// @brief Close a file and force cached data and directory information to be written to the storage device
        /// @return true for success or false for failure.
        bool close();
        size_t print(const char *content);

        /// @return number of bytes available from the current position to EOF or INT_MAX if more than INT_MAX bytes are available
        int available();

        /// @brief Read the next byte from a file.
        /// @return For success return the next byte in the file as an int. If an error occurs or end of file is reached return -1.
        int read();

        operator bool() const {
                return true;
                return file.isOpen();
        }


protected:
        bool init(const String &path, oflag_t oflag);

private:
        SdFat_ns::File file;

        friend class SdFatWrapper25;
};

#endif
}

extern SdFatWrapper25_ns::SdFatWrapper25 SD;