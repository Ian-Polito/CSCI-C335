/*
 * Filename: main.c
 * Description: Final Project Main Code
 * Author: Ian Polito | ipolito
 * Partner: Colin Curry | colcurry
 * Class: C335 
 * Date: 11/30/18
 */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_user_btn.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
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

FIL Fil;		/* File object */
BYTE Buff2[128];		/* File read buffer */

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
	return (((pixelCol->blue >> 3) << 11) | ((pixelCol->green >> 2) << 5) | (pixelCol->red >> 3));
}

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

int playaudio(char str[]) {  
  	FRESULT rc;                   /* Result code */
  	DIR dir;                      /* Directory object */
  	FILINFO fno;                  /* File information object */
  	UINT bw, br;
  	unsigned int retval;   
  	int bytesread;
  	
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
  	return (1);
}

void drawimage(char img[]) {
	char footer[20];
	int count = 0;
	int i;
	bmppixel pixelCols[128];
	uint16_t convertedCols[128];
	int j;
	int y = 0;
	
	FRESULT rc;			/* Result code */
  	DIR dir;			/* Directory object */
  	FILINFO fno;			/* File information object */
  	UINT bw, br;
  	unsigned int retval;
  	BMPHeader header;
  	BMPInfo info;
  	BMPMagic magic;
	
	rc = f_open(&Fil, "crash.bmp", FA_READ);
	printf("crash.bmp\n");
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
		f3d_lcd_setAddrWindow(y, 0, y, 160, 0x3);
		f3d_lcd_pushColor(convertedCols, 128);
		y++;
	}
	rc = f_close(&Fil);
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
  	printf("loading gyro\n");
  	f3d_gyro_init();
  	delay(10);
  	
  	printf("Reset\n");
  	
  	f_mount(0, &Fatfs);/* Register volume work area */
  	delay(100);
  	char str[30];
  	int selected = 0;
  	nunchuk_t data;
  	uint16_t white[128];
  	uint16_t red[20];
  	uint16_t blue[10];
	uint16_t black[10];
	uint16_t yellow[10];
	int check = 0;
	int col = 0;
  	int i;
  	int score = 0;
  	int down_accel = 0;
	int side_accel = 0;
	float gyro[3] = {};
	int ship_x = 60;
	int ship_y = 30;
	int landing = 0;
  	for (i = 0; i < 128; i++) {
		white[i] = WHITE;
		if (i < 10) {
			blue[i] = BLUE;
			black[i] = BLACK;
			yellow[i] = YELLOW;
		}
		if (i < 20) {
			red[i] = RED;
		}
  	}
	//start game
	while (1) {
		//do starting game stuff
		playaudio("start.wav");
		//reset variables
		down_accel = 0;
		side_accel = 0;
		ship_x = 60;
		ship_y = 20;
		f3d_lcd_fillScreen(BLACK);
		//draw terrain
		int y;
		f3d_lcd_setAddrWindow(0,158,128,158,MADCTLGRAPHICS);
		for (y = 0; y < 1; y++) {
			f3d_lcd_pushColor(white ,128);
		}
		//create randomized landing zone
		landing = rand() % (108 + 1 - 0) + 0;
		//draw landing zone
		f3d_lcd_setAddrWindow(landing,158,landing+20,158,MADCTLGRAPHICS);
		for (y = 0; y < 1; y++) {
			f3d_lcd_pushColor(red, 20);
		}
		//draw ship at the start
		sprintf(str, "A");
		f3d_lcd_drawString(60,20,str,BLUE,BLACK);
		while (1) {
			f3d_gyro_getdata(gyro);
			f3d_nunchuck_read(&data);
			//update display
			sprintf(str, "Score: +%4d", score);
			f3d_lcd_drawString(0,0,str,WHITE,BLACK);
			sprintf(str, "Momentum: +%3d", down_accel);
        		f3d_lcd_drawString(0,10,str,WHITE,BLACK);
			//check if bottom thruster should be used (nunchuck)
			if (data.z == 1 || data.c == 1) {
				//use bottom thrusters
				check = 1;
				down_accel -= 2;
				//draw thruster
				/*
				f3d_lcd_setAddrWindow(ship_x+3,ship_y+16,ship_x+6,ship_y+11,MADCTLGRAPHICS);
                                for (y = 0; y < 6; y++) {
					if (col == 0) {
                                        	f3d_lcd_pushColor(red, 4);
					} else {
						f3d_lcd_pushColor(yellow, 4);
					}
                                }
				*/
			} else {
				//increment downward speed
				down_accel += 1;
			}
			//check gyro data for side thrusters
			if (data.jx < 107 || gyro[1] < -30) {
				//use left side thruster
				check = 1;
				side_accel -= 2;
				//draw thruster
				/*
				f3d_lcd_setAddrWindow(ship_x-3,ship_y+8,ship_x-2,ship_y+3,MADCTLGRAPHICS);
                                for (y = 0; y < 6; y++) {
					if (col == 0) {
                                        	f3d_lcd_pushColor(red, 3);
					} else {
						f3d_lcd_pushColor(yellow, 3);
					}
                                }
				*/
			} else if (data.jx > 147 || gyro[1] > 30) {
				//use right side thruster
				check = 1;
				side_accel += 2;
				//draw thruster
				/*
				f3d_lcd_setAddrWindow(ship_x+12,ship_y+8,ship_x+13,ship_y+3,MADCTLGRAPHICS);
                                for (y = 0; y < 6; y++) {
					if (col == 0) {
                                        	f3d_lcd_pushColor(red, 3);
					} else {
						f3d_lcd_pushColor(yellow, 3);
					}
                                }
				*/
			}/* else {
				//slow down sideways speed towards 0
				if (side_accel > 0) {
					side_accel -= 1;
				} else if (side_accel < 0) {
					side_accel += 1;
				}
			}*/
			if (check == 1) {
				//playaudio("thruster.wav");
				check = 0;
			}
			if (col == 0) {
				col = 1;
			} else {
				col = 0;
			}
			//redraw ship
			//first draw a space over the old coords
			sprintf(str, " ");
			f3d_lcd_drawString(ship_x,ship_y,str,BLUE,BLACK);
			//then erase the thrusters
			/*
			f3d_lcd_setAddrWindow(ship_x+3,ship_y+16,ship_x+6,ship_y+11,MADCTLGRAPHICS);
                        for (y = 0; y < 6; y++) {
                        	f3d_lcd_pushColor(black, 4);
                        }
			f3d_lcd_setAddrWindow(ship_x-3,ship_y+8,ship_x-2,ship_y+3,MADCTLGRAPHICS);
                        for (y = 0; y < 6; y++) {
                                f3d_lcd_pushColor(black, 3);
                        }
			f3d_lcd_setAddrWindow(ship_x+12,ship_y+8,ship_x+13,ship_y+3,MADCTLGRAPHICS);
                        for (y = 0; y < 6; y++) {
                        	f3d_lcd_pushColor(black, 3);
                        }
			*/
			//increment x by side_accel
			ship_x += side_accel;
			//increment y by down_accel
			ship_y += down_accel;
			//fix ship coords if needed
			if (ship_y > 148) {
				ship_y = 148;
			}
			while (ship_x > 128) {
				ship_x = ship_x - 128;
			}
			while (ship_x < 0) {
				ship_x = ship_x + 128;
			}
			//redraw ship
                        sprintf(str, "A");
                        f3d_lcd_drawString(ship_x,ship_y,str,BLUE,BLACK);
			//check if ship has landed
			if ((ship_x < (landing + 20) && ship_x > (landing)) && ship_y == 148) {
				//check speed
				if (down_accel < 6) {
					//we've landed
					sprintf(str,"Congratulations");
					f3d_lcd_drawString(25,30,str,WHITE,BLACK);
					sprintf(str,"The Eagle has landed");
					f3d_lcd_drawString(5,40,str,WHITE,BLACK);
					sprintf(str,"100 Points");
					f3d_lcd_drawString(35,60,str,WHITE,BLACK);
					playaudio("land.wav");
					score += 100;
					break;
				} else {
					//we've crashed
					playaudio("crash.wav");
					//draw crash image
					drawimage("crash.bmp");
					while(1) {
						if (user_btn_read() == 1) {
							break;
						}
					}
					score = 0;
					break;
				}
			} else if ((ship_x > (landing) || ship_x < (landing + 20)) && ship_y == 148) {
				//we've crashed
				playaudio("crash.wav");
				//draw crash image
				drawimage("crash.bmp");
				while(1) {
					if (user_btn_read() == 1) {	
						break;
					}
				}
				score = 0;
				break;
			}
			delay(100);
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
