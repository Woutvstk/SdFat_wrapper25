#include "SdFat_wrapper25.h"

#include <Arduino.h>
using namespace std;

ArduinoOutStream cout(Serial);

bool SdFatWrapper25::begin(uint8_t SdCsPin, SPIClass &spi, uint32_t frequency, const char *mountpoint, uint8_t max_files, bool format_if_empty)
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
  return (uint64_t)csd.capacity()*512;
}

uint64_t SdFatWrapper25::totalBytes()
{
  return (uint64_t)sd.sectorsPerCluster()*sd.clusterCount()*512;
}


uint64_t SdFatWrapper25::usedBytes()
{
  return (uint64_t)sd.sectorsPerCluster()*(sd.clusterCount()-sd.freeClusterCount())*512;
}


/*
*Maps SdFat library Sd card types
*
* \return 0: No_card/error, 2: SD v1/v2, 3:SDHC
*/
uint8_t SdFatWrapper25::cardType()
{

uint8_t returnVal = 0;

//returns 0 - SD V1, 1 - SD V2, or 3 - SDHC/SDXC
uint8_t type = sd.card()->type();

if(type == 1 || type == 2)
{
  returnVal = 2;
}
else if(type == 3)
{
  returnVal = 3;
}

return returnVal;

}