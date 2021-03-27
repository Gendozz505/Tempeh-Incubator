
#include <Wire.h> //  I2C/TWI comunication library
#include "rgb_lcd.h"  //  "Grove" lcd library
#include <dht.h>  //  sensor library using lib from https://www.ardumotive.com/how-to-use-dht-22-sensor-en.html



#define DHTTYPE DHT22 //  defines sensor type - DHT 22 (AM2302)



//  replacements
dht DHT;
rgb_lcd lcd;



//  pins set up
const int DHT22_PIN = 2;  //  set up DHT22
const int Up_buttonPin   = 12;  //  set up the pushbutton (+)
const int Down_buttonPin = 13;  //  set up the pushbutton (-)
const int Bulb_ON = 3;  //  set up the ON led (green)
const int Bulb_OFF = 5; //  set up the OFF led (red)
const int Relay = 4;  //  set up the relay



//  variables
int Temp_limit = 30;  //  counter for the number of button presses
int up_buttonState = 0; //  current state of the up button
int up_lastButtonState = 0; //  previous state of the up button
int down_buttonState = 0; //  current state of the up button
int down_lastButtonState = 0; //  previous state of the up button
int i;  //  loop counter
int state_change_counter = 0; //  num of state change during the procces
float hum;  //  stores humidity value
float temp; //  stores temperature value
bool bPress = false;  //  helps us to detect when the state of buttons has changed
bool state_helper = true; //  helps to determine num of state change during the procces



void setup ()
{
  Serial.begin (9600);
  //  set up I/O
  pinMode (Up_buttonPin , INPUT_PULLUP);
  pinMode (Down_buttonPin , INPUT_PULLUP);
  pinMode (Relay, OUTPUT);
  pinMode (Bulb_OFF, OUTPUT);
  pinMode (Bulb_ON, OUTPUT);
  lcd.begin (16, 2);  //  set up the LCD's number of columns and rows:
  PrintTLimit ();
  delay (2000);
  PrintTandH ();
  digitalWrite (Relay, LOW);
  digitalWrite (Bulb_OFF, LOW);
  digitalWrite (Bulb_ON, LOW);
}



void loop ()
{
  if (temp <= Temp_limit - 0.5)
  {
    while (temp < Temp_limit + 0.5)
    {
      Relay_ON ();
      if (state_helper)   // counts relay state changes
      {
        state_change_counter ++;
        state_helper = false;
      }
      checkUp ();
      checkDown ();
      if (temp >= Temp_limit + 0.5)
        break;
      if (bPress)
        Delay_screen ();
      PrintTandH ();
    }
  }
  else
  {
    Relay_OFF ();
    state_helper = true;
    checkUp ();
    checkDown ();
    if (bPress)
      Delay_screen ();
    PrintTandH ();
  }
}



// checks UP button
void checkUp ()
{
  up_buttonState = digitalRead (Up_buttonPin);
  // compare the buttonState to its previous state
  if (up_buttonState != up_lastButtonState) 
  {
    if (up_buttonState == LOW)  // if the current state is LOW then the button went from on to off:
    {
      bPress = true;
      Temp_limit ++;
      i = 0;
    } 
    delay (50);    // delay to avoid bouncing
  }
  up_lastButtonState = up_buttonState;    // save the current state as the last state, for next time through the loop
}



// checks DOWN button
void checkDown ()
{
  down_buttonState = digitalRead (Down_buttonPin);
  if (down_buttonState != down_lastButtonState)   // compares the buttonState to its previous state
  {
    // if the state has changed, increment the counter
    if (down_buttonState == LOW)  // if the current state is LOW then the button went from on to off:
    {
      bPress = true;
      Temp_limit --;
      i = 0;
    } 
    delay (50);   // delay to avoid bouncing
  }
  down_lastButtonState = down_buttonState;  // save the current state as the last state, for next time through the loop
}



// turns on the relay
void Relay_ON ()
{
  // configuration for Relay and leds
  digitalWrite (Bulb_OFF, LOW);
  digitalWrite (Relay, HIGH);
  analogWrite (Bulb_ON, 10); // reduces power supply to decrease the led's brightness
}



//  turns off the relay
void Relay_OFF ()
{
  // configuration for Relay and leds
  analogWrite (Bulb_OFF, 40); // reduces power supply to decrease the led's brightness
  digitalWrite (Relay, LOW);
  digitalWrite (Bulb_ON, LOW);   
}



//  captures temperature limit screen for couple seconds
void Delay_screen ()
{
  Save_Mode ();
  for (i = 0; i < 10; ++i) // delays the temperature limit print on LCD
  {
    checkUp ();
    checkDown ();
    PrintTLimit ();
  }
  bPress = false;
  PrintTandH ();
}



//  configuration for Relay and leds, saves the system from overheating while pushing buttons
void Save_Mode ()
{
    digitalWrite (Relay, LOW);
    analogWrite (Bulb_OFF, 40);
    digitalWrite (Bulb_ON, LOW);
    lcd.clear ();
}



//  prints tempearure limit screen
void PrintTLimit ()
{
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print ("Limit (T)");
  lcd.setCursor (2,1);
  lcd.print (Temp_limit);
  lcd.setCursor (6,1);
  lcd.print ("O/I=");
  lcd.setCursor (10,1);
  lcd.print (state_change_counter);
  delay (100);  // delay to avoid LCD bouncing
}



//  prints the main screen with temperature and humidity observation
void PrintTandH ()
{
  lcd.clear ();
  int chk = DHT.read22 (DHT22_PIN);
  hum = DHT.humidity;     // read humidity data and store it to variable hum
  temp = DHT.temperature; // read temerature data and store it to variable temp
  lcd.setCursor (0, 0);  // set the cursor to column 0, line 0
  lcd.print ("Humidity(%):"); // print text
  lcd.print (hum); //  print humidity value
  lcd.setCursor (0, 1);  // set the cursor to column 0, line 1
  lcd.print ("Temperature:"); // print text
  lcd.print (temp);  // print temperature value
  delay (100);  // delay to avoid LCD bouncing
}
