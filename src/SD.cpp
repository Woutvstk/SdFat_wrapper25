#include "SD.h"
using namespace std;

ArduinoOutStream cout(Serial);

// ########################### SdFat SD card filesystem functions ####################################

bool SdFatWrapper25::begin(uint8_t SdCsPin, SPIClass &spi, uint32_t frequency)
{

  if (!sd.begin(SdSpiConfig(SdCsPin, SHARED_SPI, frequency, &spi)))
  {
    sd.initErrorHalt(&Serial);
    return false;
  }
  else
  {
    cout << "SD init succes\n";

    if (!sd.card()->readCSD(&csd))
    {
      cout << F("readInfo failed\n");
      return false;
    }
    return true;
  }
}

uint64_t SdFatWrapper25::cardSize()
{
  return (uint64_t)csd.capacity() * 512;
}

uint64_t SdFatWrapper25::totalBytes()
{
  return (uint64_t)sd.sectorsPerCluster() * sd.clusterCount() * 512;
}

uint64_t SdFatWrapper25::usedBytes()
{
  return (uint64_t)sd.sectorsPerCluster() * (sd.clusterCount() - sd.freeClusterCount()) * 512;
}

uint64_t SdFatWrapper25::freeBytes()
{
  return (uint64_t)sd.sectorsPerCluster() * sd.freeClusterCount() * 512;
}

/*
 *Maps SdFat library Sd card types to esp32 sd library card types
 *
 * \return 0: No_card/error, 2: SD v1/v2, 3:SDHC
 */
uint8_t SdFatWrapper25::cardType()
{

  uint8_t returnVal = 0;

  // returns 0 - SD V1, 1 - SD V2, or 3 - SDHC/SDXC
  uint8_t type = sd.card()->type();

  if (type == 1 || type == 2)
  {
    returnVal = 2;
  }
  else if (type == 3)
  {
    returnVal = 3;
  }

  return returnVal;
}

SdFatFileWrapper25 SdFatWrapper25::open(const String &path, const char *mode, const bool create)
{
  oflag_t oflag = 0;

  if (strcmp(mode, "r") == 0)
  {
    // Serial.println("File open mode is Read");
    oflag = oflag | O_RDONLY;
  }
  else if (strcmp(mode, "w") == 0)
  {
    // Serial.println("File open mode is Write");
    oflag = oflag | O_WRONLY;
  }
  else if (strcmp(mode, "a") == 0)
  {
    // Serial.println("File open mode is Append");
    oflag = oflag | O_APPEND;
    oflag = oflag | O_WRONLY;
  }
  else
  {
    Serial.println("ERROR file open mode invalid");
  }

  if (create)
  {
    // Serial.println("File wil be created if not non-existend");
    oflag = oflag | O_CREAT;
  }

  SdFatFileWrapper25 file25 = SdFatFileWrapper25();

  if (!file25.init(path, oflag))
  {
    Serial.println("File was NOT opened succesfully!!!");
    errorPrint();
  }
  return file25;
}

bool SdFatWrapper25::exists(const char *path)
{
  return sd.exists(path);
}

bool SdFatWrapper25::mkdir(const char *path)
{
  return sd.mkdir(path);
}

bool SdFatWrapper25::remove(const char *path)
{

  return sd.remove(path);
}

bool SdFatWrapper25::rmdir(const char *path)
{
  return sd.rmdir(path);
}

bool SdFatWrapper25::rename(const char *pathFrom, const char *pathTo)
{
  return sd.rename(pathFrom, pathTo);
}

size_t SdFatFileWrapper25::print(const char *content)
{
  return file.print(content);
}

void SdFatWrapper25::errorPrint()
{
  // if (sd.sdErrorCode()) {
  cout << F("SD errorCode: ") << hex << showbase;
  printSdErrorSymbol(&Serial, sd.sdErrorCode());
  cout << F(" = ") << int(sd.sdErrorCode()) << endl;
  cout << F("SD errorData = ") << int(sd.sdErrorData()) << dec << endl;
  //}
}

// ########################### SdFat file object functions ####################################

bool SdFatFileWrapper25::init(const String &path, oflag_t oflag)
{

  return file.open(path.c_str(), oflag);
}

size_t SdFatFileWrapper25::write(const uint8_t *buf, size_t size)
{
  if (!file.isOpen())
  {
    Serial.println("file is not open!");
  }

  return file.write(buf, size);

}

size_t SdFatFileWrapper25::read(uint8_t *buf, size_t size)
{
  return file.read(buf, size);
}

int SdFatFileWrapper25::read()
{
  return file.read();
}

bool SdFatFileWrapper25::close()
{
  return file.close();
}

int SdFatFileWrapper25::available()
{
  return file.available();
}