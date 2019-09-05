/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_delay.h>
#include <f3d_mag.h>
#include <f3d_nunchuck.h>
#include <f3d_rtc.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14152965

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

FATFS Fatfs;		/* File system object */
FIL Fil;		/* File object */
BYTE Buff[128];		/* File read buffer */

uint16_t magic;

typedef struct {
	unsigned char magic[2];
} BMPMagic;

typedef struct {
    uint32_t filesz;
    uint16_t creator1;
    uint16_t creator2;
    uint32_t bmp_offset;
} BMPHeader;

typedef struct {
	uint32_t header_sz;
	int32_t  width;
    	int32_t  height;
	uint16_t nplanes;
	uint16_t bitspp;
	uint32_t compress_type;
    	uint32_t bmp_bytesz;
	int32_t  hres;
    	int32_t  vres;
    	uint32_t ncolors;
    	uint32_t nimpcolors;
} BMPInfo;

typedef struct {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
} bmppixel;

uint16_t convertColors(bmppixel * pixelCol) {
	return (((pixelCol->red >> 3) << 11) | ((pixelCol->green >> 2) << 5) | (pixelCol->blue >> 3));
}

int main(void) { 
  char footer[20];
  int count=0;
  int i;
  
  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;
  BMPHeader header;
  BMPInfo info;
  BMPMagic magic;
  
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  printf("loading uart\n");
  f3d_uart_init();
  printf("loading i2c1\n");
  f3d_i2c1_init();
  delay(10);
  printf("loading accel\n");
  f3d_accel_init(); 
  delay(10);
  printf("loading delay\n");
  f3d_delay_init();
  delay(10);
  printf("loading rtc\n");
  f3d_rtc_init();
  delay(10);
  printf("loading nunchuck\n");
  f3d_nunchuck_init();
  delay(10);
  printf("loading lcd-sd\n");
  f3d_lcd_init();
  delay(10);
  
  f_mount(0, &Fatfs);		/* Register volume work area (never fails) */

  printf("\nOpen an existing file (message.txt).\n");
    rc = f_open(&Fil, "MESSAGE.TXT", FA_READ);
  if (rc) die(rc);
 
  printf("\nType the file content.\n");
  for (;;) {
    rc = f_read(&Fil, Buff, sizeof Buff, &br);	/* Read a chunk of file */
    if (rc || !br) break;			/* Error or end of file */
    for (i = 0; i < br; i++)		        /* Type the data */
      putchar(Buff[i]);
  }
  if (rc) die(rc);
  
  printf("\nClose the file.\n");
  rc = f_close(&Fil);
  if (rc) die(rc);
  
  printf("\nCreate a new file (hello.txt).\n");
  rc = f_open(&Fil, "HELLO.TXT", FA_WRITE | FA_CREATE_ALWAYS);
  if (rc) die(rc);
  
  printf("\nWrite a text data. (Hello world!)\n");
  rc = f_write(&Fil, "Hello world!\r\n", 14, &bw);
  if (rc) die(rc);
  printf("%u bytes written.\n", bw);
  
  printf("\nClose the file.\n");
  rc = f_close(&Fil);
  if (rc) die(rc);
  
  printf("\nOpen root directory.\n");
  rc = f_opendir(&dir, "");
  if (rc) die(rc);
  
  printf("\nDirectory listing...\n");
  for (;;) {
    rc = f_readdir(&dir, &fno);		/* Read a directory item */
    if (rc || !fno.fname[0]) break;	/* Error or end of dir */
    if (fno.fattrib & AM_DIR)
      printf("   <dir>  %s\n", fno.fname);
    else
      printf("%8lu  %s\n", fno.fsize, fno.fname);
  }
  if (rc) die(rc);
  
  printf("\nTest completed.\n");

  rc = disk_ioctl(0,GET_SECTOR_COUNT,&retval);
  printf("%d %d\n",rc,retval);

//PROGRAM START
	
	nunchuk_t test;
	float angle[3] = {};
	float tilt[2] = {};
	float pitch;
	float roll;
	f3d_lcd_fillScreen(RED);
	int image = 0;
	int j = 0;
	int y = 0;
	bmppixel pixelCols[128];
	uint16_t convertedCols[128];
		
 	while (1) {
		f3d_nunchuck_read(&test);
		if (test.c == 1 || test.jx > 147) {
			//go to next image
			image++;
			if (image == 3) {
				image = 0;
			}
		} else if (test.z == 1 || test.jx < 107) {
			//go to prev image
			image--;
			if (image == -1) {
				image = 2;
			}
		}
		//get accel data for image orientation
		f3d_accel_read(angle);
		tilt[0] = atan2 (
                                angle[0],
                                sqrt(pow(angle[1],2) + pow(angle[2],2))
                );
                pitch = (int)(tilt[0] * (180 / PI));
		tilt[1] = atan2 (
                                angle[1],
                                sqrt(pow(angle[0],2) + pow(angle[2],2))
                );
                roll = (int)(tilt[1] * (180 / PI));
		//draw image
		if (image == 0) {
			//draw one.bmp
			rc = f_open(&Fil, "one.bmp", FA_READ);
			printf("one.bmp opened\n");
			rc = f_read(&Fil, &magic, 2, &br);
			printf("Magic %c%c\n", magic.magic[0], magic.magic[1]);
			rc = f_read(&Fil, &header, sizeof(info), &br);
			printf("file size %d offset %d\n", header.filesz, header.bmp_offset);
			rc = f_read(&Fil, &info, sizeof(info),&br);
			f_lseek(&Fil, -(sizeof(info) + sizeof(header) + 2));
			f_lseek(&Fil, header.bmp_offset);
			printf("Width %d Height %d, bitspp %d\n", info.width, info.height, info.bitspp);
			for (i = 0; i < 160; ++i) {
				f_read(&Fil, pixelCols, 3 * 128, &br);
				for (j = 0; j < 128; ++j) {
					convertedCols[j] = convertColors(&pixelCols[j]);
				}
				if (pitch < 30 && roll < 30 && roll > -30) {
					f3d_lcd_setAddrWindow(y, 0, y, 160, 0x3);
					f3d_lcd_pushColor(convertedCols, 128);
				} else if (pitch > 30 && roll < 30 && roll > -30) {
					f3d_lcd_setAddrWindow(y, 0, y, 160, 0x5);
					f3d_lcd_pushColor(convertedCols, 128);
				} else if (roll > -30 && pitch < 30 && pitch > -30) {
					f3d_lcd_setAddrWindow(y, 0, y, 160, 0xA);
					f3d_lcd_pushColor(convertedCols, 128);
				} else if (roll < -30 && pitch < 30 && pitch > -30) {
					f3d_lcd_setAddrWindow(y, 0, y, 160, 0x4);
					f3d_lcd_pushColor(convertedCols, 128);
				}
				y++;
			}
			rc = f_close(&Fil);
		} else if (image == 1) {
			//draw two.bmp
			//draw one.bmp
                        rc = f_open(&Fil, "two.bmp", FA_READ);
                        printf("one.bmp opened\n");
                        rc = f_read(&Fil, &magic, 2, &br);
                        printf("Magic %c%c\n", magic.magic[0], magic.magic[1]);
                        rc = f_read(&Fil, &header, sizeof(info), &br);
                        printf("file size %d offset %d\n", header.filesz, header.bmp_offset);
                        rc = f_read(&Fil, &info, sizeof(info),&br);
                        f_lseek(&Fil, -(sizeof(info) + sizeof(header) + 2));
                        f_lseek(&Fil, header.bmp_offset);
                        printf("Width %d Height %d, bitspp %d\n", info.width, info.height, info.bitspp);
                        for (i = 0; i < 160; ++i) { 
                                f_read(&Fil, pixelCols, 3 * 128, &br);
                                for (j = 0; j < 128; ++j) {
                                        convertedCols[j] = convertColors(&pixelCols[j]);
                                }
                                if (pitch < 30 && roll < 30 && roll > -30) {
                                        f3d_lcd_setAddrWindow(y, 0, y, 160, 0x3);
                                        f3d_lcd_pushColor(convertedCols, 128);
                                } else if (pitch > 30 && roll < 30 && roll > -30) {
                                        f3d_lcd_setAddrWindow(y, 0, y, 160, 0x5);
                                        f3d_lcd_pushColor(convertedCols, 128);
                                } else if (roll > -30 && pitch < 30 && pitch > -30) {
                                        f3d_lcd_setAddrWindow(y, 0, y, 160, 0xA);
                                        f3d_lcd_pushColor(convertedCols, 128);
                                } else if (roll < -30 && pitch < 30 && pitch > -30) {
                                        f3d_lcd_setAddrWindow(y, 0, y, 160, 0x4);
                                        f3d_lcd_pushColor(convertedCols, 128);
                                }
                                y++;
                        }
                        rc = f_close(&Fil);
		} else if (image == 2) {
			//draw three.bmp
			//draw one.bmp
                        rc = f_open(&Fil, "three.bmp", FA_READ);
                        printf("one.bmp opened\n");
                        rc = f_read(&Fil, &magic, 2, &br);
                        printf("Magic %c%c\n", magic.magic[0], magic.magic[1]);
                        rc = f_read(&Fil, &header, sizeof(info), &br);
                        printf("file size %d offset %d\n", header.filesz, header.bmp_offset);
                        rc = f_read(&Fil, &info, sizeof(info),&br);
                        f_lseek(&Fil, -(sizeof(info) + sizeof(header) + 2));
                        f_lseek(&Fil, header.bmp_offset);
                        printf("Width %d Height %d, bitspp %d\n", info.width, info.height, info.bitspp);
                        for (i = 0; i < 160; ++i) { 
                                f_read(&Fil, pixelCols, 3 * 128, &br);
                                for (j = 0; j < 128; ++j) {
                                        convertedCols[j] = convertColors(&pixelCols[j]);
                                }
                                if (pitch < 30 && roll < 30 && roll > -30) {
                                        f3d_lcd_setAddrWindow(y, 0, y, 160, 0x3);
                                        f3d_lcd_pushColor(convertedCols, 128);
                                } else if (pitch > 30 && roll < 30 && roll > -30) {
                                        f3d_lcd_setAddrWindow(y, 0, y, 160, 0x5);
                                        f3d_lcd_pushColor(convertedCols, 128);
                                } else if (roll > -30 && pitch < 30 && pitch > -30) {
                                        f3d_lcd_setAddrWindow(y, 0, y, 160, 0xA);
                                        f3d_lcd_pushColor(convertedCols, 128);
                                } else if (roll < -30 && pitch < 30 && pitch > -30) {
                                        f3d_lcd_setAddrWindow(y, 0, y, 160, 0x4);
                                        f3d_lcd_pushColor(convertedCols, 128);
                                }
                                y++;
                        }
                        rc = f_close(&Fil);
		}
	}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
