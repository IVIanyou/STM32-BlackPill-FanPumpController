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
#define Temp_pin_sense  PA1
#define Temp_pin_Vcc  PA2

int value_ADC_temp;
float R_ADC_temp;
float temp_lookup [71] = {27.743, 26.533, 25.385, 24.296, 23.262, 22.279, 21.346, 20.459, 19.615, 18.811, 18.047, 17.318, 16.624, 15.962, 15.331, 14.729, 14.154, 13.605, 13.081, 12.580, 12.101, 11.643, 11.205, 10.786, 10.384, 10.000, 9.639, 9.291, 8.955, 8.630, 8.318, 8.017, 7.728, 7.450, 7.183, 6.927, 6.682, 6.446, 6.221, 6.004, 5.797, 5.598, 5.407, 5.224, 5.049, 4.881, 4.720, 4.565, 4.416, 4.273, 4.136, 4.004, 3.876, 3.754, 3.636, 3.522, 3.413, 3.307, 3.205, 3.106, 3.010, 2.916, 2.826, 2.739, 2.656, 2.576, 2.499, 2.426, 2.359, 2.286, 2.220};  //R_tempsensor in kOhm, für 0-70°C
float R1 = 9.88; //Widerstand für den Spannungsteiler 9,88kOhm
float temp;
int i;

byte PWM_fan;
byte PWM_pump;

byte PWM_max_fan = 100;
byte PWM_min_fan = 40;
byte temp_max_fan = 50;
byte temp_min_fan = 30;

byte PWM_max_pump = 50;
byte PWM_min_pump = 20;
byte temp_max_pump = 50;
byte temp_min_pump = 30;


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
  _display.setRotation(2); //Screen um 180° drehen
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

  // ADC Setup
  pinMode(Temp_pin_sense, INPUT_ANALOG);
  analogReadResolution(12);
  pinMode(Temp_pin_Vcc, OUTPUT);
  digitalWrite(Temp_pin_Vcc, HIGH); // Pin daneben High => analog wie an 3.3V/Vcc
}

void loop() {
  for (i=1; i<=1000; i++){
    value_ADC_temp = analogRead(Temp_pin_sense);
    R_ADC_temp = R_ADC_temp + R1 * (4095/float(value_ADC_temp) - 1);
    //delay(1);
  }
  R_ADC_temp = R_ADC_temp / 1000;  // 1000*Auslesen als Glättung


  for (i=0; i<70; i++){
    if ((R_ADC_temp <= temp_lookup[i]) && (R_ADC_temp >= temp_lookup[i+1]) ){
      temp = float(i) + (R_ADC_temp - temp_lookup[i]) / (temp_lookup[i+1] - temp_lookup[i]);
      break;
    }
  }

  PWM_fan = ((PWM_max_fan - PWM_min_fan)/(temp_max_fan - temp_min_fan))*(temp - temp_min_fan) + PWM_min_fan; //Berechnung PWM-Signal fan
  PWM_fan = constrain(PWM_fan, PWM_min_fan, PWM_max_fan);
  analogWrite(PWM_fan_pin, PWM_fan * 2.55); //Setzen des PWM-Signals mit Umrechnung Prozent -> 8Bit (100->255)

  PWM_pump = ((PWM_max_pump - PWM_min_pump)/(temp_max_pump - temp_min_pump))*(temp - temp_min_pump) + PWM_min_pump; //Berechnung PWM-Signal pump
  PWM_pump = constrain(PWM_pump, PWM_min_pump, PWM_max_pump);
  analogWrite(PWM_pump_pin, PWM_pump * 2.55); //Setzen des PWM-Signals mit Umrechnung Prozent -> 8Bit (100->255)
  
  _display.clearDisplay();
  _display.setCursor(0, 0);
  _display.print("T: ");
  _display.print(temp, 1);
  _display.println("'C");
  _display.print("Fans: ");
  _display.print(PWM_fan);
  _display.println("%");
  _display.print("Pump: ");
  _display.print(PWM_pump);
  _display.println("%");
  _display.display();
  
}
