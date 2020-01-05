/*
   BuzzzMas Stepper Controller
*/
#include <Adafruit_NeoPixel.h>
#include <BasicStepperDriver.h>

#define LED_PIN 3
#define LED_COUNT 3
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//#####################################################
// User defined variables

// Speed the motor spins
int rpm = 2;

// Color of the LEDs during game play
uint32_t nonTrippedColor = strip.Color(0, 0, 255);

// Color of the LEDs when the game has been lost (I lost the game)
uint32_t trippedColor = strip.Color(255, 0, 0);

// Duration of the tone that is played when the sensor touches the wire
unsigned long duration = 1000;

// Frequency of the tone when the sensor touches the wire
int frequency = 200;

//#####################################################


// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 32

// Pins for Stepper motor
#define DIR 16
#define STEP 17

// 2-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

// Sensor Parameters
#define InterruptPin 2
boolean isTripped = false;

// Speaker
#define SPEAKER_PIN A6
unsigned long lastTrippedTime = 0;


void setup() {
  stepper.begin(rpm, MICROSTEPS);
  pinMode(InterruptPin, INPUT_PULLUP);    // sets the digital pin 13 as output
  attachInterrupt(digitalPinToInterrupt(InterruptPin), buzzz, LOW);
    strip.begin();
    setAllPixels(nonTrippedColor);
  //stepper.startRotate(1000000);
}

void loop() {
  if (!isTripped) {
    stepper.rotate(360);
  }
}

void buzzz() {
  if (millis() - lastTrippedTime > duration) {
    stepper.stop();
    isTripped = true;
    setAllPixels(trippedColor);
    tone(SPEAKER_PIN, frequency, duration);
    lastTrippedTime = millis();
  }
}

void setAllPixels(uint32_t color) {
    strip.fill(color);
    strip.show();
}

void setPixel(int led, int red, int green, int blue) {
  strip.begin();
  strip.setPixelColor(led, strip.Color(red, green, blue));
  strip.show();
}