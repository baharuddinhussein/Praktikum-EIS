#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <ESP8266WiFi.h>
#include <CTBot.h>
#include <DHT.h>

// Wi-Fi credentials
const char* ssid = "Oppo Reno7";
const char* password = "spartax123";

// Telegram Bot credentials
const String botToken = "7372609926:AAHuAI68wRZL_99Ij56CBxxuT-Eu9JU33C4";
const int chatId = 1291828573; // Note: chatId is a number, not a string

CTBot myBot;

// Definisi pin LED
const int ledRendah = D6;  // LED untuk indikator rendah
const int ledSedang = D7;  // LED untuk indikator sedang
const int ledTinggi = D8;  // LED untuk indikator tinggi

// Definisi pin buzzer
const int buzzerPin = D4;  // Pin untuk buzzer

// Inisialisasi objek LCD dengan alamat I2C 0x27 (sesuaikan dengan modul I2C yang digunakan)
LiquidCrystal_PCF8574 lcd(0x27);

// DHT11
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Deklarasi fungsi sendTelegramMessage di awal
void sendTelegramMessage(String status, int mqValue, float temp, float humidity);

void setup() {
  // Inisialisasi I2C dengan pin SDA dan SCL
  Wire.begin(D2, D1);

  // Inisialisasi LCD
  lcd.begin(16, 2);
  
  // Nyalakan lampu latar LCD
  lcd.setBacklight(255);
  
  // Bersihkan layar LCD
  lcd.clear();
  
  // Tampilkan pesan awal
  lcd.setCursor(0, 0);
  lcd.print("Welcome");
  lcd.setCursor(0, 1);
  lcd.print("To Our Project");

  // Tambahkan delay untuk menampilkan pesan selamat datang
  delay(3000); // Menampilkan pesan selama 3 detik
  
  // Bersihkan layar LCD setelah menampilkan pesan selamat datang
  lcd.clear();
  
  // Inisialisasi pin LED dan buzzer sebagai output
  pinMode(ledRendah, OUTPUT);
  pinMode(ledSedang, OUTPUT);
  pinMode(ledTinggi, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Inisialisasi DHT11
  dht.begin();

  // Connect to Wi-Fi
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - IP address: ");
  Serial.println(WiFi.localIP());

  // Tampilkan pesan WiFi connected di LCD
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP().toString());
  
  // Tambahkan delay untuk menampilkan pesan WiFi connected
  delay(3000); // Menampilkan pesan selama 3 detik

  // Bersihkan layar LCD setelah menampilkan pesan WiFi connected
  lcd.clear();
  
  // Initialize Telegram Bot
  myBot.wifiConnect(ssid, password);
  myBot.setTelegramToken(botToken);
  
  if (myBot.testConnection()) {
    Serial.println("Telegram bot connected");
  } else {
    Serial.println("Failed to connect to Telegram bot");
  }
}

void loop() {
  // Periksa koneksi Wi-Fi
  if (WiFi.status() != WL_CONNECTED) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Disconnect");
    reconnectWiFi();
  } else {
    // Membaca nilai analog dari sensor MQ-135
    int mqValue = analogRead(A0);

    // Membaca suhu dan kelembapan dari sensor DHT11
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Tampilkan teks di baris pertama
    lcd.setCursor(0, 0);
    lcd.print("EIS KELOMPOK 7");

    // Bersihkan baris kedua LCD
    lcd.setCursor(0, 1);
    lcd.print("                "); // Membersihkan baris kedua

    // Menampilkan indikator berdasarkan nilai sensor MQ-135 pada LCD
    lcd.setCursor(0, 1);
    lcd.print("Status: ");
    if (mqValue < 250) {
      lcd.print("Bersih");
      digitalWrite(ledRendah, HIGH); // Nyalakan LED rendah
      digitalWrite(ledSedang, LOW);  // Matikan LED sedang
      digitalWrite(ledTinggi, LOW);  // Matikan LED tinggi
      digitalWrite(buzzerPin, LOW);  // Matikan buzzer
      sendTelegramMessage("Status: Bersih", mqValue, t, h);
    } else if (mqValue < 400) {
      lcd.print("Rendah");
      digitalWrite(ledRendah, LOW);   // Matikan LED rendah
      digitalWrite(ledSedang, HIGH);  // Nyalakan LED sedang
      digitalWrite(ledTinggi, LOW);   // Matikan LED tinggi
      digitalWrite(buzzerPin, LOW);   // Matikan buzzer
      sendTelegramMessage("Status: Rendah", mqValue, t, h);
    } else {
      lcd.print("Tinggi");
      digitalWrite(ledRendah, LOW);   // Matikan LED rendah
      digitalWrite(ledSedang, LOW);   // Matikan LED sedang
      digitalWrite(ledTinggi, HIGH);  // Nyalakan LED tinggi
      digitalWrite(buzzerPin, HIGH);  // Nyalakan buzzer
      sendTelegramMessage("Status: Tinggi", mqValue, t, h);
    }
  }

  // Tunggu sejenak sebelum membaca kembali nilai dari sensor
  delay(2000);
}

void sendTelegramMessage(String status, int mqValue, float temp, float humidity) {
  String message = status + "\nNilai Sensor: " + String(mqValue);
  message += "\nSuhu: " + String(temp) + "C";
  message += "\nKelembapan: " + String(humidity) + "%";
  myBot.sendMessage(chatId, message);
}

void reconnectWiFi() {
  // Tampilkan pesan reconnecting di Serial Monitor
  Serial.print("Reconnecting to WiFi...");
  lcd.setCursor(0, 1);
  lcd.print("Reconnecting...");
  
  // Attempt to reconnect to WiFi
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi reconnect");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Reconnect");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP().toString());
  delay(3000); // Menampilkan pesan selama 3 detik
}
