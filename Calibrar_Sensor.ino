//Código para calibrar sensor VL53L0X
//---------------------------------------
#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const int maxDistance = 410; // Máxima distancia en mm (ajusta según tus necesidades)

void setup() {
  Serial.begin(115200);

  // Inicializa I2C con los pines específicos (ajústalos si es necesario)
  Wire.begin(21, 22);

  // Inicializa el sensor VL53L0X
  if (!lox.begin()) {
    Serial.println("Failed to boot VL53L0X");
    while (1);
  }
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  lox.rangingTest(&measure, false); // Realiza una medición

  if (measure.RangeStatus != 4) { // 4 significa "fuera de rango"
    int invertedDistance = maxDistance - measure.RangeMilliMeter;
    invertedDistance = constrain(invertedDistance, 0, maxDistance); // Asegura que los valores estén en rango
    Serial.print("Inverted Distance (mm): ");
    Serial.println(invertedDistance);
  } else {
    Serial.println("Out of range");
  }

  delay(100);
}
