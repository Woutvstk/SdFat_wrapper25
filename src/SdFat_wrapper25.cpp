#include "SdFat_wrapper25.h"

#include <Arduino.h>
using namespace std;

ArduinoOutStream cout(Serial);


bool SdFatWrapper25::begin(uint8_t SdCsPin, SPIClass &spi, uint32_t frequency, const char *mountpoint, uint8_t max_files, bool format_if_empty)
{
  cout << "Entered 25 begin\n";

  if (!sd.begin(SdSpiConfig(SdCsPin, SHARED_SPI, frequency, &spi))) {

    Serial.println("SD init fail\n");
    Serial.println("CS pin is:  ");
    Serial.println(SdCsPin);
    sd.initErrorHalt(&Serial);
    return false;
  }
  else
  {
    cout << "SD init succes\n";
    return true;
  }
}

