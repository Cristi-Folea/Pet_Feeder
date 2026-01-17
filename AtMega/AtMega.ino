#include <Servo.h>

Servo feederServo;


// --- CONFIGURARE PINI ---
const int PIN_SERVO = 9;
const int PIN_TRIG = 10;
const int PIN_ECHO = 11;

// --- CONFIGURARE MECANICA ---
const int POZITIE_INCHIS = 0;    // Unghiul de repaus (ușa închisă)
const int POZITIE_DESCHIS = 60;  // Unghiul activ (ușa deschisă) - Ajustează dacă e prea mult

// --- CONFUGURARE SENZOR ULTRASUNETE ---
const float threshold = 25.00;
bool senzorUltrasunetePornit = false;

// --- CONFIGURATIE COD ARDUINO CLOUD ---
int timpCurgere[] = {400,600,800}; // timp curgere
char dataBuffer[3]; //buffer pentru a citi

// --- TIMING ---
const long TIMEOUT_SENZOR = 300000;
unsigned long ultimulTimpSenzor = 0;



void setup() {

  //Comunicare intre cipuri:
  Serial.begin(115200);
  feederServo.attach(PIN_SERVO);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  //Senzor ultrasunete
  

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
    switch (comanda){
      case 'H': //Comanda de dat mancare:
        if (mod > 0 && mod <= 3){
          executeFeeding(mod);
        }
        break;
      case 'U': //Comanda de schimbat ultrasunete: 
        if (mod == 0){
          senzorUltrasunetePornit = false;
        }
        if ( mod == 1){
          senzorUltrasunetePornit = true;
        }
        break;
      default : break;
    }
  }

  if (senzorUltrasunetePornit){

    if(millis() - ultimulTimpSenzor > TIMEOUT_SENZOR){
      if(prezentaUltrasunete()){
        executeFeeding(1);
        ultimulTimpSenzor = millis();
      }
    }

  }
}

bool prezentaUltrasunete (){
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH, 30000);
  if (duration == 0) return false;

  float distance = (duration * .0343/2);
  return (distance > 0 && distance <= threshold);
  
}

void executeFeeding(int mod) {
  
  feederServo.write(POZITIE_DESCHIS);

  delay(timpCurgere[mod - 1]);

  feederServo.write(POZITIE_INCHIS);
  delay(100);
}