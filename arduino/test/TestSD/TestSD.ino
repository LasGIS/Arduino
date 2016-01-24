/*
 * SD карта подключается так:
 ** MOSI - пин 11
 ** MISO - пин 12
 ** CLK - пин 13
 ** CS - пин 4
*/
#include <SD.h>;
 
File myFile;
 
void setup()
{
  Serial.begin(9600);
   while (!Serial) {
    ; //Ждём инициализации серийного порта. Нужно для Leonardo
  }
 
  Serial.print("Initializing SD card...");
  // Магия. Этот вывод должен быть настроен как выход.
  // Иначе, некоторые функции могут не работать.
  pinMode(10, OUTPUT);
 
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
 
  if (SD.exists("example.txt")) {
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");
  }
 
  //Откроем новый файл и сразу же закроем его:
  Serial.println("Creating example.txt...");
  myFile = SD.open("example.txt", FILE_WRITE);
  myFile.close();
 
  //Проверяем, создан ли файл:
  if (SD.exists("example.txt")) {
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");
  }
 
  //Удаляем файл:
  Serial.println("Removing example.txt...");
  SD.remove("example.txt");
 
  if (SD.exists("example.txt")){
    Serial.println("example.txt exists.");
  } else {
    Serial.println("example.txt doesn't exist.");
  }
}
 
void loop(){
  //Тут ничего не происходит
}
