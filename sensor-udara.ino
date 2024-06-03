#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int valorsensorgas;
int LED = 7;
int buzzer = 8; // Define pin for the buzzer

// Tentukan batas bawah dan atas untuk deteksi gas
const int gasThresholdLow = 300;  
const int gasThresholdHigh = 1000; 

void setup() { 
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer pin as output
  lcd.init(); // Inisialisasi LCD
  lcd.backlight(); // Mengaktifkan lampu latar LCD
  Serial.begin(9600);
}

void loop() {
  valorsensorgas = analogRead(A1);
  delay(1000);

  lcd.clear();
  lcd.print("sensor_gas:");
  lcd.print(valorsensorgas, 1); 
  Serial.print(valorsensorgas);
  Serial.println();

  // Cek apakah nilai sensor gas berada dalam rentang tertentu
  if (valorsensorgas >= gasThresholdLow && valorsensorgas <= gasThresholdHigh) {
    digitalWrite(LED, HIGH); // Nyalakan LED
    digitalWrite(buzzer, HIGH); // Nyalakan buzzer
    lcd.setCursor(0, 1); // Move the cursor to the second line
    lcd.print("Gas Detected!"); // Display message for high gas level
  } else {
    digitalWrite(LED, LOW); // Matikan LED
    digitalWrite(buzzer, LOW); // Matikan buzzer
  }
  delay(1000);
}
