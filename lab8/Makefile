TEMPLATEROOT = ../Fall2018-Lab

# compilation flags for gdb

CFLAGS  = -O1 -g
ASFLAGS = -g 

# object files

OBJS=  $(STARTUP) 	main.o \
			f3d_delay.o \
			f3d_lcd_sd.o \
			f3d_user_btn.o \
			glcdfont.o \
			f3d_uart.o \
			f3d_accel.o \
			f3d_i2c.o \
			f3d_mag.o \
			f3d_gyro.o \
			f3d_nunchuck.o \
			stm32f30x_i2c.o \
			stm32f30x_tim.o \
			stm32f30x_rcc.o \
			stm32f30x_gpio.o \
			stm32f30x_usart.o \
			stm32f30x_spi.o \
			newlib_stubs.o

# include common make file

include $(TEMPLATEROOT)/c335.mk
