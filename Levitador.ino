#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const int maxDistance = 400; // Máxima distancia en mm
const int Tm = 50; // Tiempo de muestreo más rápido

const int freq = 500;
const int resolution = 12;

char serialBuffer[10];
byte bufferIndex = 0;

int red, green, blue;
int led_red = 10;
int led_green = 11;
int led_blue = 12;

unsigned long now = 0;
unsigned long tiempopasado = 0;

void setup() {
  Serial.begin(115200); // Asegúrate de que LabVIEW use la misma velocidad

  // Configurar PWM
  ledcSetup(0, freq, resolution);  // Canal 0 -> Verde
  ledcSetup(1, freq, resolution);  // Canal 1 -> Rojo
  ledcSetup(2, freq, resolution);  // Canal 2 -> Azul

  ledcAttachPin(led_green, 0);
  ledcAttachPin(led_red, 1);
  ledcAttachPin(led_blue, 2);

  // Pines I2C del ESP32-S3 (ajusta si es necesario)
  Wire.begin(8, 9); // SDA = GPIO8, SCL = GPIO9

  if (!lox.begin()) {
    Serial.println("Failed to boot VL53L0X");
    while (1);
  }
}

void loop() {
  now = millis();
  if ((now - tiempopasado) >= Tm) {
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);

    if (measure.RangeStatus != 4) {
      int invertedDistance = maxDistance - measure.RangeMilliMeter;
      invertedDistance = constrain(invertedDistance, 0, maxDistance);
      Serial.print("C");
      Serial.print(invertedDistance); // Sin espacios ni saltos
    }

    tiempopasado = now;
  }

  // Lectura rápida del valor PWM enviado por serial
  while (Serial.available()) {
    char incoming = Serial.read();
    if (incoming == '\n') {
      serialBuffer[bufferIndex] = '\0'; // Termina cadena
      int pwmValue = atoi(serialBuffer); // Convierte a int
      green = constrain(pwmValue, 0, 4095); // PWM 12 bits
      ledcWrite(0, green); // Aplica al canal verde
      bufferIndex = 0;
    } else {
      if (bufferIndex < sizeof(serialBuffer) - 1) {
        serialBuffer[bufferIndex++] = incoming;
      }
    }
  }
}
