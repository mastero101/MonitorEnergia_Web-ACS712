#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const int sensorIn = A0;
int mVperAmp = 120; // use 185 for 5A, 100 for 20A Module and 66 for 30A Module
int contconexion = 0;

double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;
double Watt = 0;
double Whr = 0;

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Mastero Wifi"
#define WIFI_PASSWORD "mastero101"

// Insert Firebase project API Key
#define API_KEY "AIzaSyARivOf5xIrVPcCuACeT6ZtK3mvT2gWKGM"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "1234@gmail.com"
#define USER_PASSWORD "123456"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://acs-712-powermeter-default-rtdb.firebaseio.com/"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String tempPath = "/Watt";
String humPath = "/AMP";
String presPath = "/VRMS";
String timePath = "/timestamp";

// Parent Node (to be updated in every loop)
String parentPath;

FirebaseJson json;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Variable to save current epoch time
int timestamp;

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 120000;


// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

void setup() 
{
pinMode(A0, INPUT);
Serial.begin(115200);
delay(10);

initWiFi();
timeClient.begin();

// Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";
}

float getVPP()
  {
  float result;
  
  int readValue; //value read from the sensor
  int maxValue = 0; // store max value here
  int minValue = 1024; // store min value here
  
  uint32_t start_time = millis();
  
  while((millis()-start_time) < 1000) //sample for 1 Sec
  {
  readValue = analogRead(sensorIn);
  // see if you have a new maxValue
  if (readValue > maxValue)
  {
  /*record the maximum sensor value*/
  maxValue = readValue;
  }
  if (readValue < minValue)
  {
  /*record the maximum sensor value*/
  minValue = readValue;
  }
  }
  
  // Subtract min from max
  result = ((maxValue - minValue) * 5)/1024.0;
  
    return result; 
}

void loop() {
 
    Voltage = getVPP();
    VRMS = (Voltage/2.0) *0.707; // sq root
    AmpsRMS = (VRMS * 1000)/mVperAmp;
    Watt = (120*AmpsRMS)-3.14;
    Whr = Watt/3600;
    float Wattage = (120*AmpsRMS)-5.6; //Observed 18-20 Watt when no load was connected, so substracting offset value to get real consumption.
    Serial.print(AmpsRMS);
    Serial.println(" Amps");
    Serial.print(Watt);
    Serial.println(" Watt ");

        // Send new readings to database
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    //Get current timestamp
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);

    parentPath= databasePath + "/" + String(timestamp);

    json.set(tempPath.c_str(), String(Watt));
    json.set(humPath.c_str(), String(AmpsRMS));
    json.set(presPath.c_str(), String(VRMS));
    json.set(timePath, String(timestamp));
    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
  }
 }
