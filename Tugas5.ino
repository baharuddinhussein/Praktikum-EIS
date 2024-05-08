const int photoresistorPin = 34; // Pin yang terhubung dengan sensor photoresistor
const int lampuPin = 2; // Pin yang terhubung dengan lampu

const int ambangCahaya = 500; // Ambang batas untuk mengaktifkan lampu (dalam lux)

void setup() {
  pinMode(lampuPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int nilaiCahaya = bacaCahaya();
  
  Serial.print("Nilai cahaya: ");
  Serial.println(nilaiCahaya);
  
  if (nilaiCahaya < ambangCahaya) {
    digitalWrite(lampuPin, HIGH); // Menyalakan lampu
  } else {
    digitalWrite(lampuPin, LOW); // Mematikan lampu
  }
  
  delay(1000); // Delay untuk mengurangi frekuensi pembacaan sensor
}

int bacaCahaya() {
  int nilaiSensor = analogRead(photoresistorPin);
  
  // Konversi nilai sensor menjadi lux (sangat bergantung pada karakteristik sensor)
  // Anda perlu melakukan kalibrasi untuk sensor Anda
  int nilaiLux = map(nilaiSensor, 0, 4095, 0, 1023);
  
  return nilaiLux;
}
