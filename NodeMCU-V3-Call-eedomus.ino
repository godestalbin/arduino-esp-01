//Validate how long we need to power the sensor before getting an accurate reading
//After 2sec the reading starts to be accurate
//Reading 0 after 0.5 secondsSoil humidity: rawValue=330, value=100
//Reading 1 after 1 secondsSoil humidity: rawValue=657, value=59
//Reading 2 after 1.5 secondsSoil humidity: rawValue=687, value=54
//Reading 3 after 2 secondsSoil humidity: rawValue=708, value=51
//Reading 4 after 2.5 secondsSoil humidity: rawValue=717, value=50
//Reading 5 after 3 secondsSoil humidity: rawValue=726, value=48
//Reading 6 after 3.3 secondsSoil humidity: rawValue=729, value=48
//Reading 7 after 4 secondsSoil humidity: rawValue=734, value=47
//Reading 8 after 4.5 secondsSoil humidity: rawValue=733, value=47
//Reading 9 after 5 secondsSoil humidity: rawValue=737, value=46

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//Constants 
const char* ssid = "Livebox-3074"; // remplacer par le SSID de votre WiFi
const char* password = "HGLXeNXcseWcCePCtG"; // remplacer par le mot de passe de votre WiFi
const int hygrometer = A0;  //Hygrometer sensor analog pin output at pin A0 of Arduino
const int powerSensor = 1; //D1;

//Variables 
int rawValue;
int value;

void setup(){
  Serial.begin(115200);
  Serial.println(""); //CR after reboot garbages
  Serial.println("Waking up"); 

  //Power Sensor
  pinMode(powerSensor, OUTPUT);           // set pin to output
  digitalWrite(powerSensor, HIGH);       // turn on pullup resistors

  //Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
  delay(1000);

  // When the plant is watered well the sensor will read a value 380~400, I will keep the 400 
  // value but if you want you can change it below. 

  //for (int i=0; i<10; i++) {
    //Serial.printf("Reading %d after %f seconds\0", i+1, (i*500)/1000); 
    rawValue = analogRead(hygrometer);   //Read analog value 
    value = rawValue;
    value = constrain(value,400,1023);  //Keep the ranges!
    value = map(value,400,1023,100,0);  //Map value : 400 will be 100 and 1023 will be 0
    Serial.printf("Soil humidity: rawValue=%d, value=%d\n", rawValue, value);
  
    //delay(500);
  //}

  //Update sensor on eedomus
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient

  String httpRequest = "http://api.eedomus.com/set?action=periph.value&periph_id=1123471&value=" + String(value) + "&api_user=G34fOz&api_secret=C7B12v2n97fAd97C";
    http.begin(httpRequest);  //Specify request destination
    Serial.println(httpRequest);
    int httpCode = http.GET(); //Send the request

    Serial.printf("httpCode=%d\n", httpCode); 
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
    }
    http.end();   //Close connection
  }

  Serial.println("Going into deep sleep for 10 secs - 5 minutes");
  ESP.deepSleep(10e6); // 300e6 = 320 seconds = 5min
}

void loop() {
}
