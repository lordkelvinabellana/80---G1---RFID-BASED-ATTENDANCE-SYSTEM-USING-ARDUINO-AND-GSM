#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
SoftwareSerial sim(3, 2);

#define SS_PIN 10 //RX slave select
#define RST_PIN 9
#define LED_DENIED_PIN 7
#define BUZZER 8
#define LED_ACCESS_PIN 6

LiquidCrystal_I2C lcd(0x27, 20, 4);
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
byte card_ID[4]; //card UID size 4byte
byte Name1[4] = {0xEB, 0x24, 0x08, 0x25}; //  LORD
byte Name2[4] = {0x8B, 0xD3, 0x01, 0x25}; //  DEANNAH
byte Name3[4] = {0x1B, 0x69, 0x0A, 0x25}; //  ALYZZA
byte Name4[4] = {0xA3, 0xD2, 0xC7, 0x18}; //  BRIAN
//if you want the arduino to detect the cards only once
int NumbCard[4];
int j = 0;
String Name; //user name
long Number;//user number
int n ;//The number of card you want to detect (optional)
String message;
String _buffer;
int _timeout;
void setup() {
  Serial.begin(9600);// Initialize serial communications with the PC
  _buffer.reserve(50);
  sim.begin(9600); // Initialize simmodule
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  lcd.init();
  lcd.backlight();
  lcd.print("RFID Attendance");
  lcd.setCursor(0, 1);
  lcd.print("Group 1");
  delay(2000);
  lcd.clear();
  lcd.print("Members:");
  lcd.setCursor(0, 1);
  lcd.print("Abellana, Lord K");
  delay(1000);
  lcd.clear();
  lcd.print("Amper, Bryan");
  lcd.setCursor(0, 1);
  lcd.print("Bacol, Deannah M");
  delay(1000);
  lcd.clear();
  lcd.print("Carias, Alyzza M");
  delay(1000);
  reset_state();
  Serial.println("CLEARSHEET");// clears starting at row 1
  Serial.println("LABEL,Date,Time,Last Name,Given Name, Stuent ID");// make four columns (Date,Time,[Name:"user name"]line 48 & 52,[Number:"user number"]line 49 & 53)
  pinMode( LED_DENIED_PIN , OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode( LED_ACCESS_PIN , OUTPUT);
}
void loop() {
  //look for new card
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;//got to start of loop if there is no card present
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
  }
  for (byte i = 0; i < 3; i++) {
    card_ID[i] = mfrc522.uid.uidByte[i];
    if (card_ID[i] == Name1[i]) {
      Name = "Abellana, Lord Kelvin"; //user name
      Number = 433178; //user number
      j = 0; //first number in the NumbCard array : NumbCard[j]
      lcd.clear();
      lcd.noBlink();
      lcd.setCursor(0, 0);
      lcd.print("Welcome");
      lcd.setCursor(0, 1);
      lcd.print(Name);
    }
    else if (card_ID[i] == Name2[i]) {
      Name = "Bacol, Deannah May"; //user name
      Number = 694292; //user number
      j = 1; //first number in the NumbCard array : NumbCard[j]
      lcd.clear();
      lcd.noBlink();
      lcd.setCursor(0, 0);
      lcd.print("Welcome");
      lcd.setCursor(0, 1);
      lcd.print(Name);
    }
    else if (card_ID[i] == Name3[i]) {
      Name = "Carias, Alyzza"; //user name
      Number = 828063; //user number
      j = 2; //first number in the NumbCard array : NumbCard[j]
      lcd.clear();
      lcd.noBlink();
      lcd.setCursor(0, 0);
      lcd.print("Welcome");
      lcd.setCursor(0, 1);
      lcd.print(Name);
    }
    else if (card_ID[i] == Name4[i]) {
      Name = "Amper, Bryan"; //user name
      Number = 966289; //user number
      j = 3; //first number in the NumbCard array : NumbCard[j]
      lcd.clear();
      lcd.noBlink();
      lcd.setCursor(0, 0);
      lcd.print("Welcome");
      lcd.setCursor(0, 1);
      lcd.print(Name);
    }
    else {
      lcd.clear();
      lcd.noBlink();
      lcd.setCursor(0, 0);
      lcd.print("Not Registered");
      lcd.setCursor(0, 1);
      lcd.print("Invalid");
      digitalWrite(LED_DENIED_PIN, HIGH);
      digitalWrite(BUZZER, HIGH);
      delay(1500);
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED_DENIED_PIN, LOW);
      reset_state();
      // digitalWrite(GreenLed,LOW);
      //  digitalWrite(RedLed,HIGH);
      goto cont;//go directly to line 85
    }
  }
  if (NumbCard[j] == 1) { //to check if the card already detect
    //if you want to use LCD
    Serial.println("Already Exist");
    lcd.noBlink();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Already");
    lcd.setCursor(0, 1);
    lcd.print(" Logged-in");
    digitalWrite(LED_DENIED_PIN, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(1000);
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED_DENIED_PIN, LOW);
    delay(3000);
    //  digitalWrite(RedLed, HIGH);

    reset_state();

  }
  else {
    NumbCard[j] == 2;//put 1 in the NumbCard array : NumbCard[j]={1,1} to let the arduino know if the card was detecting
    n++;//(optional)
    Serial.print("DATA,DATE,TIME," + Name);//send the Name to excel
    Serial.print(",");
    Serial.println(Number); //send the Number to excel
    message = (Name + "has arrived inside the school premises");
    accepted();
    sendSms(message);
    reset_state();
    //  digitalWrite(Buzzer,LOW);
    Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
  }
  delay(500);
cont:
  delay(500);
  //digitalWrite(GreenLed,LOW);
  //digitalWrite(RedLed,LOW);
  //if you want to close the Excel when all card had detected and save Excel file in Names Folder. in my case i have just 4 card (optional)
  //if(n==4){
  //  Serial.println("FORCEEXCELQUIT");
  //   }
}
void reset_state()
{
  lcd.clear();
  lcd.print("Place your Card");
  lcd.setCursor(0, 1);
  lcd.print("Here:");
  lcd.blink();

}
void accepted() {
  digitalWrite(LED_ACCESS_PIN, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
  delay(100);
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(LED_ACCESS_PIN, LOW);
  digitalWrite(BUZZER, LOW);
  delay(100);
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(LED_ACCESS_PIN, LOW);
  digitalWrite(BUZZER, LOW);
  delay(100);
}
void sendSms(String message) {
  delay(1000);
  lcd.clear();
  lcd.print("Sending SMS...");
  delay(500);
  sim.println("AT");
  delay(500);
  sim.println("AT+CMGF=1");
  lcd.setCursor(0, 1);
  lcd.print("Message Sent");
  delay(500);
  sim.println("AT+CMGS=\"+639286236782\"\r");
  delay(500);
  sim.print(message);
  delay(500);
  sim.write(26);

}
