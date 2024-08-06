// MAX7219 communication pins
#define SERIAL_DATA_PIN 11
#define SERIAL_CLOCK_PIN 13 // #TODO: needs to be changed since it's the interal LED
#define SERIAL_LOAD_PIN 10

// Buzzer output
#define BUZZER_PIN 3

// Light sensor input
// must be analog
#define LIGHT_SENSOR_PIN A0

// RGB LED output
// must use PWM pins
#define RGB_R_PIN 5
#define RGB_G_PIN 6
#define RGB_B_PIN 9

// Leave floating
// must be analog
#define RANDOM_PIN A5

// Play button
#define PLAY_BUTTON_PIN 2

// Grandbot Controller buttons
#define LEFT_BUTTON_PIN 8
#define UP_BUTTON_PIN A2
#define RIGHT_BUTTON_PIN A3
#define DOWN_BUTTON_PIN A4
#define FORWARD_BUTTON_PIN A6
#define BACKWARD_BUTTON_PIN A7