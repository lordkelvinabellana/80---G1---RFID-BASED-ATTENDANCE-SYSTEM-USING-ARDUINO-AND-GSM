#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10 //RX slave select
#define RST_PIN 9
#include <Wire.h> 
#define LED_DENIED_PIN 7
#define BUZZER 8

#define LED_ACCESS_PIN 6
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

byte card_ID[4]; //card UID size 4byte
byte Name1[4]={0xBB,0x6F,0x31,0x25};//  lord





//if you want the arduino to detect the cards only once
int NumbCard[1];//this array content the number of cards. in my case i have just one cards.
int j=0;        



String Name//user name
long Number;//user number
int n ;//The number of card you want to detect (optional)  

void setup() {

  
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

     lcd.init();

  // Print a message to the LCD.

  lcd.backlight();

 

  reset_state();
 
   

  
  Serial.println("CLEARSHEET");                 // clears starting at row 1
  Serial.println("LABEL,Date,Time,Name,Number");// make four columns (Date,Time,[Name:"user name"]line 48 & 52,[Number:"user number"]line 49 & 53)
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
     card_ID[i]=mfrc522.uid.uidByte[i];

       if(card_ID[i]==Name1[i]){
       Name="Lord Kelvin Abellana";//user name
       Number=433178;//user number
       j=0;//first number in the NumbCard array : NumbCard[j]
        lcd.clear();
        lcd.noBlink();
         lcd.setCursor(0,0);
        lcd.print("Welcome");
           lcd.setCursor(0,1);
           lcd.print(Name);
         
      }
      else{
          lcd.clear();
          lcd.noBlink();
         lcd.setCursor(0,0);
        lcd.print("Not Registered");
           lcd.setCursor(0,1);
           lcd.print("Invalid");
          digitalWrite(LED_DENIED_PIN, HIGH);
        digitalWrite(BUZZER, HIGH);
           delay(1000);
           digitalWrite(BUZZER, LOW);
           digitalWrite(LED_DENIED_PIN, LOW);
           reset_state();
           
         // digitalWrite(GreenLed,LOW);
        //  digitalWrite(RedLed,HIGH);
          goto cont;//go directly to line 85
         
          
     }
}
      if(NumbCard[j] == 1){//to check if the card already detect
      //if you want to use LCD
      //Serial.println("Already Exist");
          lcd.noBlink();
       lcd.clear();
        lcd.setCursor(0,0);
       lcd.print("Already");
       lcd.setCursor(0,1);
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
      else{
      NumbCard[j] = 2;//put 1 in the NumbCard array : NumbCard[j]={1,1} to let the arduino know if the card was detecting 
      n++;//(optional)
      Serial.print("DATA,DATE,TIME," + Name);//send the Name to excel
      Serial.print(",");
      Serial.println(Number); //send the Number to excel
      digitalWrite(LED_ACCESS_PIN, HIGH);
      digitalWrite(BUZZER, HIGH);
           delay(100);
           digitalWrite(LED_ACCESS_PIN, LOW);
           digitalWrite(BUZZER, LOW);
      delay(1000);
    //  digitalWrite(Buzzer,LOW);
      Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
      }
      delay(1000);
cont:
delay(2000);
//digitalWrite(GreenLed,LOW);
//digitalWrite(RedLed,LOW);

//if you want to close the Excel when all card had detected and save Excel file in Names Folder. in my case i have just 2 card (optional)
//if(n==2){
    
  //  Serial.println("FORCEEXCELQUIT");
 //   }
}

void reset_state()

{
      lcd.clear();
    lcd.print("Place your Card");
   lcd.setCursor(0,1);
  lcd.print("Here:");
  lcd.blink();
  

    

 //   digitalWrite(GreenLed, LOW);

 //   digitalWrite(RedLed, LOW);

}
void green(){
   digitalWrite(LED_ACCESS_PIN, HIGH);
   delay(100);
   digitalWrite(LED_ACCESS_PIN, LOW);

}
   
