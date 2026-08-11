#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <YX5300_ESP32.h>
#define RX 0
#define TX 1
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
unsigned long previousMillis;
unsigned long pauseMillis;
int b_next = 10;  //button defined pin
int b_pauseplay = 11;
int b_prev = 12;
int b_vinc = 4;
int b_vdec = 3;
int i;
enum { play_song,
	     pause_song,
	     next_song,
	     prev_song,
	     volume_ctrl };
unsigned char songState;
YX5300_ESP32 mp3;
void setup() {
	mp3 = YX5300_ESP32(Serial2, RX, TX);  //init yx5300 connection
	pinMode(b_next, INPUT_PULLUP);        //needs to be INPUT_PULLUP not just INPUT if not using resistor
	pinMode(b_prev, INPUT_PULLUP);
	pinMode(b_pauseplay, INPUT_PULLUP);
	pinMode(b_vinc, INPUT_PULLUP);
	pinMode(b_vdec, INPUT_PULLUP);
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
	pauseMillis = millis();
	mp3.playFolderInLoop(01);
}

void loop() {
	int next_status = digitalRead(b_next);
	int pause_status = digitalRead(b_pauseplay);
	int prev_status = digitalRead(b_prev);
	int vinc_status = digitalRead(b_vinc);
	int vdec_status = digitalRead(b_vdec);
	unsigned long currentMillis = millis();

	switch (songState) {
		case play_song:
			delay(200);
			if (currentMillis >= previousMillis) {
				if (next_status == LOW) {
					delay(120);
					songState = next_song;
				} else if (pause_status == LOW) {
					songState = pause_song;
				} else if (prev_status == LOW) {
					songState = prev_song;
				} else if (vinc_status == LOW) {
					mp3.incrementVolume();
					Serial.write("up vol");
					Serial.write('\n');
					delay(100);
				} else if (vdec_status == LOW) {
					mp3.decrementVolume();
					Serial.write("down vol");
					Serial.write('\n');
					delay(100);
				}
			}
			break;

		case pause_song:
			mp3.pause();
			delay(120);
			if (currentMillis > previousMillis) {
				if (pause_status == LOW) {
					mp3.resume();
					songState = play_song;
				}
			}
			break;

		case next_song:
			Serial.write("next song case reached");
			Serial.write('\n');
			delay(120);
			mp3.next();
			i++;
			if (i >= 50) {
				i = 1;
			}
			funcdraw();
			previousMillis = millis();
			songState = play_song;
			break;

		case prev_song:
			delay(120);
			mp3.prev();
			i = i - 1;
			if (i <= 0) {
				i = 50;
			}
			funcdraw();
			previousMillis = millis();
			songState = play_song;
			break;
	}
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
