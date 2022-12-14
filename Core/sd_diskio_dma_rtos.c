#include "sd_diskio_dma_rtos.h"

#include <ff_gen_drv.h>
#include <assert.h>
#include <semphr.h>


#define SD_TIMEOUT 2 * 1000
#define SD_DEFAULT_BLOCK_SIZE 512
#define DISABLE_SD_INIT


//  when using cachable memory region, it may be needed to maintain the cache
//  validity. Enable the define below to activate a cache maintenance at each
//  read and write operation.
//  Notice: This is applicable only for cortex M7 based platform.
#define ENABLE_SD_DMA_CACHE_MAINTENANCE 1


static volatile DSTATUS Stat = STA_NOINIT;
static SemaphoreHandle_t sd_semaphore;
static SemaphoreHandle_t sd_mutex;

static DSTATUS SD_CheckStatus(BYTE lun);
DSTATUS SD_initialize (BYTE);
DSTATUS SD_status (BYTE);
DRESULT SD_read (BYTE, BYTE*, DWORD, UINT);
DRESULT SD_write (BYTE, const BYTE*, DWORD, UINT);
DRESULT SD_ioctl (BYTE, BYTE, void*);

const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize,
  SD_status,
  SD_read,
  SD_write,
  SD_ioctl,
};

static DSTATUS SD_CheckStatus(BYTE lun)
{
  Stat = STA_NOINIT;
  if(BSP_SD_GetCardState(0) == BSP_ERROR_NONE){
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

DSTATUS SD_initialize(BYTE lun)
{
  Stat = STA_NOINIT;
  /*
   * check that the kernel has been started before continuing
   * as the osMessage API will fail otherwise
   */
#if !defined(DISABLE_SD_INIT)

  if(osKernelGetState() == osKernelRunning) {
    if(BSP_SD_Init(0) == BSP_ERROR_NONE) {
      Stat = SD_CheckStatus(lun);
    }
  }
#else
  Stat = SD_CheckStatus(lun);
#endif

  if (Stat != STA_NOINIT) {
    sd_semaphore = xSemaphoreCreateCounting(10, 0);
    sd_mutex = xSemaphoreCreateMutex();
  } else {
    assert(0);
  }

  return Stat;
}

DSTATUS SD_status(BYTE lun)
{
  return SD_CheckStatus(lun);
}

DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  xSemaphoreTake(sd_mutex, portMAX_DELAY);

  DRESULT res = RES_ERROR;
  if(BSP_SD_ReadBlocks_DMA(0, (uint32_t*)buff, (uint32_t)sector, count) == BSP_ERROR_NONE) {
    xSemaphoreTake(sd_semaphore, SD_TIMEOUT);

    uint32_t timer = xTaskGetTickCount() + SD_TIMEOUT;

    while(timer > xTaskGetTickCount()) {
      if (BSP_SD_GetCardState(0) == SD_TRANSFER_OK) {
        res = RES_OK;

        #if (ENABLE_SD_DMA_CACHE_MAINTENANCE == 1)
        // the SCB_InvalidateDCache_by_Addr() requires a 32-Byte aligned address,
        // adjust the address and the D-Cache size to invalidate accordingly.
        uint32_t alignedAddr = (uint32_t)buff & ~0x1F;
        SCB_InvalidateDCache_by_Addr((uint32_t*)alignedAddr,
          count*BLOCKSIZE + ((uint32_t)buff - alignedAddr));
        #endif
        break;
      }
    }
  }
  xSemaphoreGive(sd_mutex);

  return res;
}

DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  xSemaphoreTake(sd_mutex, portMAX_DELAY);

  DRESULT res = RES_ERROR;
  if(BSP_SD_WriteBlocks_DMA(0, (uint32_t*)buff, (uint32_t)sector, count) == BSP_ERROR_NONE) {
    xSemaphoreTake(sd_semaphore, SD_TIMEOUT);

    uint32_t timer = xTaskGetTickCount() + SD_TIMEOUT;

    while(timer > xTaskGetTickCount()) {
      if (BSP_SD_GetCardState(0) == SD_TRANSFER_OK) {
        res = RES_OK;
        break;
      }
    }
  }
  xSemaphoreGive(sd_mutex);

  return res;
}

DRESULT SD_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  BSP_SD_CardInfo CardInfo;

  if (Stat & STA_NOINIT) return RES_NOTRDY;

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    BSP_SD_GetCardInfo(0, &CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockNbr;
    res = RES_OK;
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    BSP_SD_GetCardInfo(0, &CardInfo);
    *(WORD*)buff = CardInfo.LogBlockSize;
    res = RES_OK;
    break;

  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    BSP_SD_GetCardInfo(0, &CardInfo);
    *(DWORD*)buff = CardInfo.LogBlockSize / SD_DEFAULT_BLOCK_SIZE;
    res = RES_OK;
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}

void BSP_SD_WriteCpltCallback(uint32_t Instance)
{
  BaseType_t higher_priority_task_woken = pdFALSE;

  xSemaphoreGiveFromISR(sd_semaphore, &higher_priority_task_woken);
  portYIELD_FROM_ISR (higher_priority_task_woken);
}

void BSP_SD_ReadCpltCallback(uint32_t Instance)
{
  BaseType_t higher_priority_task_woken = pdFALSE;

  xSemaphoreGiveFromISR(sd_semaphore, &higher_priority_task_woken);
  portYIELD_FROM_ISR (higher_priority_task_woken);
}
