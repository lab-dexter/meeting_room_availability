// Import required libraries
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "LiquidCrystal_I2C.h"

// Construct an LCD object and pass it the 
// I2C address, width (in characters) and
// height (in characters). Depending on the
// Actual device, the IC2 address may change.
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// WiFi parameters
const char* ssid = "<SSID NAME>";
const char* password = "<PASSWORD>";

// Define 2 lines what to display on LCD screen
String line_1;
String line_2;

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
ESP8266WebServer server(LISTEN_PORT);

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Print the IP address
  Serial.println(WiFi.localIP());

  server.on("/", handleRootPath);
  server.on("/free", room_free);
  server.on("/in_use", room_in_use);
  server.begin();
  Serial.println("Server Listening");

  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(16,2);
  lcd.init();

  // Turn on the backlight.
  lcd.backlight();

  // Print initial text on LCD screen
  LCD_print("NO", "INFO");
}

void loop() {
  server.handleClient();
}

void handleRootPath(){
  server.send(200, "text/plain", "Hi");
  LCD_print("NO", "INFO");
}

void room_free(){
  server.send(200, "text/plain", "Room marked as free");
  LCD_print("ROOM", "IS FREE");
}

void room_in_use(){
  server.send(200, "text/plain", "Room marked as in use");
  LCD_print("ROOM", "IS IN USE!");
}

void LCD_print(String line_1, String line_2){
  lcd.clear();
  
  // Move the cursor characters to the right and
  // zero characters down (line 1).
  lcd.setCursor(5, 0);

  // Print line_1 to the screen, starting at 5,0.
  lcd.print(line_1);

  // Move the cursor to the next line and print line_2
  lcd.setCursor(5, 1);      
  lcd.print(line_2);
}







