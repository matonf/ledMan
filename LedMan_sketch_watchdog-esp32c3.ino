/*
  Code ledManESP32-c3 par ONFRAY Matthieu
  Mes travaux sont sur : www.onfray.info
  Pour carte ESP32-c Seeed studio, avec led RGB en cathode commune (brancher sur gnd) 
  avec Watchdof géré par la librairie AdaFruit
 */

#include <Adafruit_SleepyDog.h>


//utilisation du preprocesseur
//LED_RGB true ou false
#define LED_RGB true
//PROTO true ou false
#define PROTO true

#if PROTO == true
//multiple des temps d'exécution aléatoires des actions qui tournent en boucle
const byte multiple = 1;
#else
const byte multiple = 6;
#endif

// VARIABLES DES LED
#if LED_RGB == true
// Broches utilisée pour LED, brancher sur des pin digital
const byte PIN_LED_RED = 10;
const byte PIN_LED_GREEN = 9;
const byte PIN_LED_BLUE = 8;
#else
const byte PIN_LED_BLUE = 8;
#endif


// FONCTIONS LED
#if LED_RGB == true
void coloreLed(unsigned int wait) {
  Serial.println("Passage dans coloreLed");
  setColor(255, 0, 0, wait*2);
  setColor(0, 255, 0, wait*2);
  setColor(0, 0, 255, wait*2);
  setColor(255, 0, 255, wait*2);
  setColor(0, 255, 255, wait*2);
  setColor(255, 255, 0, wait*2);
  setColor(255, 255, 255, wait*2);
  //setColor(0, 0, 0, 0);
  Serial.println("Sortie de coloreLed");
}
#endif


void clignoteLed(unsigned int wait) 
{
  Serial.println("Passage dans clignoteLed");
  byte i;
  for (i = 0; i < 5; i++) 
  {
    analogWrite(PIN_LED_BLUE, 0);
    delay(wait*3);
    analogWrite(PIN_LED_BLUE, 255);
    delay(wait*3);
  }

  for (i = 0; i < 5; i++) 
  {
    analogWrite(PIN_LED_BLUE, 0);
    delay(wait * 8);
    analogWrite(PIN_LED_BLUE, 255);
    delay(wait * 8);
  }
  Serial.println("Sortie de clignoteLed");
}


#if LED_RGB == true
// Affiche une couleur
void setColor(byte redValue, byte greenValue, byte blueValue, unsigned int wait) 
{
  analogWrite(PIN_LED_RED, redValue);
  analogWrite(PIN_LED_GREEN,  greenValue);
  analogWrite(PIN_LED_BLUE, blueValue);
  delay(wait * 2);
}
#endif


void fadeLed(unsigned int wait) {
  Serial.println("Passage dans fadeLed");
  byte val = 0;
  // Variables pour les effets lumineux
  byte fadeAmount = 5;  // how many points to fade the LED by
  byte fadeValue;
  while (val < 6) 
  {
    //croissant
    for (fadeValue = 0; fadeValue < 255; fadeValue += fadeAmount) 
    {
      // set the brightness of pins
#if LED_RGB == true
      analogWrite(PIN_LED_GREEN, fadeValue);
      analogWrite(PIN_LED_RED, fadeValue);
#endif
      analogWrite(PIN_LED_BLUE, fadeValue);
      // wait for XX milliseconds to see the dimming effect
      delay(wait);
    }

    //décroissant
    for (fadeValue = 255; fadeValue > 0; fadeValue -= fadeAmount) 
    {
#if LED_RGB == true
      analogWrite(PIN_LED_GREEN, fadeValue);
      analogWrite(PIN_LED_RED, fadeValue);
#endif
      analogWrite(PIN_LED_BLUE, fadeValue);
      delay(wait);
    }
    //augmente le compteur de boucle
    val++;
  }
  Serial.println("Sortie de fadeLed");
}


#if LED_RGB == true
void rgbLed(unsigned int wait) {
  Serial.println("Passage dans rgbLed");
  actionLed_ON(PIN_LED_RED);
  delay(wait*2);
  actionLed_OFF(PIN_LED_RED);
  actionLed_ON(PIN_LED_GREEN);
  delay(wait*2);
  actionLed_OFF(PIN_LED_GREEN);
  actionLed_ON(PIN_LED_BLUE);
  delay(wait*2);
  actionLed_ON(PIN_LED_GREEN);
  delay(wait*4);
  actionLed_OFF(PIN_LED_BLUE);
  actionLed_ON(PIN_LED_RED);
  delay(wait*4);
  actionLed_OFF(PIN_LED_GREEN);
  actionLed_ON(PIN_LED_BLUE);
  delay(wait*4);
  actionLed_ON(PIN_LED_GREEN);
  delay(wait*4);
  actionLed_OFF(PIN_LED_GREEN);
  actionLed_OFF(PIN_LED_RED);
  actionLed_OFF(PIN_LED_BLUE);
  Serial.println("Sortie de rgbLed");
}


void actionLed_ON(byte pin) {
  for (byte i = 1; i < 255; i++) {
    analogWrite(pin, i);
    delay(10);
  }
}


void actionLed_OFF(byte pin) {
  for (byte i = 255; i > 1; i--) {
    analogWrite(pin, i);
    delay(10);
  }
}
#endif


// FONCTIONS OBLIGATOIRES ARDUINO

// Fonction setup(), appelée au démarrage de la carte Arduino
void setup() {
  // Initialise les broches
#if LED_RGB == true
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
#endif
  pinMode(PIN_LED_BLUE, OUTPUT);

  Serial.begin(115200);
  while (!Serial)
    ;  // wait for Arduino Serial Monitor (native USB boards)
  randomSeed(analogRead(2));
  Serial.println("Passage dans le setup() fini");
}


// Fonction loop(), appelée continuellement en boucle tant que la carte Arduino est alimentée
void loop() {

#if LED_RGB == true
  //4 actions définies pour la LED RGB
  switch (random(0, 4))
#else
  //2 actions seulement en LED mono
  switch (random(0, 2))
#endif
  {
    case 0: clignoteLed(random(100, 130 * multiple)); break;
    case 1: fadeLed(random(20, 12 * multiple)); break;
#if LED_RGB == true
    case 2: rgbLed(random(100, 500 * multiple)); break;
    case 3: coloreLed(random(1000, 1000 * multiple)); break;
#endif
  }   


  //raz la couleur de la led
#if LED_RGB == true
    setColor(0, 0, 0, 0);
#endif


  //recalcule un prochain délai aléatoire avant prochaines animations
  int delaiAleatoire = random(multiple, multiple * multiple * 10);
  Serial.print("On va attendre ");
  Serial.print(delaiAleatoire);
  Serial.println(" secondes.");

  byte xyz = 0;
  int sleePT = 0;
  while (xyz < delaiAleatoire) 
  {
#if PROTO == true
    int sleepMS = 1000;
    delay(sleepMS);
#else
    int sleepMS = Watchdog.sleep(1000);
#endif
    xyz++;
    sleePT += sleepMS;
  }

#if PROTO == true
  Serial.print("J'ai attendu ");
#else
  Serial.print("J'ai dormi ");
#endif
  Serial.print(sleePT/1000);
  Serial.println(" secondes.");

}
