// Sketch to upload pictures to Dropbox when motion is detected
#include <Bridge.h>
#include <Process.h>
int flag = 1;
// Picture process
Process picture;

// Filename
String filename;

// Pin
int accessgranted = 13;

// Path
String path = "/mnt/sda1/";

void setup() {
  Serial.begin(9600);
  pinMode(accessgranted,INPUT);
  // For debugging, wait until the serial console is connected
delay(4000);
while(!Serial);
  
  // Bridge
  Bridge.begin();
  Serial.println("YUN ready");
}

void loop(void) 
{
 // Set pin mode
 
 if (digitalRead(accessgranted) == HIGH) 
{
  flag=1;
}

 
 if (flag == 1){   
  if (digitalRead(accessgranted) == LOW) {
    flag=0;
    Serial.println("Access granted take a picture");
    // Generate filename with timestamp
    filename = "";
    picture.runShellCommand("date +%s");
    while(picture.running());

    while (picture.available()>0) {
      char c = picture.read();
      filename += c;
      //Serial.println(c);
    } 
    filename.trim();
    filename += ".jpg";
 
    // Take picture
    picture.runShellCommand("fswebcam " + path + filename + " -S10");
    while(picture.running());
    Serial.print("Took a picture: ");
     Serial.println(path + filename);
     // Set pin mode
     pinMode(accessgranted,OUTPUT);
     digitalWrite(accessgranted,HIGH);  //Tell UNO to open the gate (Servo)
     
    // Upload to Dropbox
   picture.runShellCommand("python " + path + "upload_picture_main.py " + path + filename);
     while(picture.running());
    Serial.println("Uploaded to Dropbox");
  }
 }
}

