
/**next steps: set up & test mp3 module. design display. add pause/play and back buttons**/
/**for the display:
<now playing< (scrolling)
song title (changes)
back - pause/play - forward*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
int b_0 = 12;  //button defined pin
int i;
//may need some defs here for sm

enum { song0,
	     song1,
	     song2 };
unsigned char songState;

// static const unsigned char PROGMEM my_bitmap[] = {
// };


void setup() {
	pinMode(b_0, INPUT_PULLUP);  //needs to be INPUT_PULLUP not just INPUT if not using resistor

	Serial.begin(9600);

	if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
		Serial.println(F("SSD1306 allocation failed"));
		for (;;)
			;
	}
	display.display();
	delay(2000);  // Pause for 2 seconds
	display.clearDisplay();
	//testdrawbitmap();  // Draw a small bitmap image
}

void loop() {

	int b_val = digitalRead(b_0);

	switch (songState) {

		case song0:
			delay(120);
			i = 1;
			funcdraw();
			if (b_val == LOW) {
				Serial.print("1");
				Serial.print('\n');
	
				songState = song1;
				break;
			}
			if (b_val == HIGH) {
				Serial.print("wait");
				Serial.print('\n');
				songState = song0;
				break;
			}

		case song1:
			delay(120);
			//print 2 on the screen
			i = 2;
			funcdraw();
			if (b_val == LOW) {
				Serial.print("2");
				Serial.print('\n');
	
				songState = song2;
				break;
			}
			if (b_val == HIGH) {
				Serial.print("wait");
				Serial.print('\n');
				songState = song1;
				break;
			}

		case song2:
			delay(120);
			//print 3 on the screen
			i = 3;
			funcdraw();
			if (b_val == LOW) {
				Serial.print("3");
				Serial.print('\n');
	
				songState = song0;
				break;
			}
			if (b_val == HIGH) {
				Serial.print("wait");
				Serial.print('\n');
				songState = song2;
				break;
			}
	}
	//Serial.println("Exited state machine.");
}

void testscrolltext(void) {
	display.clearDisplay();

	display.setTextSize(2);  // Draw 2X-scale text
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(10, 0);
	display.println(F("DVD"));
	display.display();  // Show initial text
	delay(100);

	// Scroll in various directions, pausing in-between:
	display.startscrollright(0x00, 0x0F);
	delay(2000);
	display.stopscroll();
	delay(1000);
	display.startscrollleft(0x00, 0x0F);
	delay(2000);
	display.stopscroll();
	delay(1000);
	display.startscrolldiagright(0x00, 0x07);
	delay(2000);
	display.startscrolldiagleft(0x00, 0x07);
	delay(2000);
	display.stopscroll();
	delay(1000);
}

// void testdrawbitmap(void) {
//   display.clearDisplay();

//   display.drawBitmap(
//     /*display.width() - LOGO_WIDTH) / 2*/ 0,
//     /*display.height() - LOGO_HEIGHT) / 2*/ 0,
//     my_bitmap, SCREEN_WIDTH, (SCREEN_HEIGHT*2), 1);
//   display.display();
//   delay(1000);
// }

// void testdrawbitmap2(void) {
//   display.clearDisplay();
void funcdraw(void) {
	display.clearDisplay();

	display.setTextSize(1);               // Normal 1:1 pixel scale
	display.setTextColor(SSD1306_WHITE);  // Draw white text
	display.setCursor(0, 0);              // Start at top-left corner
	display.println(F("This is song "));
	display.print(i);
	display.display();
}
