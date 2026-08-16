#include <Arduino.h>

// Definiciones de pines para los LEDs
#define Led_Rojo     18
#define Led_Amarillo 20
#define Led_Verde    19

// Pin ADC para los botones analógicos
#define Pin_Boton 27

// Rangos ADC para cada botón (ajustar según mediciones reales)
#define B0_Min 740
#define B0_Max 750

#define B1_Min 800
#define B1_Max 810

#define B2_Min 860
#define B2_Max 870

#define B3_Min 900
#define B3_Max 920

// Banderas de estado de los LEDs
bool rojoEncendido = false;
bool amarilloEncendido = false;
bool verdeEncendido = false;
bool todosEncendidos = false;

// Bandera de anti-rebote para evitar activaciones múltiples
bool teclaPresionada = false;

// Función de filtro de mediana (toma 5 muestras y devuelve el valor central)
int leerMedianaADC(int pin) {
  int lecturas[1];
  for (int i = 0; i < 5; i++) {
    lecturas[i] = analogRead(pin);
    delay(3); // Pequeño retraso entre muestras
  }
  
  // Ordenamiento de burbuja para encontrar la mediana
  for (int i = 0; i < 4; i++) {
    for (int j = i + 1; j < 5; j++) {
      if (lecturas[i] > lecturas[j]) {
        int temporal = lecturas[i];
        lecturas[i] = lecturas[j];
        lecturas[j] = temporal;
      }
    }
  }
  
  return lecturas[2]; // Devuelve el valor de la mediana
}

void setup() {
  Serial.begin(115200); // Inicializa comunicación serial
  
  // Configura pines de LEDs como salidas
  pinMode(Led_Rojo, OUTPUT);
  pinMode(Led_Amarillo, OUTPUT);
  pinMode(Led_Verde, OUTPUT);
  pinMode(Pin_Boton, INPUT); // Configura pin del botón como entrada
  
  // Inicializa LEDs apagados
  digitalWrite(Led_Rojo, LOW);
  digitalWrite(Led_Amarillo, LOW);
  digitalWrite(Led_Verde, LOW);
  
  Serial.println("Sistema Listo."); 
}

void loop() {
  int valorAdc = leerMedianaADC(Pin_Boton); // Lee valor ADC con filtro
  Serial.print("ADC: ");
  Serial.println(valorAdc); // Imprime valor para depuración
  
  // Verifica si el valor está dentro del rango de algún botón
  bool enRango =
    (valorAdc >= B0_Min && valorAdc <= B0_Max) ||
    (valorAdc >= B1_Min && valorAdc <= B1_Max) ||
    (valorAdc >= B2_Min && valorAdc <= B2_Max) ||
    (valorAdc >= B3_Min && valorAdc <= B3_Max);
  
  // Maneja el evento de presionar botón
  if (enRango && !teclaPresionada) {
    teclaPresionada = true; // Activa anti-rebote
    
    // Botón 0: Alterna todos los LEDs
    if (valorAdc >= B0_Min && valorAdc <= B0_Max) {
      todosEncendidos = !todosEncendidos;
      rojoEncendido = amarilloEncendido = verdeEncendido = todosEncendidos;
      digitalWrite(Led_Rojo, rojoEncendido ? HIGH : LOW);
      digitalWrite(Led_Amarillo, amarilloEncendido ? HIGH : LOW);
      digitalWrite(Led_Verde, verdeEncendido ? HIGH : LOW);
      Serial.println("Botón 0: Alternar TODOS los LEDs");
    }
    
    // Botón 1: Alterna LED rojo
    else if (valorAdc >= B1_Min && valorAdc <= B1_Max) {
      rojoEncendido = !rojoEncendido;
      digitalWrite(Led_Rojo, rojoEncendido ? HIGH : LOW);
      Serial.println("Botón 1: Alternar ROJO");
    }
    
    // Botón 2: Alterna LED amarillo
    else if (valorAdc >= B2_Min && valorAdc <= B2_Max) {
      amarilloEncendido = !amarilloEncendido;
      digitalWrite(Led_Amarillo, amarilloEncendido ? HIGH : LOW);
      Serial.println("Botón 2: Alternar AMARILLO");
    }
    
    // Botón 3: Alterna LED verde
    else if (valorAdc >= B3_Min && valorAdc <= B3_Max) {
      verdeEncendido = !verdeEncendido;
      digitalWrite(Led_Verde, verdeEncendido ? HIGH : LOW);
      Serial.println("Botón 3: Alternar VERDE");
    }
  }
  
  // Reinicia la bandera cuando se suelta el botón
  if (!enRango && teclaPresionada) {
    teclaPresionada = false;
  }
  
  delay(20); // Reduce carga del CPU y ayuda al anti-rebote
}