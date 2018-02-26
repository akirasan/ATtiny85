/*

JUEGO de SIMON con DOS posibles movimientos
Ejemplo de uso de un ATTiny85
by akirasan  (akirasan.net)
Febrero 2018

*/

// --- definicion de PINS ATTiny85
// LED1 con PUSHB1
// LED2 con PUSHB2

#define LED1 2
#define LED2 1

#define PUSHB1 3
#define PUSHB2 4

#define BUZZER 0

#define MAX_MOVIMIENTOS 50

byte secuencia[MAX_MOVIMIENTOS];
boolean juego_iniciado = false;
byte pos_actual = 0;

// Animación y sonido de cuando se inicia un nuevo juego
void inicio_juego() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED1, HIGH); delay(150); digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH); delay(150); digitalWrite(LED2, LOW);
    analogWrite(BUZZER, 10); delay(15); digitalWrite(BUZZER, LOW);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED1, HIGH); digitalWrite(LED2, HIGH);
    analogWrite(BUZZER, 10); delay(15); digitalWrite(BUZZER, LOW);
    delay(500);
    digitalWrite(LED1, LOW); digitalWrite(LED2, LOW);
    delay(500);
  }
  randomSeed(analogRead(0));
  turno_simon();
}

// Recorre la secuencia de movimientos realizada hasta ahora por SIMON
void recordar_secuencia() {
  digitalWrite(LED1, LOW); digitalWrite(LED2, LOW);
  delay(800);

  for (int x = 1; x <= pos_actual; x++) {
    if (secuencia[x] == 0) {
      analogWrite(BUZZER, 155); digitalWrite(LED1, HIGH); delay(10); digitalWrite(BUZZER, LOW);
      delay(240);
      digitalWrite(LED1, LOW);
    }
    if (secuencia[x] == 1) {
      analogWrite(BUZZER, 25); digitalWrite(LED2, HIGH);  delay(10); digitalWrite(BUZZER, LOW);
      delay(240);
      digitalWrite(LED2, LOW);
    }
    delay(250);
  }
}

// Añade un nuevo moviento a la secuencia
void add_secuencia() {
  pos_actual++;
  secuencia[pos_actual] = random(100) % 2;
}

// El jugador ha fallado un movimiento, volvemos a comenzar el juego
void fallo_jugador() {
  analogWrite(BUZZER, 2024);
  digitalWrite(LED1, HIGH); digitalWrite(LED2, HIGH);
  delay(2000);
  digitalWrite(LED1, LOW); digitalWrite(LED2, LOW);
  analogWrite(BUZZER, 0);

  pos_actual = 0;
  inicio_juego();
}

// Turno del jugaor, si falla algún moviento devuelve FALSE
boolean turno_jugador() {

  boolean correcto = true;
  byte x = 1;
  while ((correcto) && (x <= pos_actual)) {

    if (digitalRead(PUSHB1) == HIGH) {
      if (secuencia[x] == 0) {
        digitalWrite(LED1, HIGH); delay(250); digitalWrite(LED1, LOW);
        x++;
      }
      else
      {
        correcto = false;
      }
    }

    if (digitalRead(PUSHB2) == HIGH) {
      if (secuencia[x] == 1) {
        digitalWrite(LED2, HIGH); delay(250); digitalWrite(LED2, LOW);
        x++;
      }
      else
      {
        correcto = false;
      }
    }
  }
  return correcto;
}

// Turno de SIMON, añade nuevo moviento y muestra toda la secuencia
void turno_simon() {
  add_secuencia();
  recordar_secuencia();
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
void setup() {
  pinMode(PUSHB1, INPUT);
  pinMode(PUSHB2, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  inicio_juego();
}



void loop() {
  if (turno_jugador()) {
    turno_simon();
  }
  else   //FALLO GAMER!!!
  {
    fallo_jugador();
  }
}
