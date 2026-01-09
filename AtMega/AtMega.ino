#include <Servo.h>

Servo feederServo;

// --- CONFIGURARE MECANICĂ ---
const int PIN_SERVO = 9;
const int POZITIE_INCHIS = 0;    // Unghiul de repaus (ușa închisă)
const int POZITIE_DESCHIS = 60;  // Unghiul activ (ușa deschisă) - Ajustează dacă e prea mult

// --- CONFIGURATIE COD ARDUINO CLOUD ---
int timpCurgere[] = {400,600,800}; // timp curgere
char dataBuffer[3]; //buffer pentru a citi

void setup() {

  //Comunicare intre cipuri:
  Serial.begin(115200);
  feederServo.attach(PIN_SERVO);

  //Pozitia initiala:
  feederServo.write(POZITIE_INCHIS);

  delay(500); 
}

void loop() {
  // Verificăm dacă ESP-ul a strigat la noi
  if (Serial.available() >= 2) {
    // Citim caracterul
    int bytesRead = Serial.readBytes(dataBuffer,2);
    dataBuffer[bytesRead] = '\0';

    int mod = dataBuffer[1] - '0';
    char comanda = dataBuffer[0];

    if (comanda == 'H' && (mod > 0 && mod <=3 )){
      executeFeeding (mod);
    }
  }
}

void executeFeeding(int mod) {
  
  feederServo.write(POZITIE_DESCHIS);

  delay(timpCurgere[mod - 1]);

  feederServo.write(POZITIE_INCHIS);
}