// Definición de pines_
#define BUTTON_PIN 0
#define LED_PIN 1

int ledState = LOW; // 0 significa que el LED está apagado inicialmente

void setup() {
  // Configura el pin del botón con resistencia pull-up interna
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // Configura el pin del LED como salida
  pinMode(LED_PIN, OUTPUT);
  
  digitalWrite(LED_PIN, ledState);
}

void loop() {
  // Detecta si el botón ha sido presionado (el valor será LOW debido al pull-up)
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(100); // Pequeño retardo para evitar rebotes (debouncing)
    
    // Si sigue presionado después del delay, cambiamos el estado
    if (digitalRead(BUTTON_PIN) == LOW) {
      ledState = !ledState; // Alterna el estado (si era LOW pasa a HIGH y viceversa)
      digitalWrite(LED_PIN, ledState);
      
      // Espera a que el usuario suelte el botón para no repetir el ciclo
      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
      }
    }
  }
}