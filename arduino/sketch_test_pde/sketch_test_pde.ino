//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>
#include <string.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168, 2, 177);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 0, 0);

// create client
EthernetClient client;

// received string
int stringPos = 0;
// is reading check
boolean startRead = false; 
char fbLikes[4]; // string for incoming fb like

// connect to local server to fetch the data
char server[] = "192.168.2.199"; 
unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 5000;  // delay between updates, in milliseconds


// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup()
{
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);

  // init display
  lcd.begin(16, 2);              // start the library

  // startup application
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to....");
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print("Twinbit @ BOARD");
  delay(2000);
  if (Ethernet.localIP()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ethernet:");
    lcd.setCursor(0, 1);
    lcd.print(Ethernet.localIP());
    delay(2000);
  }
  // starting screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Twinbit - C3");
  lcd.setCursor(0, 1);
  lcd.print("Loading.......");
  httpRequest();
  delay(100);
}



// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 3000)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /api/likes HTTP/1.1");
    client.println("Host: www.arduino.cc");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}


void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:  
  if (client.available()) {
    char c = client.read();
    if (c == '<') { 
       startRead = true;
     }
     else if (startRead) {
       if (c != '>') {
         //Serial.print(c);
         fbLikes[stringPos] = c;
         stringPos++;
       }
       else {
         // got what we need here! We can disconnect now
         stringPos = 0;
         startRead = false;
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Twinbit - C3");
         lcd.setCursor(0, 1);
         lcd.print("Likes: ");
         lcd.setCursor(7, 1);
         lcd.print(fbLikes);
         client.stop();
         client.flush();
       }
     }
   }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}


