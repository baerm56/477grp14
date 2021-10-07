#ifndef WAVEPLAYER_H_
#define WAVEPLAYER_H

#include "stm32l1xx_hal.h"
#include "fatfs.h"

#define SD_CS_Pin GPIO_PIN_1
#define SD_CS_GPIO_Port GPIOB
#define SD_SPI_HANDLE hspi2

typedef struct
{
  uint32_t   ChunkID;       /* 0 */ // RIFF
  uint32_t   FileSize;      /* 4 */ // Size of file - 8 bytes
  uint32_t   FileFormat;    /* 8 */ // WAVE
  uint32_t   SubChunk1ID;   /* 12 */// fmt
  uint32_t   SubChunk1Size; /* 16*/ // 16
  uint16_t   AudioFormat;   /* 20 */// PCM = 1
  uint16_t   NbrChannels;   /* 22 */// Mono = 1, Stereo = 2
  uint32_t   SampleRate;    /* 24 */// 8000, 11020, etc

  uint32_t   ByteRate;      /* 28 */
  uint16_t   BlockAlign;    /* 32 */
  uint16_t   BitPerSample;  /* 34 */// 8, 16
  uint32_t   SubChunk2ID;   /* 36 */// data
  uint32_t   SubChunk2Size; /* 40 */

}WAV_Header;

void WaveplayerInit(SPI_HandleTypeDef * hspi);
int GetFile(const char* filePath);
void readFile(uint8_t * buffer);
void closeFile();

#endif /* WAVEPLAYER_H */