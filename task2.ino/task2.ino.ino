#include <LiquidCrystal.h>
#include "IRremote.h"
#include "Servo.h"

/*-----( Declare GPIO )-----*/
const int ultrasonic_trig = 2; // Trig Pin of Ultrasonic Sensor
const int ultrasonic_echo = 4; // Echo Pin of Ultrasonic Sensor
const int servo = 5;           // Signal Pin of Servo to Arduino Analog Pin 5
const int receiver = 6;        // Signal Pin of IR receiver to Arduino Digital Pin 6
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
const int buzzer = 13;         // Signal Pin of buzzer to Arduino Digital Pin 13

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // Create instance of 'irrecv'
decode_results results;      // Create instance of 'decode_results'
Servo myservo;               // Create instance of 'Servo'

/*-----( Declare variables )-----*/
long duration;
int distance;
bool use_ultrasonic = false;

/*-----( Functions )-----*/
void translateIR() // Takes action based on IR code received
{
  switch(results.value)
  {
  case 0xFFA25D: Serial.println("US"); printModeOnLCD("Ultrasonic"); use_ultrasonic = !(use_ultrasonic);  break;
  case 0xFF6897: Serial.println("0");  printAngleOnLCD("0");   playBuzzer(); spinServo(0);   break;
  case 0xFF30CF: Serial.println("1");  printAngleOnLCD("20");  playBuzzer(); spinServo(20);  break;
  case 0xFF18E7: Serial.println("2");  printAngleOnLCD("40");  playBuzzer(); spinServo(40);  break;
  case 0xFF7A85: Serial.println("3");  printAngleOnLCD("60");  playBuzzer(); spinServo(60);  break;
  case 0xFF10EF: Serial.println("4");  printAngleOnLCD("80");  playBuzzer(); spinServo(80);  break;
  case 0xFF38C7: Serial.println("5");  printAngleOnLCD("100"); playBuzzer(); spinServo(100); break;
  case 0xFF5AA5: Serial.println("6");  printAngleOnLCD("120"); playBuzzer(); spinServo(120); break;
  case 0xFF42BD: Serial.println("7");  printAngleOnLCD("140"); playBuzzer(); spinServo(140); break;
  case 0xFF4AB5: Serial.println("8");  printAngleOnLCD("160"); playBuzzer(); spinServo(160); break;
  case 0xFF52AD: Serial.println("9");  printAngleOnLCD("180"); playBuzzer(); spinServo(180); break;
  case 0xFFFFFFFF: Serial.println(" REPEAT"); break;  
  default: 
    Serial.println(" other button   ");
  }// End Case
  delay(500); // Do not get immediate repeat
} //END translateIR

void measureDistance() // Measures distance on Ultrasonic Sensor
{
  digitalWrite(ultrasonic_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonic_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonic_trig, LOW);
  duration = pulseIn(ultrasonic_echo, HIGH);
  distance = duration * 0.034 / 2;  // equation for distance is (time x speed of sound) / 2
  if ((distance >= 0) && (distance <= 180))
  {
    spinServo(distance);
  }
}

void printModeOnLCD(char x[]) // Prints mode on LCD screen
{
  // Print mode message to the LCD.
  if (use_ultrasonic) { lcd.print(x); lcd.print(" OFF  ");}
  else { lcd.print(x); lcd.print(" ON  ");}
  
  // Set the cursor position back to column 0, line 0
  lcd.setCursor(0, 0);

  // Play buzzer twice for mode change
  playBuzzer();
  delay(50); 
  playBuzzer();
} //END printModeOnLCD

void printAngleOnLCD(char x[]) // Prints servo status on LCD screen
{
  // Print angle message to the LCD.
  lcd.print("Servo Angle: ");
  lcd.print(x); // angle
  lcd.print((char)223); // degree symbol
  lcd.print("  "); // white space

  // Set the cursor position back to column 0, line 0
  lcd.setCursor(0, 0);
} //END printAngleOnLCD

void playBuzzer() // Plays the buzzer noise for 50ms
{
  digitalWrite(buzzer, HIGH);
  delay(50); //wait for 10ms
  digitalWrite(buzzer, LOW);
  delay(1); //wait for 1ms
} //END playBuzzer

void spinServo(int x) // Spin the servo to set degrees
{
  myservo.write(x);
  delay(15);
} //END spinServo

/*-----( Setup )-----*/

void setup() {
  // Initialize the buzzer pin as an output
  pinMode(buzzer, OUTPUT);
  // Initialize the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Start the IR receiver 
  irrecv.enableIRIn();
  // Initialize motor pins as output
  myservo.attach(5);
  // Initialize the ultrasonic sensor
  pinMode(ultrasonic_trig, OUTPUT);
  pinMode(ultrasonic_echo, INPUT);
  // Initialize serial communication at 9600 baudrate
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");
} //END setup

/*-----( Main Loop )-----*/

void loop() {
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }
  if (use_ultrasonic)
  {
    measureDistance();
  }
} //END main loop
