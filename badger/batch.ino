/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "mdrnnn.h"
#include <stdlib.h>
#include "qr.h"
#include <Arduino.h>

struct Button {
    const uint8_t PIN;
    bool pressed;
};
uint32_t lastMillis = 0;

Button button1 = {12, false};

void ARDUINO_ISR_ATTR isr() {
    button1.pressed = true;
}
/* Entry point ----------------------------------------------------------------*/
void id(){
  EPD_2in13_V3_Init();
	EPD_2in13_V3_Clear();
  UBYTE *BlackImage;
	UWORD Imagesize = ((EPD_2in13_V3_WIDTH % 8 == 0)? (EPD_2in13_V3_WIDTH / 8 ): (EPD_2in13_V3_WIDTH / 8 + 1)) * EPD_2in13_V3_HEIGHT;
  if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
		printf("Failed to apply for black memory...\r\n");
		while (1);
	}
   #if 1  // Drawing on the image
	  Paint_NewImage(BlackImage, EPD_2in13_V3_WIDTH, EPD_2in13_V3_HEIGHT, 90, WHITE);  	
	  Debug("Drawing\r\n");
    	Paint_SelectImage(BlackImage);
	    Paint_Clear(WHITE);
	    Paint_DrawBitMap(mdrnnn);
      Paint_DrawString_EN(5, 15, "Jan Byczuk", &Font24, WHITE, BLACK);
      Paint_DrawString_EN(5, 68, "Leader of R&D Team", &Font16, WHITE, BLACK);
      Paint_DrawString_EN(5, 90, "Service Department", &Font16, WHITE, BLACK);
      EPD_2in13_V3_Display(BlackImage);
	    //DEV_Delay_ms(2000);
  #endif
  EPD_2in13_V3_Sleep();
	free(BlackImage);
	BlackImage = NULL;

}void setup()
{
	pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);
  DEV_Module_Init();
  DEV_Delay_ms(2000);
  EPD_2in13_V3_Init();
	EPD_2in13_V3_Clear();
  id();
}



void code(){
  EPD_2in13_V3_Init();
	EPD_2in13_V3_Clear();
  UBYTE *BlackImage;
	UWORD Imagesize = ((EPD_2in13_V3_WIDTH % 8 == 0)? (EPD_2in13_V3_WIDTH / 8 ): (EPD_2in13_V3_WIDTH / 8 + 1)) * EPD_2in13_V3_HEIGHT;
  if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
		printf("Failed to apply for black memory...\r\n");
		while (1);
	}
   #if 1  // Drawing on the image
	  Paint_NewImage(BlackImage, EPD_2in13_V3_WIDTH, EPD_2in13_V3_HEIGHT, 90, WHITE);  	
	  Debug("Drawing\r\n");
    	Paint_SelectImage(BlackImage);
	    Paint_Clear(WHITE);
	    Paint_DrawBitMap(qr);
      EPD_2in13_V3_Display(BlackImage);
  #endif
}

void loop() {
    if (button1.pressed) {
	    code();
	    button1.pressed = false;
      DEV_Delay_ms(20000);
      id();
    }
    if (button1.pressed == false && millis()-lastMillis >1200000){
      id();
      uint32_t lastMillis = 0;
    }
}
