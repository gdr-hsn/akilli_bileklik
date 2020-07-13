#include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#define USE_ARDUINO_INTERRUPTS true // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h> // Includes the PulseSensorPlayground Library.
File myFile;
SoftwareSerial sim(2, 3);
int _timeout;
String _buffer;
String number = "+905385839563"; //-> change with your number
String number2 = "+905549972284";
int myBPM;
// Variables
const int PulseWire = 0; // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13; // The on-board Arduino LED, close to PIN 13.
int Threshold = 550; // Determine which Signal to "count as a beat" and which to ignore.
// Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
// Otherwise leave the default "550" value.

PulseSensorPlayground pulseSensor; // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
void setup() {
  

  
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);

Serial.begin(9600); // For Serial Monitor
 

// Configure the PulseSensor object, by assigning our variables to it.
pulseSensor.analogInput(PulseWire);
pulseSensor.blinkOnPulse(LED13); //auto-magically blink Arduino's LED with heartbeat.
pulseSensor.setThreshold(Threshold);

// Double-check the "pulseSensor" object was created and "began" seeing a signal.
if (pulseSensor.begin()) {
Serial.println("We created a pulseSensor Object !"); //This prints one time at Arduino power-up, or on Arduino reset.

delay(2000); //delay for 7 seconds to make sure the modules get the signal
  _buffer.reserve(50);
  Serial.println("Sistem Started...");
  sim.begin(9600);
  delay(1000);
  Serial.println("Type s to send an SMS, r to receive an SMS, and c to make a call");
}
}

void loop() {

 myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".
// "myBPM" hold this BPM value now.
if (pulseSensor.sawStartOfBeat()) { // Constantly test to see if "a beat happened".
Serial.println("♥ A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
Serial.print("BPM: "); // Print phrase "BPM: "
Serial.println(myBPM); // Print the value inside of myBPM.
if((myBPM<30 || myBPM>130)) {
  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);
  delay(1000);
  if((myBPM<10 || myBPM>150)){
  callNumber();
  delay(10000);
}else{
  SendMessage(myBPM);
  delay(10000);
}
}

digitalWrite(7,LOW);
digitalWrite(8,LOW);

if (Serial.available() > 0)
    switch (Serial.read())
    {
      case 's':
        SendMessage(myBPM);
        break;
      case 'r':
        RecieveMessage();
        break;
      case 'c':
        callNumber();
        break;
    }
  if (sim.available() > 0)
    Serial.write(sim.read());




}
delay(1000); // considered best practice in a simple sketch.

Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    if(myBPM==0) {
      
    }
    else {
    Serial.print("Writing to test.txt...");
    myFile.println(myBPM);
    // close the file:
    myFile.close();
    Serial.println("done.");}
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
