/**
  ******************************************************************************
  * @file    Demonstrations/Src/fatfs_storage.c
  * @author  MCD Application Team
  * @brief   This file includes the Storage (FatFs) driver 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BITMAP_BUFFER_SIZE 2000 /* You can adapt this size depending on the amount of RAM available */
#define BITMAP_HEADER_SIZE sizeof(BmpHeader) /* Bitmap specificity */
#define MIN(a,b) (((a)<(b))?(a):(b))
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t aBuffer[BITMAP_HEADER_SIZE + BITMAP_BUFFER_SIZE];
FILINFO MyFileInfo;
DIR MyDirectory;
FIL MyFile;
UINT BytesWritten, BytesRead;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Open a file and display it on lcd
  * @param  DirName: the Directory name to open
  * @param  FileName: the file name to open
  * @param  BufferAddress: A pointer to a buffer to copy the file to
  * @param  FileLen: the File length
  * @retval err: Error status (0=> success, 1=> fail)
  */
uint32_t Storage_OpenReadFile(uint8_t Xpoz, uint16_t Ypoz, const char *BmpName)
{
  uint32_t size = 0;
  FIL bmpfile;
  uint32_t nbline;
  BmpHeader* pbmpheader = (BmpHeader*)aBuffer;
  
  /* Close a bmp file */
  f_open(&bmpfile, BmpName, FA_READ);
  
  /* Read the constant part of the header from the file and store it at the top of aBuffer*/
  f_read(&bmpfile, &aBuffer, BITMAP_HEADER_SIZE, &BytesRead);
  
  /* Get the size of the data stored inside the file */
  size = pbmpheader->fsize - pbmpheader->offset;
  
  /* Start reading at the top of the file */
  f_lseek(&bmpfile, 0);
  
  /* Read the entire header from the file and store it at the top of aBuffer */
  f_read(&bmpfile, &aBuffer, pbmpheader->offset, &BytesRead);
  
  /* Compute the number of entire lines which can be stored inside the buffer */
  nbline = (BITMAP_BUFFER_SIZE - pbmpheader->offset + BITMAP_HEADER_SIZE)/(pbmpheader->w * 2);
  
  /* As long as the entire bitmap file as not been displayed */
  do
  {
    uint32_t nbbytetoread;
    
    /* Get the number of bytes which can be stored inside the buffer */
    nbbytetoread = MIN(size,nbline*pbmpheader->w*2);
  
    /* Adapt the total size of the bitmap, stored inside the header, to this chunk */
    pbmpheader->fsize = pbmpheader->offset + nbbytetoread;
  
    /* Adapt the number of line, stored inside the header, to this chunk */
    pbmpheader->h = nbbytetoread/(pbmpheader->w*2);
    
    /* Start reading at the end of the file */
    f_lseek(&bmpfile, pbmpheader->offset + size - nbbytetoread);
    
    /* Store this chunk (or the entire part if possible) of the file inside a buffer */
    f_read(&bmpfile, aBuffer + pbmpheader->offset, nbbytetoread, &BytesRead);
  
    /* Draw the bitmap */
    BSP_LCD_DrawBitmap(Xpoz, Ypoz, aBuffer);    
      
    /* Update the remaining number of bytes to read */
    size -= nbbytetoread;
    
    /* Change the display position of the next bitmap */
    Ypoz += nbline;
      
  }while (size > 0);
  
  /* Close the bmp file */
  f_close(&bmpfile);
  
  return 0;
}      

/**
  * @brief  Copy file BmpName1 to BmpName2 
  * @param  BmpName1: the source file name
  * @param  BmpName2: the destination file name
  * @retval err: Error status (0=> success, 1=> fail)
  */
uint32_t Storage_CopyFile(const char* BmpName1, const char* BmpName2)
{
  uint32_t index = 0;
  FIL file1, file2;
  
  /* Open an Existent BMP file system */
  f_open(&file1, BmpName1, FA_READ);
  /* Create a new BMP file system */
  f_open(&file2, BmpName2, FA_CREATE_ALWAYS | FA_WRITE);
  
  do 
  {
    f_read(&file1, aBuffer, _MAX_SS, &BytesRead);
    f_write(&file2, aBuffer, _MAX_SS, &BytesWritten);  
    index+= _MAX_SS;
    
  } while(index < file1.fsize);
  
  f_close(&file1);
  f_close(&file2);
  
  return 1;
}

/**
  * @brief  Opens a file and copies its content to a buffer.
  * @param  DirName: the Directory name to open
  * @param  FileName: the file name to open
  * @param  BufferAddress: A pointer to a buffer to copy the file to
  * @param  FileLen: File length
  * @retval err: Error status (0=> success, 1=> fail)
  */
uint32_t Storage_CheckBitmapFile(const char* BmpName, uint32_t *FileLen)
{
  uint32_t err = 0;
 
  if(f_open(&MyFile, BmpName, FA_READ) != FR_OK)
  {
    err = 1;
  }
    
  return err;
}

/**
  * @brief  List up to 25 file on the root directory with extension .BMP
  * @param  DirName: Directory name
  * @param  Files: Buffer to contain read files
  * @retval The number of the found files
  */
uint32_t Storage_GetDirectoryBitmapFiles(const char* DirName, char* Files[])
{
  uint32_t counter = 0, index = 0;
  FRESULT res;

  res = f_opendir(&MyDirectory, DirName);
  
  if(res == FR_OK)
  {
    for (;;)
    {
      res = f_readdir(&MyDirectory, &MyFileInfo);
      if(res != FR_OK || MyFileInfo.fname[0] == 0) 
        break;
      if(MyFileInfo.fname[0] == '.') 
        continue;
      
      if(!(MyFileInfo.fattrib & AM_DIR))
      {
        do
        {
          counter++;
        }
        while (MyFileInfo.fname[counter] != 0x2E);
        
        
        if(index < MAX_BMP_FILES)
        {
          if((MyFileInfo.fname[counter + 1] == 'B') && (MyFileInfo.fname[counter + 2] == 'M') && (MyFileInfo.fname[counter + 3] == 'P'))
          {
            if(sizeof(MyFileInfo.fname) <= (MAX_BMP_FILE_NAME + 2))
            {
              sprintf (Files[index], "%s", MyFileInfo.fname);
              index++;
            }
          }
        }
        counter = 0;
      }
    }
  }
    
  return index;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared
  * @param  BufferLength: buffer's length
  * @retval  0: pBuffer1 identical to pBuffer2
  *          1: pBuffer1 differs from pBuffer2
  */
uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  uint8_t ret = 1;
  
  while (BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      ret = 0;
    }
    
    pBuffer1++;
    pBuffer2++;
  }
  
  return ret;
}
