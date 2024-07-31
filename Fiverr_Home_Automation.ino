#include <Arduino.h>
//#if defined(ESP32)
 // #include <WiFi.h>
//#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
//#endif
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "" // firebase API Key
#define DATABASE_URL "" // firebase DB URL
#define WIFI_SSID ""                                          
#define WIFI_PASSWORD ""

#define LDR_PIN A0
//Define Firebase Data object

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int ldrData = 0;
bool signupOK = false;
float voltage = 0.0;

String sValue, sValue2;
int rly1 = 16; //PIN D0 NODEMCU gen purpose rly 
int rly2 = 5; //PIN D1 NODEMCU gen purpose rly
int rly3 = 13; //PIN D7 NODEMCU hi power rly
int rly4 = 15; //PIN D8 NODEMCU hi power rly
const char* ssid = "";
const char* password = "";

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  
  Serial.begin(115200);
  pinMode(rly1,OUTPUT);
  pinMode(rly2,OUTPUT);
  pinMode(rly3,OUTPUT);
  pinMode(rly4,OUTPUT);
  /*
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");*/
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("signup ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);


}

void loop() {
  if (Serial.available()) {
    // Read the number of elements in the float array
    int numElements = Serial.parseInt();
    Serial.read(); // Read and discard the comma

    // Create an array to store the float values
    float receivedArray[numElements];

    // Read each float value from the serial buffer
    for (int i = 0; i < numElements; i++) {
      receivedArray[i] = Serial.parseFloat();
      Serial.read(); // Read and discard the comma
    }

    // Process the received float array
    for (int i = 0; i < numElements; i++) {
      Serial.print("Current Sensor");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(receivedArray[i]);
    }
  }
  
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    ldrData = analogRead (LDR_PIN);
    voltage = analogRead(LDR_PIN);
  //  voltage = (float)analogReadMilliVolts(voltage)/1000;
     if (Firebase.RTDB.setInt(&fbdo, "Sensor/ldr_data", ldrData))
      {Serial.println(); Serial.print(ldrData);
      Serial.print(" successfully saved to:" + fbdo.dataPath());
      Serial.println("(" + fbdo.dataType() + ")"); }
      else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

     if (Firebase.RTDB.setFloat(&fbdo, "Sensor/voltage", voltage))
      {Serial.println(); Serial.print(voltage);
      Serial.print(" successfully saved to:" + fbdo.dataPath());
      Serial.println("(" + fbdo.dataType() + ")"); }
      else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}
if (Firebase.ready() && signupOK ) {
    if (Firebase.RTDB.getString(&fbdo, "/R1")) {
      if (fbdo.dataType() == "string") {
        sValue = fbdo.stringData();
        int a = sValue.toInt();
        Serial.println(a);
        if (a == 1){
          digitalWrite(rly1,HIGH);}
        else
          {digitalWrite(rly1,LOW);
        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
    
    if (Firebase.RTDB.getString(&fbdo, "/R2")) {
      if (fbdo.dataType() == "string") {
        sValue2 = fbdo.stringData();
        int b = sValue2.toInt();
        Serial.println(b);
        if (b == 1){
          digitalWrite(rly2,HIGH);
        }else{
          digitalWrite(rly2,LOW);
        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }

      if (Firebase.RTDB.getString(&fbdo, "/R3")) {
      if (fbdo.dataType() == "string") {
        sValue2 = fbdo.stringData();
        int b = sValue2.toInt();
        Serial.println(b);
        if (b == 1){
          digitalWrite(rly3,HIGH);
        }else{
          digitalWrite(rly3,LOW);
        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
          if (Firebase.RTDB.getString(&fbdo, "/R4")) {
      if (fbdo.dataType() == "string") {
        sValue2 = fbdo.stringData();
        int b = sValue2.toInt();
        Serial.println(b);
        if (b == 1){
          digitalWrite(rly4,HIGH);
        }else{
          digitalWrite(rly4,LOW);
        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
  }

}
