//Conecciones
//MPU6050  -  Arduino (ATmega328)
//GND      -  GND
//VCC      -  VCC
//SDA      -  Pin A4
//SCL      -  Pin A5
//INT      -  Pin 2

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 mpu;
#define INTERRUPT_PIN 2
#define LED_PIN 13
bool blinkState = false;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]
VectorInt16 aa;         // [x, y, z]
VectorInt16 aaReal;     // [x, y, z]
VectorInt16 aaWorld;    // [x, y, z]
VectorFloat gravity;    // [x, y, z]
float ypr[3];           // [yaw, pitch, roll]

volatile bool mpuInterrupt = false;

int ang_x, ang_y, ang_z;
int valor_x = 0;
int valor_y = 0;
int valor_z = 0;

void dmpDataReady() {
  mpuInterrupt = true;
}

void setup()
{
  Serial.begin(9600);
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  Wire.setClock(400000);
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  mpu.testConnection();
  devStatus = mpu.dmpInitialize();

  // Valores de calibracion
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1688);

  // Activar DMP
  if (devStatus == 0) {
    //Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // Activar interrupcion
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    //Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    //(if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
}

void loop()
{
  // Si falla al iniciar, parar programa
  if (!dmpReady) return;

  // Ejecutar mientras no hay interrupcion
  while (!mpuInterrupt && fifoCount < packetSize) {
  }

  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // Obtener datos del FIFO
  fifoCount = mpu.getFIFOCount();

  // Controlar overflow
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
    // Serial.println(F("FIFO overflow!"));
  }
  else if (mpuIntStatus & 0x02) {
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    mpu.getFIFOBytes(fifoBuffer, packetSize);

    fifoCount -= packetSize;

    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  }

  ang_z = ypr[0] * 180 / M_PI;
  ang_y = ypr[1] * 180 / M_PI;
  ang_x = ypr[2] * 180 / M_PI;

  ////////////////////////////   X
  if (ang_x > 30) // Si el valor del eje X enviado por el sensor es mayor que este numero la variable "valor_x" se pone en "10"
  {
    valor_x = 10;
  }

  if (ang_x < -10) // Si el valor del eje X enviado por el sensor es menor que este numero la variable "valor_x" se pone en "20"
  {
    valor_x = 20;
  }

  if (ang_x > 4 && ang_x < 20) // Si el valor del eje X esta entre estos dos valores entonces la variable "valor_x" se pone en "0"
  {
    valor_x = 0;
  }

  ////////////////////////////   Y

  if (ang_y > 20)
  {
    valor_y = 40;
  }

  if (ang_y < -11)
  {
    valor_y = 50;
  }

  if (ang_y > -10 && ang_y < 19)
  {
    valor_y = 0;
  }

  ////////////////////////////   Z

  if (ang_z > 30)
  {
    valor_z = 70;
  }

  if (ang_z < -10)
  {
    valor_z = 80;
  }

  if (ang_z > -10 && ang_z < 30)
  {
    valor_z = 0;
  }

  ////////////////////////////

  // Mostrar Yaw, Pitch, Roll
  //Serial.println(ang_x); // Valores devueltos por el MPU6050
  //Serial.println(ang_y);
  //Serial.println(ang_z);

  //Serial.print("valor de X: ");
  Serial.println(valor_x);
  //Serial.print("valor de Y: ");
  Serial.println(valor_y);
  //Serial.print("valor de Z: ");
  //Serial.println(valor_z);
  delay(100);
}
