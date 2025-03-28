#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED setup
#define SCREEN_WIDTH 128 // Width of OLED screen in pixels
#define SCREEN_HEIGHT 64 // Height of OLED screen in pixels
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// GPIO Pins For Inter-Integrated Communication (I2C)
#define OLED_SDA 21
#define OLED_SCL 16

// GPIO Pin definitions
#define fireButtonPin 5     
#define medicalButtonPin 4  
#define securityButtonPin 3 
#define stopButtonPin 10  // Stop button pin
#define fireLedPin 13       
#define medicalLedPin 12    
#define securityLedPin 14   
#define buzzerPin 15        

// Struct to store emergency locations 
struct EmergencyLocation {
  String location;  // The variable location of type string
};
// Struct to store patient details
struct Patient {
  String name; // Name of patient as a string
  int id;      // Patinet id as an integer
  String location; // location of patient as a string
};

// Locations in the hospital
EmergencyLocation locations[] = {
  {"OPD"}, {"Theater"}, {"ICU"}, {"Ward"},
  {"Emergency Room"}, {"Radiology"}, {"Pharmacy"}
};

// Patients requiring medical emergency
Patient patients[] = {
  {"Afia Fosu-Gyeabour", 101 ,"OPD"},
  {"Justice Adugdaa", 102 , "Theater"},
  {"Ida Asare", 103 , "ICU"},
  {"Richel Wellington", 104 , "Ward"},
  {"Joyce Yesta Amable", 105 , "Emergency Room"},
  {"Umeh Joy", 106 , "Radiology"},
  {"Steven Donkor", 107 , "Pharmacy"}
};

const int locationCount = sizeof(locations) / sizeof(locations[0]); //Counting the number of locations in the array
const int patientCount = sizeof(patients) / sizeof(patients[0]); //Counting the number of patients in the array

void setup() {
  Serial.begin(115200); // Initializing serial communication
  Serial.println("Emergency Alert Ready!");

// Connfiguration of the pins as input or output
  pinMode(fireButtonPin, INPUT_PULLUP); // Enabling input-pullup resistor
  pinMode(medicalButtonPin, INPUT_PULLUP); // Enabling input-pullup resistor
  pinMode(securityButtonPin, INPUT_PULLUP); // Enabling input-pullup resistor
  pinMode(stopButtonPin, INPUT_PULLUP); // Enabling input pull-up resistor
  pinMode(fireLedPin, OUTPUT);
  pinMode(medicalLedPin, OUTPUT);
  pinMode(securityLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed! Check wiring.");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println("Emergency Alert Ready!");
  display.display();
  delay(2000);
  randomSeed(analogRead(0));
}

void loop() {
  if (digitalRead(fireButtonPin) == 0) {
    int locationIndex = random(0, locationCount);
    activateFireAlert(locations[locationIndex]);
    waitForStopOrTimeout();
  } 
  else if (digitalRead(medicalButtonPin) == 0) {
    int patientIndex = random(0, patientCount);
    activateMedicalAlert(patients[patientIndex]);
    waitForStopOrTimeout();
  } 
  else if (digitalRead(securityButtonPin) == 0) {
    int locationIndex = random(0, locationCount);
    activateSecurityAlert(locations[locationIndex]);
    waitForStopOrTimeout();
  }
}

void activateFireAlert(EmergencyLocation location) {
  digitalWrite(fireLedPin, 1);
  tone(buzzerPin, 2000); // Frequency of the buzzer sound
  Serial.println(" Fire Emergency! ");
  Serial.print("Location: "); Serial.println(location.location);
  displayAlert(" Fire Emergency! ", location.location);
  simulateWhatsAppMessage("Fire Emergency at " + location.location);
}

void activateMedicalAlert(Patient patient) {
  digitalWrite(medicalLedPin, HIGH);
  tone(buzzerPin, 3000);
  Serial.println(" Medical Emergency! ");
  Serial.print("Patient: "); Serial.println(patient.name);
  Serial.print("Room: "); Serial.println(patient.id);
  Serial.print("Location: "); Serial.println(patient.location);
  displayAlert(" Medical Emergency! ", patient.name + " Room: " + String(patient.id) + " Location: " + patient.location);
  simulateWhatsAppMessage("Medical Emergency: " + patient.name + " (Room: " + String(patient.id) + ") at " + patient.location);
}

void activateSecurityAlert(EmergencyLocation location) {
  digitalWrite(securityLedPin, HIGH);
  tone(buzzerPin, 4000);
  Serial.println(" Security Emergency! ");
  Serial.print("Location: "); Serial.println(location.location);
  displayAlert(" Security Alert! ", location.location);
  simulateWhatsAppMessage("Security Alert at " + location.location);
}

void waitForStopOrTimeout() {
  unsigned long startTime = millis();
  while (millis() - startTime < 15000) { // 15-second timeout
    if (digitalRead(stopButtonPin) == LOW) { // Stop button pressed
      deactivateAlert();
      return;
    }
  }
  deactivateAlert();
}

void deactivateAlert() {
  digitalWrite(fireLedPin, LOW);
  digitalWrite(medicalLedPin, LOW);
  digitalWrite(securityLedPin, LOW);
  noTone(buzzerPin);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println("System Normal");
  display.display();
}

void displayAlert(String title, String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 5);
  display.println(title);
  display.setCursor(10, 25);
  display.println(message);
  display.display();
}

// Simulate WhatsApp messaging (for Wokwi)
void simulateWhatsAppMessage(String message) {
  Serial.println("[SIMULATED] WhatsApp Message: " + message);
}