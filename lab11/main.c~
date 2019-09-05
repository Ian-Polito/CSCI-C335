/* main.c --- 
 * 
 * Filename: main.c
 * Description: Lab 11 main code
 * Author: Colin Curry | colcurry
 * Partner: Ian Polito |ipolito
 * Class: C335 
 * Date: 11/9/18
 */
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuck.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <f3d_dac.h>
#include <ff.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIMER 20000
#define AUDIOBUFSIZE 128

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;

FATFS Fatfs;		/* File system object */
FIL fid;		/* File object */
BYTE Buff[512];		/* File read buffer */
int ret;

struct ckhd {
  uint32_t ckID;
  uint32_t cksize;
};

struct fmtck {
  uint16_t wFormatTag;      
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

FRESULT rc;                   /* Result code */
DIR dir;                      /* Directory object */
FILINFO fno;                  /* File information object */
UINT bw, br;
unsigned int retval;   
int bytesread;

int playaudio(char str[]) {
  printf("\nOpen %s\n",str);
  rc = f_open(&fid, str, FA_READ);
  printf("%d",rc);
  printf("\n%s opened\n",str);
  
  if (!rc) {
    struct ckhd hd;
    uint32_t  waveid;
    struct fmtck fck;
    
    readckhd(&fid, &hd, 'FFIR');
    
    f_read(&fid, &waveid, sizeof(waveid), &ret);
    if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
      return -1;
    
    readckhd(&fid, &hd, ' tmf');
    
    f_read(&fid, &fck, sizeof(fck), &ret);
    
    // skip over extra info
    
    if (hd.cksize != 16) {
      printf("extra header info %d\n", hd.cksize - 16);
      f_lseek(&fid, hd.cksize - 16);
    }
    
    printf("audio format 0x%x\n", fck.wFormatTag);
    printf("channels %d\n", fck.nChannels);
    printf("sample rate %d\n", fck.nSamplesPerSec);
    printf("data rate %d\n", fck.nAvgBytesPerSec);
    printf("block alignment %d\n", fck.nBlockAlign);
    printf("bits per sample %d\n", fck.wBitsPerSample);
    
    // now skip all non-data chunks !
    
    while(1){
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
	break;
      f_lseek(&fid, hd.cksize);
    }
    
    printf("Samples %d\n", hd.cksize);
    
    // Play it !
    
    //      audioplayerInit(fck.nSamplesPerSec);
    
    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
    while (hd.cksize) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(Audiobuf, AUDIOBUFSIZE/2);
	f_read(&fid, Audiobuf, next, &ret);
	hd.cksize -= ret;
	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
	hd.cksize -= ret;
	audioplayerWhole = 0;
      }
    }
    audioplayerStop();
  }
  
  printf("\nClose the file.\n"); 
  rc = f_close(&fid);
  
  if (rc) die(rc);
  while(1);
}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  printf("loading uart\n");
  f3d_uart_init();
  printf("loading led\n");
  f3d_led_init();
  printf("loading button\n");
  f3d_user_btn_init();
  printf("loading i2c1\n");
  f3d_i2c1_init();
  delay(10);
  printf("loading timer\n");
  f3d_timer2_init();
  delay(10);
  printf("loading dac\n");
  f3d_dac_init();
  delay(10);
  printf("loading delay\n");
  f3d_delay_init();
  delay(10);
  printf("loading rtc\n");
  f3d_rtc_init();
  delay(10);
  printf("loading systick\n");
  f3d_systick_init(100);
  delay(10);
  printf("loading nunchuck\n");
  f3d_nunchuck_init();
  delay(10);
  printf("loading lcd-sd\n");
  f3d_lcd_init();
  delay(10);
  
  printf("Reset\n");
  
  f_mount(0, &Fatfs);/* Register volume work area */
  delay(100);
  f3d_lcd_fillScreen(RED);
  //program to select audio files
  char str[30];
  int selected = 0;
  nunchuk_t data;
  uint16_t colors[10];
  uint16_t red[10];
  int i;
  for (i = 0; i < 10; i++) {
	colors[i] = WHITE;
	red[i] = RED;
  }
	sprintf(str, "thermo.wav");
	f3d_lcd_drawString(0,0,str,WHITE,BLACK);
	sprintf(str, "pacman.wav");
        f3d_lcd_drawString(0,10,str,WHITE,BLACK);
	sprintf(str, "peacock.wav");
        f3d_lcd_drawString(0,20,str,WHITE,BLACK);
	sprintf(str, "boing_x.wav");
        f3d_lcd_drawString(0,30,str,WHITE,BLACK);
	sprintf(str, "500hz.wav");
        f3d_lcd_drawString(0,40,str,WHITE,BLACK);
	sprintf(str, "500hz-16.wav");
        f3d_lcd_drawString(0,50,str,WHITE,BLACK);
	//draw visual selector for nunchuck
  while (1) {
	int y;
	f3d_lcd_setAddrWindow(80,1+(selected * 10),82,3+(selected * 10), MADCTLGRAPHICS);
       	for (y = 0; y < 3; y++) {
        	f3d_lcd_pushColor(colors, 10);
        }
	f3d_nunchuck_read(&data);
	if (data.jy > 147) {
		//move selector up
		f3d_lcd_setAddrWindow(80,1+(selected * 10),82,3+(selected * 10), MADCTLGRAPHICS);
	        for (y = 0; y < 3; y++) {
        	        f3d_lcd_pushColor(red, 10);
        	}
		selected --;
		if (selected < 0) {
			selected = 5;
		}
		f3d_lcd_setAddrWindow(80,1+(selected * 10),82,3+(selected * 10), MADCTLGRAPHICS);
	        for (y = 0; y < 3; y++) {
        	        f3d_lcd_pushColor(colors, 10);
        	}
	} else if (data.jy < 107) {
		//move selector down
		f3d_lcd_setAddrWindow(80,1+(selected * 10),82,3+(selected * 10), MADCTLGRAPHICS);
                for (y = 0; y < 3; y++) {
                        f3d_lcd_pushColor(red, 10);
                }
                selected ++;
                if (selected > 5) {
                        selected = 0;
                }
                f3d_lcd_setAddrWindow(80,1+(selected * 10),82,3+(selected * 10), MADCTLGRAPHICS);
                for (y = 0; y < 3; y++) {
                        f3d_lcd_pushColor(colors, 10);
                }
	}
	delay(100);
	//check if audio is selected
	if (data.z == 1 || data.c == 1) {
		if (selected == 0) {
			//thermo
			playaudio("thermo.wav");
		} else if (selected == 1) {
			//pacman
			playaudio("pacman.wav");
		} else if (selected == 2) {
			//peacock
			playaudio("peacock.wav");
		} else if (selected == 3) {
			//boing_x
			playaudio("boing_x.wav");
		} else if (selected == 4) {
			//500hz
			playaudio("500hz.wav");
		} else if (selected == 5) {
			//500hz-16
			playaudio("500hz-16");
		}
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
