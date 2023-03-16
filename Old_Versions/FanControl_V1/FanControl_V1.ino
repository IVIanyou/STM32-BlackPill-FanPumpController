#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PWM_fan_pin  PA8
#define PWM_pump_pin  PA9
#define PWM_extra_pin  PA10
#define RPM_fan_pin  PB15
unsigned int RPM_fan;
byte counter_RPM_fan;
byte counter_RPM_fan_max = 10;
byte PWM_fan;
byte PWM_pump;
unsigned long start_time_RPM_fan;


void setup() {
  // Serieller Monitor Init
  Serial.begin(9600);
  // Startet OLED Display
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  // Display Initialisierung
  _display.clearDisplay();
  _display.setTextSize(2);
  _display.setTextColor(WHITE, BLACK);
  _display.setCursor(0, 0);
  _display.println("Screen initialized!");
  _display.display();
  delay(500);
  _display.clearDisplay();
  

  //Fan Setup
  pinMode(PWM_fan_pin, OUTPUT_OPEN_DRAIN);
  pinMode(PWM_pump_pin, OUTPUT_OPEN_DRAIN);
  pinMode(PWM_extra_pin, OUTPUT_OPEN_DRAIN);
  analogWriteFrequency(25000);
  analogWrite(PWM_fan_pin, 255);
  analogWrite(PWM_pump_pin, 150); //58,8%
  analogWrite(PWM_extra_pin, 26); //10%

  //pinMode(RPM_fan_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RPM_fan_pin), RPM_fan_calc, HIGH);
  counter_RPM_fan =0;
  start_time_RPM_fan = millis();

}

void loop() {
  
  _display.clearDisplay();
  _display.setCursor(0, 0);
  _display.println("100% PA8");
  _display.println("60% PA9");
  _display.println("10% PA10");
  _display.print("RPM: ");
  _display.print(RPM_fan);
  _display.display();


  
}

void RPM_fan_calc() {
  counter_RPM_fan++;
  if(counter_RPM_fan >= counter_RPM_fan_max){
      RPM_fan = (counter_RPM_fan_max*30000)/(millis() - start_time_RPM_fan); // 60*1000/2 = 30.000  (min->s->ms, 2 Pulse pro Umdrehung)
      counter_RPM_fan =0;
      start_time_RPM_fan = millis();
  }
}
