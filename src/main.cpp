#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <array>

#define TYPING_BUTTON D5
#define BACKSPACE_BUTTON D6
#define SIZE_BUTTON D7

#define BUTTON_DELAY 250

#define WIDTH 128
#define HEIGHT 32

Adafruit_SSD1306 oled(WIDTH, HEIGHT, &Wire, -1);

char text[] = "     ";
unsigned int pos = 0;
unsigned long next_pos_time;
bool typing = false;
uint8_t size = 3;
uint8_t max_size = HEIGHT / 8;

void displayText(const char *c, uint8_t s) {
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.setTextSize(s);

  oled.print(c);

  oled.display();
}

bool is_button_pressed(uint8_t pin) { return digitalRead(pin) == LOW; }

void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(TYPING_BUTTON, INPUT_PULLUP);
  pinMode(BACKSPACE_BUTTON, INPUT_PULLUP);
  pinMode(SIZE_BUTTON, INPUT_PULLUP);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    return;
  }

  oled.clearDisplay();
  oled.display();
}

void loop() {
  displayText(text, size);

  if (typing && next_pos_time <= millis()) {
    typing = false;

    pos = pos < strlen(text) - 1 ? pos + 1 : pos;
  }

  if (is_button_pressed(SIZE_BUTTON)) {
    size = size == max_size ? 1 : size + 1;
    delay(BUTTON_DELAY);
  }

  if (is_button_pressed(BACKSPACE_BUTTON)) {
    if (typing) {
      typing = false;
    } else if (pos > 0 && text[pos] == ' ') {
      pos--;
    }

    text[pos] = ' ';
    delay(BUTTON_DELAY);
  }

  if (is_button_pressed(TYPING_BUTTON)) {
    text[pos]++;

    next_pos_time = millis() + 1500;
    typing = true;

    delay(BUTTON_DELAY);
  }
}