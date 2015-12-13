#include <SoftwareSerial.h>
 #include "DHT.h"          /****** HUMEDAD Y TEMPERATURA ******/
 #define DHTPIN 9   
 #define DHTTYPE DHT11   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
int h;
int t;
float uvindex=0;          /****** UV ******/ 
float CO, COV, OZON,HS;     /****** GASES ******/ 
float MP;                   /****** MP ******/
int measurePin = A5;
           
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
       
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
 //Hardware pin definitions  MP8511
int UVOUT = A0; //Output from the sensor
int REF_3V3 = A1; //3.3V power on the Arduino board
SoftwareSerial sim800l(7,8); //RX - TX
int circ=22, heat=23, alert=6;   /****** LEDS ******/
 
 /***** MODULO GPS ******/ 
      
SoftwareSerial GPS(2, 3); // RX, TX
      
#include "TinyGPS.h"
TinyGPS gps;
    
#define GPS_TX_DIGITAL_OUT_PIN 2
#define GPS_RX_DIGITAL_OUT_PIN 3
 #define MAX_BUFFER 500
     
unsigned long momento_anterior=0;
unsigned long bytes_recibidos=0;
     
long startMillis;
long secondsToFirstLocation = 0;
     
float latitude = 0.0;
float longitude = 0.0;
   
char latit[12];
char longi[12];


void setup() {
  
Serial.begin(19200);  //Configuracion de puerto serial por hardware en bps(baudios por segundo)
sim800l.begin(19200); //Configuracion de puerto serial del modulo (baudios por segundo)
GPS.begin(9600); //Configuracion de puerto serial del modulo (baudios por segundo)

Serial.println("Iniciando configuracion del modulo GSM"); 
sim800l.println(F("AT"));
delay(500);
Serial.println(debug());
sim800l.println(F("AT+CBC"));  //Retorna el estado de la bateria del dispositivo, el % y milivol
delay(500);
Serial.println(debug());
sim800l.println(F("AT+IPR=19200"));  //Retorna el estado de la bateria del dispositivo, el % y milivol
delay(500);
Serial.println(debug());

sim800l.println(F("AT+CSQ")); // Retorna la calidad de la señal que depende de la antena y la localizacion
delay(500);
Serial.println(debug());      
       dht.begin();
       pinMode(UVOUT, INPUT);
       pinMode(REF_3V3, INPUT);
       pinMode(circ, OUTPUT);
       pinMode(heat, OUTPUT); 
       pinMode(alert, OUTPUT);       
configuracionGPRS();
Serial.print("Configuracion finalizada.\r\n");
//enviardatos();

}

// the loop function runs over and over again forever
void loop() {

    
GetHT();
GetOzono();
GetUV();
GetCO();
GetCOV();    
GetHS();
GetGPS();
enviardatos();
//Serial.print("Envio de datos finalizado.\r\n");
  
}


void GetHT() {       
     h = dht.readHumidity();
     t = dht.readTemperature();
     Serial.print("Temperatura: ");
     delay(500);  
     Serial.print(t);
     delay(500);  
     Serial.print("C"); 
     delay(500);  
       Serial.print("\n ");
     Serial.print("Humedad: ");
     delay(500);  
     Serial.print(h);
     delay(500);  
     Serial.print("% ");       
     delay(500); 
      Serial.print("\n "); 
       }
       
       
void GetHS() {           
     int val2 = analogRead(A2);
     HS = val2 * (5.0 / 1023.0);
     Serial.print("Humedad Suelo:");
     delay(1000);  
     Serial.print(HS);
     delay(1000);  
     Serial.print("\n ");    
       }       
   
void GetUV() {         
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);
  
  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;
  
volatile  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);

//  Serial.print("Nivel MP8511:");
//  Serial.print(uvLevel);
//  delay(1000);
//  Serial.print("\n ");
//  Serial.print(" Voltaje MP8511 : ");
//  Serial.print(outputVoltage);
//  delay(1000);
//  Serial.print("\n ");
  Serial.print("Intensidad (mW/cm^2): ");
  Serial.print(uvIntensity);
  delay(500);
  Serial.print("\n ");
  delay(500);
       }
       
       
       
void GetCO() {     //MQ 7 Sensor de calidad de aire , idenfica cantidad de monoxido de carbono
     float val1 = analogRead(A3);
     CO = val1 * (5.0 / 1023.0);
     Serial.print("CO:");
     delay(500);  
     Serial.println(CO);      
     delay(500);  
       }
       
void GetCOV() {    //MQ 135 control de calidad de aire
     float val3 = analogRead(A4);
     COV = val3 * (5.0 / 1023.0); 
     Serial.print("COV:");
     delay(500);  
     Serial.println(COV);      
     delay(500);  
       
       }
void GetOzono() {    //MQ 135
     float val4 = analogRead(A5);
     OZON = val4 * (5.0 / 1023.0); 
     Serial.print("Ozono:");
     delay(500);  
     Serial.println(OZON);      
     delay(500);  
       
       }  




//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}

//The Arduino Map function but for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


char *debug()  // devuelve el ``contenido de un objeto apuntado por un apuntador''. 
{
int i=0;
char cad[255]="\0";
char c='\0';
        
        strcpy(cad,"");
        while(sim800l.available()>0)
        {
        c=sim800l.read();
        cad[i]=c;
        i++;
        }
      
return cad;
}
 
void configuracionGPRS() {
sim800l.println(F("AT+CREG=1")); // Verifica si la simcard a sido o no registrada
Serial.println(debug());
delay(500);
sim800l.println(F("AT+CIPSHUT")); // Resetea las direcciones IP
Serial.println(debug());
delay(500);
sim800l.println(F("AT+CGATT=1")); // Verifica si el gprs esta activo o no
Serial.println(debug());
delay(500);
sim800l.println(F("AT+CIPSTATUS")); //Verifica si la pila o stack IP es inicializada
Serial.println(debug());
delay(500);
sim800l.println(F("AT+CIPMUX=0")); //Esta la conexión en modo simple(udp/tcp cliente o tcp server)
Serial.println(debug());
delay(500);

// Configurar tarea y configura el APN
sim800l.println(F("AT+CSTT=\"internet.comcel.com.co\",\"COMCELWEB\",\"COMCELWEB\""));
Serial.println(debug());
delay(500);

sim800l.println(F("AT+CIICR")); // Levantar conexión wireless(GPRS o CSD)
Serial.println(debug());
delay(500);


}     

void enviardatos() {  //Se acitva si el peso se encuentra en un limite definido
//sim800l.println(F("AT+CIPSHUT")); //Resetea las direcciones IP
//Serial.println(debug());
//delay(500);
sim800l.println("AT+CIFSR"); // Obtiene una dirección IP
Serial.println(debug());
delay(2000);

sim800l.println(F("AT+CIPSTART=\"TCP\",\"107.170.208.9\",\"80\"")); //Inicia conexión UDP o TCP
Serial.println(debug());
delay(2000);

sim800l.println(F("AT+CIPSEND\r\n")); // Envia datos al servidor remoto, ctlr+z o 0x1A,
//verifica que los datos salieron del puerto serial pero no indica si llegaron al servidor UDP
Serial.println(debug());
delay(500);
//sim800l.println("GET /dato.php?temp="+ String(t)+"&hum="+ String(h)+"&uv="+ String(t)+"&co="+ String(CO)+"&cov="+ String(COV)+"&hs="+ String(HS)+"&gpslat="+String(latitude)+"&gpslog="+ String(longitude)); 
sim800l.println("GET /index.php?data=N0FIQXVGSnFZMWF6WUx3VkVMbnhRYjhRZE1uNXRTY3JpOGVQTUVobEtRMWlPYmo4TVdzMENwMTROaWloUnllOGNDeW8wZjhwWm1lN29Fc1I1QmprbHk4bFhBPT0&id_dispositivo=1&temp="+ String(t)+"&hum="+ String(h)+"&uv="+ String(t)+"&co="+ String(CO)+"&cov="+ String(COV)+"&hs="+ String(HS)+"&gpslat="+String(latitude)+"&gpslog="+ String(longitude)); 

//+"&co="+ String(CO)+"&cov="+ String(COV)+"&hs="+ String(HS)
//sim800l.println("GET /dato.php?temp="+ String(t)+"&hum="+ String(h)); 
//sim800l.println(F("GET /dato.php?temp=200&hum=200&ozon=100&uv=100")); 
// Se envia por un peticion GET los valores obtenidos
//Serial.println(debug());
//delay(500);
pushSlow("\r\n",100,100); //Envia un salto de linea
pushSlow("\x1A",100,100);//ctlr+z para finalizar el envio o 0x1A
//sim800l.write(0x1A);//ctlr+z para finalizar el envio o 0x1A
Serial.println(debug());
delay(500);
sim800l.println(F("AT+CIPSHUT")); //Resetea las direcciones IP
Serial.println(debug());
delay(4000);
//pesomin=0;
}


void GetGPS() {        
     
bool newData = false;
unsigned long chars = 0;
unsigned short sentences, failed;

 for (unsigned long start = millis(); millis() - start < 1000;)
      {
           while (GPS.available())
            {
                 int c = GPS.read();
                 ++chars;
               if (gps.encode(c)) 
                 newData = true;
            }
      }
      
if (newData) {
        if(secondsToFirstLocation == 0){
          secondsToFirstLocation = (millis() - startMillis) / 1000;         
        }
        
unsigned long age;
gps.f_get_position(&latitude, &longitude, &age);       
        
Serial.print("Localizacion: ");
Serial.print(latitude, 6);
Serial.print(" , ");
Serial.print(longitude, 6);
Serial.println("");
  
    }  
        
dtostrf(latitude, 6, 6, latit); 
dtostrf(longitude, 6, 6, longi);
//Serial.println(latit);
//Serial.println(longi);
        
      
  }
   // Envia datos por el SoftSerial
// lentamente
void pushSlow(char* command,int charaterDelay,int endLineDelay) {
  for(int i=0; i<strlen(command); i++) {
    sim800l.write(command[i]);
    if(command[i]=='\n') {
      delay(endLineDelay);
    } else {
      delay(charaterDelay);
    }
  }
} 
