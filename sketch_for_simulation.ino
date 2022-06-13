#include <LiquidCrystal.h> //include LCD library (standard library)
#include <Keypad.h> //include keypad library - first you must install library (library link in the video description)

#define redLED 10 //define the LED pins
#define greenLED 11

char* id ="433178"; //create an id
int pozisyon = 0; //keypad position
int lcdCol = 0;

const byte rows = 4; //number of the keypad's rows and columns
const byte cols = 4;

char keyMap [rows] [cols] = { //define the cymbols on the buttons of the keypad

  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins [rows] = {1, 2, 3, 4}; //pins of the keypad
byte colPins [cols] = {5, 6, 7, 8};

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, rows, cols);

LiquidCrystal lcd (A0, A1, A2, A3, A4, A5); // pins of the LCD. (RS, E, D4, D5, D6, D7)

void setup(){

  lcd.begin(16, 2);
  pinMode(redLED, OUTPUT);  //set the LED as an output
  pinMode(greenLED, OUTPUT);
  setLocked (true); //state of the password
  lcd.setCursor(0, 0);
  lcd.print("GROUP 1");
  lcd.setCursor(0, 1);
  lcd.print("SIMULATION");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter ID#");
}

void loop(){
  char whichKey = myKeypad.getKey(); //define which key is pressed with getKey  
  if(whichKey == '*' || whichKey == '#' || whichKey == 'A' ||       //define invalid keys
  whichKey == 'B' || whichKey == 'C' || whichKey == 'D'){
    pozisyon=0;
    setLocked (true);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Invalid Key!");
    delay(1000);
    lcd.clear();
  }
  if(whichKey == id [pozisyon]){
    pozisyon ++;
    num(whichKey);
  }
  if(pozisyon == 6){
    setLocked (false);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome!!!");
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print("GROUP 1");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SENDING SMS...");
    delay(3000);
    lcd.setCursor(0, 1);
    lcd.print("MESSAGE SENT");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SIMULATION END");
    delay(5000);
  }
  delay(100);
}

void setLocked(int locked){
  if(locked){
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    }
    else{
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, HIGH);
    }
  }
void num(char whichKey){
  lcd.setCursor(lcdCol,1);
  lcd.print(whichKey);
  lcdCol++;
}
