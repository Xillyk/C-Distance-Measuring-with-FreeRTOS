#include <Arduino_FreeRTOS.h>

void TaskSendData( void *pvParameters );
void TaskReadSensor( void *pvParameters );
void TaskMonitor( void *pvParameters );

#define pingPin 5
#define inPin 4
long duration, distance;

#define Rx 3
#define Tx 2
#include <SoftwareSerial.h>
SoftwareSerial UnoSerial(Rx, Tx);

void setup()
{
  pinMode(pingPin, OUTPUT);
  pinMode(inPin, INPUT);
  pinMode(Rx, INPUT);
  pinMode(Tx, OUTPUT);
  Serial.begin(115200);
  UnoSerial.begin(57600);


  while (!Serial) {
    ;
  }

  xTaskCreate(TaskReadSensor, "ReadSensor", 128, NULL, 1, NULL);
  xTaskCreate(TaskMonitor, "Monitor", 128, NULL, 1, NULL);
  xTaskCreate(TaskSendData, "SendData", 128, NULL, 1, NULL);
}

void loop() {}

void TaskReadSensor(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {
    Serial.println("-------------------------Arduino UNO-------------------------");
    Serial.println("Task ReadSensor is working");

    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);

    duration = pulseIn(inPin, HIGH);

    distance = (duration / 29) / 2;
    Serial.println(distance);
    vTaskDelay(3000 / portTICK_PERIOD_MS);

  }
}

void TaskMonitor(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {
    Serial.println("-------------------------Arduino UNO-------------------------");
    Serial.println("Task Monitor is working");
    Serial.print("Distance : ");
    Serial.print(distance);
    Serial.println(" cm");
    vTaskDelay(3000 / portTICK_PERIOD_MS);

  }
}

void TaskSendData(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {
    Serial.println("-------------------------Arduino UNO-------------------------");
    Serial.println("Task SendData is working");
    Serial.print("send >> ");
    Serial.println(distance);
    UnoSerial.print(distance);
    UnoSerial.print("\n");
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
}
