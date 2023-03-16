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
#define Temp_pin  PA1
byte PWM_fan;
byte PWM_pump;
int value_ADC_temp;
float R_ADC_temp;
float temp_lookup [71] = {27.743, 26.533, 25.385, 24.296, 23.262, 22.279, 21.346, 20.459, 19.615, 18.811, 18.047, 17.318, 16.624, 15.962, 15.331, 14.729, 14.154, 13.605, 13.081, 12.580, 12.101, 11.643, 11.205, 10.786, 10.384, 10.000, 9.639, 9.291, 8.955, 8.630, 8.318, 8.017, 7.728, 7.450, 7.183, 6.927, 6.682, 6.446, 6.221, 6.004, 5.797, 5.598, 5.407, 5.224, 5.049, 4.881, 4.720, 4.565, 4.416, 4.273, 4.136, 4.004, 3.876, 3.754, 3.636, 3.522, 3.413, 3.307, 3.205, 3.106, 3.010, 2.916, 2.826, 2.739, 2.656, 2.576, 2.499, 2.426, 2.359, 2.286, 2.220};  //R_tempsensor in kOhm, für 0-70°C
float R_VDiv = 9.88; //Widerstand für den Spannungsteiler 9,88kOhm
int i;

void setup() {
  // Serieller Monitor Init
  //Serial.begin(115200);//(9600);
  // Startet OLED Display
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    //Serial.println(F("SSD1306 allocation failed"));
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
  

  // Fan Setup
  pinMode(PWM_fan_pin, OUTPUT_OPEN_DRAIN);
  pinMode(PWM_pump_pin, OUTPUT_OPEN_DRAIN);
  pinMode(PWM_extra_pin, OUTPUT_OPEN_DRAIN);
  analogWriteFrequency(25000);
  analogWrite(PWM_fan_pin, 150);  //58,8% Start Wert
  analogWrite(PWM_pump_pin, 150); //58,8%
  analogWrite(PWM_extra_pin, 150); //58,8%


}

void loop() {
  
  _display.clearDisplay();
  _display.setCursor(0, 0);
  _display.println("80% PA8");
  _display.println("20% PA9");
  _display.println("80% PA10");
  _display.display();
  analogWrite(PWM_fan_pin, 204);
  analogWrite(PWM_pump_pin, 51);
  analogWrite(PWM_extra_pin, 204);
  delay(5000);

  _display.clearDisplay();
  _display.setCursor(0, 0);
  _display.println("80% PA8");
  _display.println("80% PA9");
  _display.println("20% PA10");
  _display.display();
  analogWrite(PWM_fan_pin, 204);
  analogWrite(PWM_pump_pin, 204);
  analogWrite(PWM_extra_pin, 51);
  delay(5000);
  
  
}
