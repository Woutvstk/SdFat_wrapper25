
// These macros were added to the variant file during testing

// #define SDFAT_FILE_TYPE 1         // only support FAT16/FAT32, not exFAT
// #define CHECK_FLASH_PROGRAMMING 0 // this reduces flash usage but may cause higher power usage when sd card is idle TODO:Check if power usage is higher
// #define MAINTAIN_FREE_CLUSTER_COUNT 1 // maintain free cluster count

// This was not added, but could be, for more: see sdfatconfig.h
// #define USE_LONG_FILE_NAMES 0		//this reduces flash even more but file/directory names are limited to 8.3 naming (8 characters filename)

#include "SD.h"

#include "variant.h"
#include <Arduino.h>
#include "Adafruit_TinyUSB.h"
#include <string>

#define SDCARD_CS 26

#define SDHandler SPI

#define SPI_SCK 3
#define SPI_MISO 29
#define SPI_MOSI 30

SdFatWrapper25 sd25;
SdFatFileWrapper25 file;

void printSDSize();
void printSDFreeSpace();
void printTestWriteRead();
void checkFileOperations();
void structWriteBenchmark(uint8_t iStructs, uint8_t jBatches);

//------------------------------------------------------------------------------
void setup()
{
  delay(2000);
  Serial.begin(9600);
  while (!Serial)
  {
    Serial.println("Waiting for serial");
    delay(1000);
  }

  SDHandler.begin();
  if (!sd25.begin(SDCARD_CS, SDHandler))
  {
    Serial.println("sd25 Begin failed\n");
  }
  delay(1000);

  printSDSize();

  printSDFreeSpace();

  printTestWriteRead();

  checkFileOperations();

  structWriteBenchmark(50, 3);
}

//------------------------------------------------------------------------------
void loop()
{
  Serial.println("loop...");
  delay(5000);
}

void printSDSize()
{
  Serial.printf("-------------------Checking Sd size from csd--------------------\n");
  uint8_t cardType = sd25.cardType();
  if (cardType == CARD_NONE)
  {
    Serial.printf("No SD_MMC card attached");
    return;
  }
  Serial.printf("SD_MMC Card Type: ");
  if (cardType == CARD_MMC)
  {
    Serial.printf("MMC\n");
  }
  else if (cardType == CARD_SD)
  {
    Serial.printf("SDSC\n");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.printf("SDHC\n");
  }
  else
  {
    Serial.printf("UNKNOWN\n");
  }

  Serial.printf("SD Card Size: %lu MB\n", (uint32_t)(sd25.cardSize() / (1024 * 1024)));
  Serial.printf("Total space: %lu MB\n\n\n", (uint32_t)(sd25.totalBytes() / (1024 * 1024)));
}

void printSDFreeSpace()
{
  Serial.printf("-------------------Counting free clusters--------------------\n");
  delay(100);
  uint32_t startTime;
  startTime = millis();
  Serial.printf("Used space: %lu MB\n", (uint32_t)(sd25.usedBytes() / (1024 * 1024)));
  Serial.printf("Scanning for free clusters took up %d seconds\n", (millis() - startTime) / 1000);
  delay(2000);
  Serial.println("-------------------Second readout--------------------");
  if (MAINTAIN_FREE_CLUSTER_COUNT)
  {
    Serial.println("Free cluster count is maintained");
  }
  else
  {

    Serial.println("Free cluster count NOT maintained");
  }

  delay(100);

  startTime = millis();
  Serial.printf("Used space: %lu MB\n", (uint32_t)(sd25.usedBytes() / (1024 * 1024)));
  Serial.printf("Second scan for free clusters took up %d seconds\n\n\n", (millis() - startTime) / 1000);
}

void printTestWriteRead()
{
  Serial.printf("-------------------Testing Read/write operations--------------------\n");
  file = sd25.open("file1.txt", "w", true);

  struct dataStruct
  {
    int a = 7;
    int b = 9;
  } myData;

  Serial.printf("myData before WRITE operation:  a: %d b: %d \n", myData.a, myData.b);

  // Write the content to the file
  file.write((uint8_t *)&myData, sizeof(myData) / sizeof(uint8_t));

  // Close the file that was in write mode
  file.close();

  // Reopen the file in read mode to verify the contents
  file = sd25.open("file1.txt", "r", false);

  dataStruct readData;
  readData.a = 2;
  readData.b = 2;
  Serial.printf("readData has been set to:  a: %d b: %d \n", readData.a, readData.b);

  file.read((uint8_t *)&readData, sizeof(readData) / sizeof(uint8_t));

  // Print the read content in hexadecimal format
  Serial.printf("readData after READ operation:  a: %d b: %d \n\n\n", readData.a, readData.b);

  file.close();
}

void checkFileOperations()
{
  Serial.printf("-------------------Checking File and folder operations--------------------\n");
  Serial.printf("Creating file \"file2.txt\"\n");
  file = sd25.open("file2.txt", "w", true);
  file.print("file operations test content");
  file.close();

  Serial.printf("Does the file \"file2.txt\" exist?: %d \n", sd25.exists("file2.txt"));
  Serial.printf("Does the file \"file2B.txt\" exist?: %d \n", sd25.exists("file2B.txt"));

  Serial.printf("Now removing file \"file2.txt\" \n");
  sd25.remove("file2.txt");
  Serial.printf("Does the file \"file2.txt\" exist?: %d \n\n", sd25.exists("file2.txt"));

  Serial.printf("Creating dir: fileOpsTestDir with file file3.txt \n");
  sd25.mkdir("/fileOpsTestDir");
  file = sd25.open("/fileOpsTestDir/file3.txt", "w", true);
  file.print("This is the content of the file inside the dir\n");
  file.close();

  // try append mode
  file = sd25.open("/fileOpsTestDir/file3.txt", "a", true);
  file.print("This is the appended content");
  file.close();

  Serial.printf("Does the directory \"fileOpsTestDir\" exist?: %d \n", sd25.exists("/fileOpsTestDir"));

  Serial.printf("Does the file inside exist?: %d \n\n\n", sd25.exists("/fileOpsTestDir/file3.txt"));
}

void structWriteBenchmark(uint8_t iStructs, uint8_t jBatches)
{

  sd25.mkdir("/storeForward/");
  struct PacketHistoryStruct
  { // testing struct of size 252 bytes
    uint32_t time;
    uint32_t to;
    uint32_t from;
    uint32_t id;
    uint8_t channel;
    uint32_t reply_id;
    bool emoji;
    uint8_t payload[223];
    uint_least16_t payload_size;
  } historyStructObject;

  Serial.printf("Size of struct: %d \n", sizeof(PacketHistoryStruct));

  delay(500);

  uint32_t startMillis;
  uint32_t elapsed;
  uint32_t avrg = 0;

  Serial.printf("Running writing packethistroy struct benchmark\n");
  for (int j = 0; j < jBatches; j++)
  {
    avrg = 0;
    Serial.printf("\n \n Waiting 5sec ... \n \n");
    delay(5000);

    for (int i = 0; i < iStructs; i++)
    {
      startMillis = millis();
      //Write a new file every time or append to file. Pick one
      // When creating lots of files in the root directory, write times can go up
      file = sd25.open("/storeForward/" + String(j) + "File" + String(i), "w", true);  //create new file every time
      // When writing to the same file, write times stay constant
      //file = sd25.open("/storeForward/" + "APFile.txt", "a", true); // append all data to the same file
      file.write((uint8_t *)&historyStructObject, sizeof(historyStructObject) / sizeof(uint8_t));
      file.close();
      elapsed = millis() - startMillis;
      avrg += elapsed;
      Serial.printf(" %d ", elapsed);
      delay(750);
    }

    Serial.printf("\n --> Avarage Elapsed millis is: %d \n\n\n", avrg / iStructs);
  }
}