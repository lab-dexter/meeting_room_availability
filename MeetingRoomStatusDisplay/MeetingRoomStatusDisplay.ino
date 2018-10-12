/**
 *  @filename   :   epd2in7b-demo.ino
 *  @brief      :   2.7inch e-paper display (B) demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     July 17 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <SPI.h>
#include <epd2in7b.h>
#include "imagedata.h"
#include <epdpaint.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "testingPOST.h"

// WiFi parameters
const char* ssid = "BarclaysWiFi";
String mac;
String date;
Epd epd;

  /**
    * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
    * In this case, a smaller image buffer is allocated and you have to 
    * update a partial display several times.
    * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
    */
unsigned char image[1024];
Paint paint(image, 176, 24);    //width should be the multiple of 8 
  
// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80
#define COLORED     1
#define UNCOLORED   0

// Create an instance of the server
ESP8266WebServer server(LISTEN_PORT);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  serverSetup();
 
  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed");
    return;
  }

  /* This clears the SRAM of the e-paper display */
  epd.ClearFrame();
}

void serverSetup() {
  // Connect to WiFi
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //Check WiFi connection status
  if(WiFi.status() == WL_CONNECTED) {         
    //mac = "aa:aa:aa:aa:aa:aa";
    mac = getMacAddress();
    Serial.print("MAC: "); Serial.println(mac);
  
    //date = "1994-03-09 00:00:00";
    date = getTime();
    Serial.print("TIME: "); Serial.println(date);
    
    Serial.println(""); Serial.println("");
  }
  else 
  {
    Serial.println("Error in WiFi connection");   
  }
  
  // Print the IP address
  Serial.println(WiFi.localIP());

  server.on("/", handleRootPath);
  server.on("/free", room_free);
  server.on("/in_use", room_in_use);
  server.begin();
  Serial.println("Server Listening");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

void handleRootPath(){
  server.send(200, "text/plain", "Hi");
}

void room_free(){
  Serial.println("Room is free");
  server.send(200, "text/plain", "Room marked as free");
  
  epd.ClearFrame();
  paint.SetWidth (160);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "Room is free", &Font16, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), 30, 60, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);
  paint.SetHeight(64);
  paint.Clear(UNCOLORED);

  paint.DrawFilledRectangle(0, 0, 50, 50, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), 60, 120, paint.GetWidth(), paint.GetHeight());
  POSTrequest(0, mac, date);
  
  epd.DisplayFrame();
}

void room_in_use(){
  Serial.println("Room in use");
  server.send(200, "text/plain", "Room marked as in use");
  
  epd.ClearFrame();
  paint.SetWidth (160);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "Room in use", &Font16, COLORED);
  epd.TransmitPartialRed(paint.GetImage(), 30, 60, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);
  paint.SetHeight(64);
  paint.Clear(UNCOLORED);
  
  paint.DrawFilledCircle(32, 32, 30, COLORED);
  epd.TransmitPartialRed(paint.GetImage(), 60, 120, paint.GetWidth(), paint.GetHeight());
  POSTrequest(1, mac, date);
  
  epd.DisplayFrame();
}

