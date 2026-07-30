/**next steps: set up & test mp3 module. design display.**/
/**for the display:
<now playing< (scrolling)
song title (changes)
prev - pause/play - forward*/

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
unsigned long previousMillis;
int b_next = 10;  //button defined pin
int b_prev = 12;
int i;

enum { play_song,
	     next_song,
	     prev_song };
unsigned char songState;


void setup() {
	pinMode(b_next, INPUT_PULLUP);  //needs to be INPUT_PULLUP not just INPUT if not using resistor
	pinMode(b_prev, INPUT_PULLUP);  //needs to be INPUT_PULLUP not just INPUT if not using resistor
	Serial.begin(9600);

	if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
		Serial.println(F("SSD1306 allocation failed"));
		for (;;)
			;
	}
	display.display();
	delay(2000);  // Pause for 2 seconds
	display.clearDisplay();
	previousMillis = millis();
}

void loop() {
	int next_status = digitalRead(b_next);
	int prev_status = digitalRead(b_prev);
	unsigned long currentMillis = millis();

	switch (songState) {

		case play_song:

			if (next_status == LOW) {
				songState = next_song;
			} else if (prev_status == LOW) {
				songState = prev_song;
			}

			else if (currentMillis - previousMillis >= 3600) {	//automatically continue to next song after current song is over
				songState = next_song;
			}
			break;

		case next_song:
			delay(120);
			next();
			i++;
			if(i >= 50){
				i = 1;
			}
			funcdraw();
			previousMillis = millis();
			songState = play_song;
			break;

		case prev_song:
			delay(120);
			prev();
			i = i - 1;
			if(i <= 0){
				i = 50;
			}
			funcdraw();
			previousMillis = millis();
			songState = play_song;
			break;
	}
}

void next() {		//will be defined accurately later for player
	Serial.write("next song");
	Serial.write('\n');
}

void prev() {		//will be defined accurately later for player
	Serial.write("previous song");
	Serial.write('\n');
}

void funcdraw() {
	display.clearDisplay();
	display.setTextSize(1);               // Normal 1:1 pixel scale
	display.setTextColor(SSD1306_WHITE);  // Draw white text
	display.setCursor(0, 0);              // Start at top-left corner
	display.println(F("This is song "));
	display.print(i);
	display.display();
}


void testscrolltext(void) { //will be used for display later
	display.clearDisplay();

	display.setTextSize(2);  // Draw 2X-scale text
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(10, 0);
	display.println(F("Now playing"));
	display.display();  // Show initial text
	delay(100);
	display.startscrollright(0x00, 0x0F);
	delay(2000);
	display.stopscroll();
	delay(1000);
	display.startscrollleft(0x00, 0x0F);
	delay(2000);
	display.stopscroll();
	delay(1000);

}

