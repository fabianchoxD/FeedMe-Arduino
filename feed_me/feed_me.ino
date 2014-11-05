// CONSTRUCTED'S Fish Feeder
// http://www.instructables.com/member/Constructed/

/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen
 
 */
#include <SPI.h>
#include <Ethernet.h>
#include <TextFinder.h>
#include <Servo.h>

String data;
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 



long FISHFEEDER = 300000; // 12 hours between feeding  43200000
long endtime; 
long now;
int automatic;

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Localhsot at PC (no DNS)
char server[] = "192.168.0.26";    // name address for Localhsot at PC (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,1,3);

int estado_automatico;
// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
TextFinder finder( client );
void setup() {

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  
  myservo.write(0);
  
 // Open serial communications and wait for port to open:
 
  if (Ethernet.begin(mac) == 0) {
   // Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  
  //Serial.println("connecting...");
  //Serial.begin(9600);
  
       

  // start the Ethernet connection:
  
}

void loop()
{
      now = millis();
       //Serial.println(now);
      endtime = now + FISHFEEDER;
 
      while(now < endtime) {
           myservo.write(0);
           // Serial.println(now);
     
       delay(3000);
       now = millis();   

  // if you get a connection, report back via serial:

     if (client.connect(server, 8080)) {
    //Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /FeedApp/Estado HTTP/1.1");
    client.println("Host: 192.168.0.26");
    client.println("Connection: close");
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    //Serial.println("connection failed");
  }
   if (client.available()) {
    char c = client.read();
    //Serial.print(c);
  }

      if (finder.find("{")) {
       long estado = finder.getValue();
      //Serial.print("estado: ");
      //Serial.println(estado);
      estado_automatico = estado;
      //Serial.println(estado_automatico);    
    
      //Condicionales para saber si hay petición desde el Celular
  // if the server's disconnected, stop the client:
client.stop();
client.flush();
  if (!client.connected()) {
    //Serial.println();
    //Serial.println("disconnecting.");
    client.stop();
    client.flush();
    // do nothing forevermore:
 
  } 
  if(estado_automatico==1){
        Feed();
        ChangeState();
     }
     
  }
  
  }
    Feed();
  
      }

    void Feed(){
     
      for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);    



                                      // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
    }
    void ChangeState(){
      data = "estado=0";      
           if (client.connect(server, 8080)) {
    //Serial.println("connected to servlet");
    // Make a HTTP request:
    client.println("POST /FeedApp/CambiarEstado HTTP/1.0");
    client.println("Host: 192.168.0.26");
    client.println("User-Agent: Arduino/1.0");
    
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);} 
  else {
    // kf you didn't get a connection to the server:
    //Serial.println("connection failed");
  }
  client.stop();
client.flush();
  if (!client.connected()) {
    //Serial.println();
    //Serial.println("disconnecting.");
    client.stop();
    client.flush();
    // do nothing forevermore:
 
  } 


    }
  
