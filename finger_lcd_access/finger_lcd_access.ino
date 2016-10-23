#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <LiquidCrystal.h>

uint8_t getFingerprintEnroll(int id);
int getFingerprintIDez();

// pin #7 is IN from sensor (GREEN wire) pin #10 for YUN
// pin #8 is OUT from arduino  (WHITE wire) pin #11 for YUN
SoftwareSerial mySerial(7,8); //fingerprint
LiquidCrystal lcd(9,6, 5, 4, 3, 2); //lcd pinout
Servo myservo;
int pos = 0;
int ledPin = 11;
const int buttonPin = 12;     // the number of the button pin      // the number of the LED pin
const int buttonPin2 = 13;     // the number of the button pin

// variables will change:
int buttonState = 0;         // variable for reading thebutton status 
unsigned long previousMillis = 0;
const long interval = 1000;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
   while (!Serial) ;
  myservo.attach(10);     // pin #10 digital pin for servo
Serial.begin(9600);
   //Serial.println("fingertest");
     finger.begin(57600);
 pinMode(11, OUTPUT); //shows granted    
if (finger.verifyPassword()) {
   Serial.println("Found fingerprint sensor!");
  } else {
   Serial.println("Did not find fingerprint sensor :(");
  while (1);
  }
  pinMode(buttonPin, INPUT);   // initialize the pushbutton pin as an input:
  pinMode(buttonPin2, INPUT);
}
void loop() {
  
unsigned long currentMillis = millis();
  buttonState = digitalRead(buttonPin);       // read the state of the button
  //buttonState = digitalRead(buttonPin2);
  
  if (buttonState == HIGH) {  // put your main code here, to run repeatedly:
Serial.println("Enrol Mode");
enrollMode();
  }
  else if (buttonState == LOW) {
 Serial.println("Authentication");
 SearchingMode();
}
else
{
Serial.println("...");  
}
}
void enrollMode()
{
   Serial.println("Type in the ID # you want to save this finger as...");
  int id = 0;
  while (true) {
    while (! Serial.available());
    char c = Serial.read();
    if (! isdigit(c)) break;
    id *= 10;
    id += c - '0';
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll(id) );
//}

uint8_t getFingerprintEnroll(int id) {
  int p = -1;
  Serial.println("Waiting for valid finger to enroll");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }
  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  
  // OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}
}
void SearchingMode()
{
  lcd.setCursor(0,0);
 lcd.print("Scan your finger");
    getFingerprintID();
    delay(100);
}
uint8_t getFingerprintID()
{ uint8_t p = finger.getImage();
switch (p)
{
case FINGERPRINT_OK:
lcd.clear();
lcd.print("Finger scanned");
delay(1000);
break;
case FINGERPRINT_NOFINGER:
return p;
case FINGERPRINT_PACKETRECIEVEERR:
return p;
case FINGERPRINT_IMAGEFAIL:
return p;
default:
return p; }

// OK successful!
p = finger.image2Tz();
switch (p) {
case FINGERPRINT_OK:
break;
case FINGERPRINT_IMAGEMESS:
return p;
case FINGERPRINT_PACKETRECIEVEERR:
return p;
case FINGERPRINT_FEATUREFAIL:
return p;
case FINGERPRINT_INVALIDIMAGE:
return p;
default:
return p; }

// OK converted!
p = finger.fingerFastSearch();
if (p == FINGERPRINT_OK)
{
  lcd.clear();
lcd.print("Access granted");
delay(1000);
lcd.clear();
lcd.print("Found ID #");
lcd.print(finger.fingerID);
door();
}
else if(p == FINGERPRINT_NOTFOUND)
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Accessed denied");
delay(2000);
return p;
}
else
{ return p; }

return finger.fingerID;
}
void door()
{
   pos = 0; pos <= 180;  //Gate opening and closing
     myservo.write(pos);  //180 opens gate while 0 closes the gate 
     delay(50);
     digitalWrite(ledPin,1);
     delay(5000);
     pos = 180; pos<=0;
     myservo.write(pos);
    delay(50);
   digitalWrite(ledPin,0);   
}

