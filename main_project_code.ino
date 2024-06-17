#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Servo.h>
#include <Wire.h>


// Buzzer
const int BUZZER_PIN = 6;

// LEDs
const int GREEN_LED = 4; // digital pin of the green led
const int RED_LED = 5;   // digital pin of the red led

// mfrc522 pins
#define RST_PIN         9
#define SS_PIN          10 

// Instances creation
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display
Servo s1;                           // Create Servo instance
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

void servo_up(void)
{
  for (int i = 0; i <= 130; i = i + 5)
    {
      s1.write(i);
      delay(45);
    }
}

void servo_down(void)
{
  for (int i = 130; i >= 0; i = i - 5)
  {
    s1.write(i);
    delay(45);
  }
}
void setup() {
  // put your setup code here, to run once:

  SPI.begin();          // Initiate SPI bus
  Serial.begin(9600);   // Initiate a serial communication
  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  // LED: Initialize the digital pins as an output
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // LCD Display
  lcd.begin();     // initializing the LCD
  lcd.backlight(); // turn on the back

  // mfrc522
  mfrc522.PCD_Init(); // Init MFRC522
  delay(4);           // optional delay. Some board do need more time after init to be ready
  // display to LCD display
  
  // Servo motor
  s1.attach(3); // attaching the servo to digital pin 3
  s1.write(0);  // servo start position
  
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("ACCESS CONTROL");
  lcd.setCursor(0,1);
  lcd.print("-Scan your CARD-");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  Serial.println("UID tag: ");
  String card_id = "";
  
  // gives us the cards uid
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    card_id.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "0"));
    card_id.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  //Serial.print(card_id.substring(1));
  Serial.println();
  if (card_id.substring(1) == "e307b0ac0aa") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized Access");

    lcd.clear();    
    lcd.setCursor(1,0);
    lcd.print("ACCESS CONTROL");
    lcd.setCursor(0,1);
    lcd.print("-Access Allowed-");

    tone(BUZZER_PIN, 450);
    delay(150);
    noTone(BUZZER_PIN);
    servo_up();
    digitalWrite(GREEN_LED, HIGH);
    delay(5000);
    digitalWrite(GREEN_LED, LOW);

    digitalWrite(RED_LED, HIGH);
    tone(BUZZER_PIN, 450);
    delay(150);
    noTone(BUZZER_PIN);
    digitalWrite(RED_LED, LOW);
    servo_down();
  
    delay(3000);

    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("ACCESS CONTROL");
    lcd.setCursor(0,1);
    lcd.print("-Scan your CARD-");
  }

 else
 {
    Serial.println("Access denied");

    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("ACCESS CONTROL");
    lcd.setCursor(1,1);   
    lcd.print("-Access Denied-");
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER_PIN, 300);
    delay(500);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER_PIN);
    delay(3000);

    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("ACCESS CONTROL");
    lcd.setCursor(0,1);
    lcd.print("-Scan your CARD-");
  }
}
