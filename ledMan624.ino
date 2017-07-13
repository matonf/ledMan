/*
  Code ledMan624 par ONFRAY Matthieu
  Mes travaux sont sur : www.onfray.info
 */

#include <avr/sleep.h>
#include <avr/wdt.h>

// Utility macros
#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)

// Utilisation du preprocesseur
// LED_RGB vaut soit true ou false
#define LED_RGB false
// PROTO vaut soit true ou false
#define PROTO false

// VARIABLES DU CHIEN DE GARDE
// Compteur temps réel de dodo du chien de garde
byte watchdog_compteur = 0;
// Nombre de dodo à faire
byte delaiAleatoire;

#if PROTO == true
// Temps d'un dodo : 9 => 8 secondes de delai, 8 => 4 secondes de délai, 7 => 2 secondes de délai, 6 => 1 seconde de délai
const byte ECHELLE_TEMPS_WDT = 6;
//multiple des temps d'exécution aléatoires des actions qui tournent en boucle
const byte MULTIPLE = 2;
#else
const byte ECHELLE_TEMPS_WDT = 9;
const byte MULTIPLE = 8;
#endif

// VARIABLES DU MODE 6H sur 24
// nombre de dodo en 24h : 10800 (24 x 60 x 60 / 8)
const unsigned int T24H = 10800;
// nombre de dodo en 6h : 2700
const unsigned int T6H = T24H / 4;
// compteur qui atteindra 10800
unsigned int t24h_compteur = 0;

// VARIABLES DES LED
#if LED_RGB == true
// 8 couleurs : NOM = format RGB en binaire
const byte COLOR_BLACK = 0b000;
const byte COLOR_RED = 0b100;
const byte COLOR_GREEN = 0b010;
const byte COLOR_BLUE = 0b001;
const byte COLOR_MAGENTA = 0b101;
const byte COLOR_CYAN = 0b011;
const byte COLOR_YELLOW = 0b110;
const byte COLOR_WHITE = 0b111;

// Broches utilisée pour LED RGB en anode commune
const byte PIN_LED_R = 0;
const byte PIN_LED_G = 1;
const byte PIN_LED_B = 2;
#else
const byte PIN_LED_B = 0;
#endif

// FONCTIONS CHIEN DE GARDE


// Use Watchdog for time delay
void WDDelay(int n) {
  //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  WDTCR = 1 << WDIE | (n & 0x8) << 2 | 1 << WDE | (n & 0x7);
  sleep_enable();
  sleep_cpu();
}


//This runs each time the watch dog wakes us up from sleep
ISR(WDT_vect)
{
  // Special sequence to disable watchdog timer
  WDTCR = 1 << WDCE | 1 << WDE;
  WDTCR = 0;
  watchdog_compteur++;
  t24h_compteur++;
  //24 heures ont passé
  if (t24h_compteur >= T24H)
  {
    t24h_compteur = 0;
    // petit clignotement de cycle de 24 heures
#if LED_RGB == true
    coloreLed(750);
#else
    clignoteLed(10);
#endif
  }
}


// FONCTIONS LED

#if LED_RGB == true
void coloreLed(unsigned int wait)
{
  displayColor(COLOR_RED, wait);
  displayColor(COLOR_GREEN, wait);
  displayColor(COLOR_BLUE, wait);
  displayColor(COLOR_MAGENTA, wait);
  displayColor(COLOR_CYAN, wait);
  displayColor(COLOR_YELLOW, wait);
  displayColor(COLOR_WHITE, wait);
  displayColor(COLOR_BLACK, wait);
}
#endif


void clignoteLed(unsigned int wait)
{
  for (unsigned int i = 0 ; i < 5 ; i++ )
  {
    digitalWrite(PIN_LED_B, LOW);
    delay(wait);
    digitalWrite(PIN_LED_B, HIGH);
    delay(wait);
  }

  for (unsigned int i = 0 ; i < 5 ; i++ )
  {
    digitalWrite(PIN_LED_B, LOW);
    delay(wait * 4);
    digitalWrite(PIN_LED_B, HIGH);
    delay(wait * 4);
  }
}


#if LED_RGB == true
// Affiche une couleur
void displayColor(byte color, unsigned int wait)
{
  // Assigne l'état des broches
  digitalWrite(PIN_LED_R, bitRead(color, 2));
  digitalWrite(PIN_LED_G, bitRead(color, 1));
  digitalWrite(PIN_LED_B, bitRead(color, 0));
  delay(wait);
}
#endif


void fadeLed(unsigned int wait)
{
  byte val = 0;
  // Variables pour les effets lumineux
  byte fadeAmount = 5;    // how many points to fade the LED by
  byte fadeValue;
  while (val < 5)
  {
    // croissant
    for (fadeValue = 0 ; fadeValue < 255; fadeValue += fadeAmount)
    {
      // set the brightness of pins
#if LED_RGB == true
      analogWrite(PIN_LED_G, fadeValue);
      analogWrite(PIN_LED_R, fadeValue);
#endif
      analogWrite(PIN_LED_B, fadeValue);
      // wait for XX milliseconds to see the dimming effect
      delay(wait);
    }

    // décroissant
    for (fadeValue = 255 ; fadeValue > 0; fadeValue -= fadeAmount)
    {
#if LED_RGB == true
      analogWrite(PIN_LED_G, fadeValue);
      analogWrite(PIN_LED_R, fadeValue);
#endif
      analogWrite(PIN_LED_B, fadeValue);
      delay(wait);
    }
    // augmente le compteur de boucle
    val++;
  }
}


#if LED_RGB == true
void rgbLed(unsigned int wait)
{
  actionLed_ON(PIN_LED_R);
  delay(wait);
  actionLed_OFF(PIN_LED_R);
  actionLed_ON(PIN_LED_G);
  delay(wait);
  actionLed_OFF(PIN_LED_G);
  actionLed_ON(PIN_LED_B);
  delay(wait);
  actionLed_ON(PIN_LED_G);
  delay(wait * 2);
  actionLed_OFF(PIN_LED_B);
  actionLed_ON(PIN_LED_R);
  delay(wait * 2);
  actionLed_OFF(PIN_LED_G);
  actionLed_ON(PIN_LED_B);
  delay(wait * 2);
  actionLed_ON(PIN_LED_G);
  delay(wait * 2);
  actionLed_OFF(PIN_LED_G);
  actionLed_OFF(PIN_LED_R);
  actionLed_OFF(PIN_LED_B);
}
#else
void superFadeLed()
{
  actionLed_ON(PIN_LED_B);
  actionLed_OFF(PIN_LED_B);
}
#endif

void actionLed_ON(byte pin)
{
  for (byte i = 1; i < 255; i++)
  {
    analogWrite(pin, i);
    delay(10);
  }
}


void actionLed_OFF(byte pin)
{
  for (byte i = 255; i > 1; i--)
  {
    analogWrite(pin, i);
    delay(10);
  }
}



// FONCTIONS OBLIGATOIRES ARDUINO

// Fonction setup(), appelée au démarrage de la carte Arduino
void setup() {
  // Initialise les broches
#if LED_RGB == true
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
#endif
  pinMode(PIN_LED_B, OUTPUT);
  // si la broche analogique n'est pas connectée, le bruit analogique aléatoire
  // provoquera l'appel de l'instruction randomSeed() pour générer
  // différent nombre de départ à chaque exécution du programme.
  // randomSeed() brouillera alors la fonction aléatoire
  randomSeed(analogRead(4));
  // première économie d'énergie : couper l'ADC
  adc_disable(); // ADC uses ~320uA
  // seconde économie d'énergie : couper le compteur N°1 (pas le 0) et l'USI
  PRR = 0 << PRTIM1 | 1 << PRTIM0 | 0 << PRUSI;
  // choisit le mode de sommeil le plus économe
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  // définit un premier temps de dodo très court pour le plaisir des yeux
  delaiAleatoire = random(MULTIPLE, MULTIPLE * 2);
  // test visuel de la led : lance un cycle de couleurs
#if LED_RGB == true
  coloreLed(750);
#else
  clignoteLed(10);
#endif
}


// Fonction loop(), appelée continuellement en boucle tant que la carte Arduino est alimentée
void loop()
{
#if LED_RGB == true && PROTO == true
  displayColor(COLOR_MAGENTA, 0);
#endif
  // Go to sleep to save power until the watchdog timer wakes us up
  WDDelay(ECHELLE_TEMPS_WDT);
  // rendu pour tester le prototype
#if LED_RGB == true && PROTO == true
  displayColor(COLOR_GREEN, 1000);
  displayColor(COLOR_BLACK, 0);
#endif

  // on a assez dormi : action !
  if (watchdog_compteur > delaiAleatoire)
  {
    // raz du compteur de dodo
    watchdog_compteur = 0;

    // quatrième économie d'énergie : n'allumer la LED que 6 heures sur 24h
    if (t24h_compteur <= T6H)
    {
      // recalcule un prochain délai aléatoire avant prochaines animations
      delaiAleatoire = random(MULTIPLE, MULTIPLE * MULTIPLE);
      // raz la couleur de la led
#if LED_RGB == true && PROTO == true
      displayColor(COLOR_BLACK, 0);
#endif

#if LED_RGB == true
      // 4 actions définies pour la LED RGB
      switch (random(0, 4))
#else
      // 3 actions seulement en LED mono
      switch (random(0, 3))
#endif
      {
        case 0 : clignoteLed(random(100, 500 * MULTIPLE)); break;
        case 1 : fadeLed(random(50, 300 * MULTIPLE)); break;
#if LED_RGB == true
        case 2 : rgbLed(random(100, 500 * MULTIPLE)); break;
        case 3 : coloreLed(random(150, 300 * MULTIPLE)); break;
#else
        case 2 : superFadeLed(); break;
#endif
      }

      // raz la couleur de la led en fin de cycle
#if LED_RGB == true
      displayColor(COLOR_BLACK, 0);
#endif
    }
  }
}

