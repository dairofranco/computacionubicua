#include <Arduino.h>

// Definiciones de pines para los LEDs
#define Red_LED    18
#define Yellow_LED 20
#define Green_LED  19

// Pin ADC para los botones analógicos
#define Button_pin 27

// Rangos ADC para cada botón (ajustar según mediciones reales)
#define B0_L 740
#define B0_H 750

#define B1_L 800
#define B1_H 810

#define B2_L 860
#define B2_H 870

#define B3_L 900
#define B3_H 920

// Banderas de estado de los LEDs
bool redOn = false;
bool yellowOn = false;
bool greenOn = false;
bool allOn = false;

// Bandera de anti-rebote para evitar activaciones múltiples
bool keyIsPressed = false;

// Función de filtro de mediana (toma 5 muestras y devuelve el valor central)
int readMedianADC(int pin) {
  int readings[5];
  for (int i = 0; i < 5; i++) {
    readings[i] = analogRead(pin);
    delay(3); // Pequeño retraso entre muestras
  }
  
  // Ordenamiento de burbuja para encontrar la mediana
  for (int i = 0; i < 4; i++) {
    for (int j = i + 1; j < 5; j++) {
      if (readings[i] > readings[j]) {
        int temp = readings[i];
        readings[i] = readings[j];
        readings[j] = temp;
      }
    }
  }
  
  return readings[2]; // Devuelve el valor de la mediana
}

void setup() {
  Serial.begin(115200); // Inicializa comunicación serial
  
  // Configura pines de LEDs como salidas
  pinMode(Red_LED, OUTPUT);
  pinMode(Yellow_LED, OUTPUT);
  pinMode(Green_LED, OUTPUT);
  pinMode(Button_pin, INPUT); // Configura pin del botón como entrada
  
  // Inicializa LEDs apagados
  digitalWrite(Red_LED, LOW);
  digitalWrite(Yellow_LED, LOW);
  digitalWrite(Green_LED, LOW);
  
  Serial.println("System Ready."); // Indicate system initialization complete
}

void loop() {
  int adcValue = readMedianADC(Button_pin); // falta Read ADC value with median filter
  Serial.print("ADC: ");
  Serial.println(adcValue); // Print ADC value for debugging
  
  // Check if ADC value is within any button's range
  bool inRange =
    (adcValue >= B0_L && adcValue <= B0_H) ||
    (adcValue >= B1_L && adcValue <= B1_H) ||
    (adcValue >= B2_L && adcValue <= B2_H) ||
    (adcValue >= B3_L && adcValue <= B3_H);
  
  // Handle button press event
  if (inRange && !keyIsPressed) {
    keyIsPressed = true; // Set debounce flag
    
    // Button 0: Toggle all LEDs
    if (adcValue >= B0_L && adcValue <= B0_H) {
      allOn = !allOn; // Toggle all-LEDs state
      redOn = yellowOn = greenOn = allOn; // Sync individual LED states
      digitalWrite(Red_LED, redOn ? HIGH : LOW);
      digitalWrite(Yellow_LED, yellowOn ? HIGH : LOW);
      digitalWrite(Green_LED, greenOn ? HIGH : LOW);
      Serial.println("Button 0: Toggle ALL LEDs");
    }
    
    // Button 1: Toggle red LED
    else if (adcValue >= B1_L && adcValue <= B1_H) {
      redOn = !redOn; // Toggle red LED state
      digitalWrite(Red_LED, redOn ? HIGH : LOW);
      Serial.println("Button 1: Toggle RED");
    }
    
    // Button 2: Toggle yellow LED
    else if (adcValue >= B2_L && adcValue <= B2_H) {
      yellowOn = !yellowOn; // Toggle yellow LED state
      digitalWrite(Yellow_LED, yellowOn ? HIGH : LOW);
      Serial.println("Button 2: Toggle YELLOW");
    }
    
    // Button 3: Toggle green LED
    else if (adcValue >= B3_L && adcValue <= B3_H) {
      greenOn = !greenOn; // Toggle green LED state
      digitalWrite(Green_LED, greenOn ? HIGH : LOW);
      Serial.println("Button 3: Toggle GREEN");
    }
  }
  
  // Reset debounce flag when button is released
  if (!inRange && keyIsPressed) {
    keyIsPressed = false;
  }
  
  delay(20); // Small delay to reduce CPU load and debounce
}