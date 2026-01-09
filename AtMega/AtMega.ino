#include <Servo.h>

Servo feederServo;

// --- CONFIGURARE MECANICĂ (AICI MODIFICI UNGHIURILE) ---
const int PIN_SERVO = 9;
const int POZITIE_INCHIS = 0;    // Unghiul de repaus (ușa închisă)
const int POZITIE_DESCHIS = 60;  // Unghiul activ (ușa deschisă) - Ajustează dacă e prea mult
const int TIMP_CURGERE = 1000;   // 1000ms = 1 secundă de deschidere

// Variabilă pentru a citi datele seriale
char incomingByte; 

void setup() {
  // Pornim comunicarea Serială. 
  // Viteza trebuie să fie IDENTICĂ cu cea din codul ESP-ului!
  // 115200 este standardul, dar dacă ai erori, vom scădea ambele la 9600.
  Serial.begin(115200);

  // Atașăm servo-ul
  feederServo.attach(PIN_SERVO);

  // SIGURANȚĂ LA START:
  // Când primește curent, îl forțăm imediat pe "Închis" ca să nu curgă mâncare la reset.
  feederServo.write(POZITIE_INCHIS);
  
  // Așteptăm puțin să ajungă în poziție
  delay(500); 
}

void loop() {
  // Verificăm dacă ESP-ul a strigat la noi
  if (Serial.available() > 0) {
    // Citim caracterul
    incomingByte = Serial.read();

    // Dacă caracterul este 'H' (de la Hrănire), executăm
    if (incomingByte == 'H') {
      executeFeeding();
    }
  }
}

// Funcția care face mișcarea fizică
void executeFeeding() {
  // 1. Deschide ușa
  feederServo.write(POZITIE_DESCHIS);
  
  // 2. Așteaptă să curgă bobițele
  delay(TIMP_CURGERE);
  
  // 3. Închide ușa
  feederServo.write(POZITIE_INCHIS);
}