#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo gateServo;

// Sensor pins
const int slotSensors[4] = {2, 3, 4, 6};
const int entryIR = 5;
const int exitIR = 13;
const int trigPin = 8;
const int echoPin = 9;
const int servoPin = 7;
const int buzzer = 11;

int availableSlots = 4;

// Variables for non-blocking LCD update
unsigned long previousMillis = 0;
const long interval = 2000;  // 2 seconds
int displayToggle = 0;

void setup() {
  lcd.init();
  lcd.backlight();

  for (int i = 0; i < 4; i++) {
    pinMode(slotSensors[i], INPUT);
  }

  pinMode(entryIR, INPUT);
  pinMode(exitIR, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);

  gateServo.attach(servoPin);
  gateServo.write(0); // gate initially closed

  lcd.setCursor(0, 0);
  lcd.print(" Car Parking ");
  delay(2000);
  lcd.clear();
}

void checkSlots() {
  availableSlots = 0;
  for (int i = 0; i < 4; i++) {
    if (digitalRead(slotSensors[i]) == HIGH) {
      availableSlots++;  // HIGH = slot is free
    }
  }
}

long getUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void openGate() {
  for (int pos = 0; pos <= 90; pos++) {
    gateServo.write(pos);
    delay(10);
  }
}

void closeGate() {
  for (int pos = 90; pos >= 0; pos--) {
    gateServo.write(pos);
    delay(10);
  }
}

void buzzerAlert() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(buzzer, HIGH);  // Use digitalWrite for active buzzer
    delay(300);
    digitalWrite(buzzer, LOW);
    delay(200);
  }
}

void loop() {
  checkSlots();
  long distance = getUltrasonicDistance();

  // Handle car entry
  if (distance < 15 && digitalRead(entryIR) == LOW) {
    if (availableSlots > 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Gate Opening...");
      openGate();

      while (digitalRead(entryIR) == LOW);  // Wait until car passes
      delay(500);
      closeGate();
    } else {
      // All slots full
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Parking Not");
      lcd.setCursor(0, 1);
      lcd.print("Available!");
      buzzerAlert();  // Alert when full
      delay(1000);
      lcd.clear();
    }
  }

  // Handle car exit
  if (digitalRead(exitIR) == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gate Opening...");
    openGate();

    while (digitalRead(exitIR) == LOW);  // Wait for car to exit
    delay(500);
    closeGate();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Car Exited");
    delay(1000);
    lcd.clear();
  }

  // Update LCD display with slot status
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    lcd.clear();
    lcd.setCursor(0, 0);

    if (displayToggle == 0) {
      lcd.print("S1:");
      lcd.print(digitalRead(slotSensors[0]) == HIGH ? "Free " : "Full ");
      lcd.print("S2:");
      lcd.print(digitalRead(slotSensors[1]) == HIGH ? "Free" : "Full");
      displayToggle = 1;
    } else {
      lcd.print("S3:");
      lcd.print(digitalRead(slotSensors[2]) == HIGH ? "Free " : "Full ");
      lcd.print("S4:");
      lcd.print(digitalRead(slotSensors[3]) == HIGH ? "Free" : "Full");
      displayToggle = 0;
    }
  }
}
