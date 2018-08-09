#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include "NTPtimeESP.h"

NTPtime NTPch("lt.pool.ntp.org"); 
strDateTime dateTime;

String host = "http://srb-middleware-dexter-lab.e4ff.pro-eu-west-1.openshiftapps.com";
String endPoint = "/v1/ra_data";

void POSTrequest(int meetingRoomStatus, String mac, String currentTime) {

  HTTPClient http;  //Declare object of class HTTPClient
     
  String json = "{\"data\":\"" + String(meetingRoomStatus)
	  + "\",\"mac\":\"" + mac + "\",\"time\":\"" + currentTime + "\"}";
  const char* jsonFinal = json.c_str();
   
  http.begin(host + endPoint);                          //Specify request destination
  http.addHeader("Content-Type", "application/json");   //Specify content-type header
   
  int request = http.POST((uint8_t *)jsonFinal, strlen(jsonFinal));    //Send the request
  delay(500);
  String response = http.getString();                                  //Get the response
     
  Serial.print("Request: "); Serial.println(jsonFinal);     //Print HTTP return code
  Serial.print("Response: ");  Serial.println(response);    //Print request response
   
  http.end();  //Close connection
}

String getMacAddress() {
  String myMac = "";
  byte mac[6]; 
  
  WiFi.macAddress(mac);
  for (int i = 0; i < 6; ++i) {
    myMac += String(mac[i], HEX);
    if (i < 5)
    myMac += ':';
  }

  return myMac;
}

String getTime() {
    String date = "1994-03-09 00:00:00";
    
    while(!dateTime.valid){
      dateTime = NTPch.getNTPtime(2, 1);
    } 
    
    byte hour = dateTime.hour;
    byte minute = dateTime.minute;
    byte second = dateTime.second;

    int year = dateTime.year;
    byte month = dateTime.month;
    byte day = dateTime.day;
    
    date = String(year)+"-"+String(month)+"-"+String(day)+" "+String(hour)+":"+String(minute)+":"+String(second);
    
    return date;
}

