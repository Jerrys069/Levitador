#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const int maxDistance = 520; // Máxima distancia en mm
const int Tm = 20;

const int freq = 500;
const int ledChannel = 0;
const int resolution = 12;

char valor;

String rgbst;
long int rgbint;

int red;
int green;
int blue;

int led_red = 9;
int led_green = 18;
int led_blue = 11;
int brightness = 0; // how bright the LED is
int entanalog = 34;
int entanalog1 = 35;
int volta = 0;
char voltaCHAR;
String entanalogSTRING;
String entanalog1STRING;
String voltaSTRING;
String voltaSTRING1;
String Invert;
double now = 0;  //tambien puede ser "unsigned long"
  double tiempopasado = 0;
  double tiempocambiado = 0;
void setup() {
  Serial.begin(115200);

  // Configuración PWM
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(led_green, ledChannel);
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_blue, OUTPUT);

  // Inicializa I2C con los pines específicos
  Wire.begin(21, 22);

  // Inicializa el sensor VL53L0X
  if (!lox.begin()) {
    Serial.println("Failed to boot VL53L0X");
    while (1);
  }
}

void loop() {
  /////////////////////////////////////////////////////////////////////////////
  // Lectura analógica y envío de datos
  /*volta = (analogRead(entanalog1)) / 1;
  entanalogSTRING = String(analogRead(entanalog1), DEC);
  entanalog1STRING = String(0, DEC);

  brightness = volta;
  voltaCHAR = volta;
  voltaSTRING = String("A" + entanalogSTRING);
  voltaSTRING1 = String("B" + entanalog1STRING);

  Serial.print(voltaSTRING); // Bytes at Port
  Serial.print(voltaSTRING1);

  delay(Tm);*/
  /////////////////////////////////////////////////////////////////////////////
  now = millis();
  tiempocambiado = now - tiempopasado;

  if (tiempocambiado >= Tm)

      {
        VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // Realiza una medición

  if (measure.RangeStatus != 4) { // 4 significa "fuera de rango"
    int invertedDistance = maxDistance - measure.RangeMilliMeter;
    invertedDistance = constrain(invertedDistance, 0, maxDistance); // Asegura rango
    String InvertC= String(invertedDistance,DEC);
   
   Invert = String("C" + InvertC);
    Serial.print(Invert);
  }
  
      
      tiempopasado = now;
      }
  /*else /cambiar si no funciona
      if ((tiempocambiado = 0) && (tiempocambiado < Tm))
      {
        ledcWrite(ledChannel, green);
      }*/
  // Control RGB basado en datos recibidos por Serial
  if (Serial.available()) {
    valor = Serial.read();
    rgbst += valor;
    if (valor == '\n') {
      rgbint = rgbst.toInt();
      green = rgbint;
      ledcWrite(ledChannel, green);
      rgbst = "";
    }
  }

  /////////////////////////////////////////////////////////////////////////////
 
}
