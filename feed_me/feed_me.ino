// CONSTRUCTED'S Fish Feeder  (Base Code)
// http://www.instructables.com/member/Constructed/

/* Web client

 This sketch connects to a localHost 
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen
 
 */
 
 /* Code V 2.0 Whit Tweet and Android Management Developed by 
 
 Joan Sebastian Sanchez 
 https://www.facebook.com/Joansebastian.sanchez    
 https://github.com/setoba1192
 
=========================================================================
 Fabian Stevens Varon     
 https://www.facebook.com/fabian.varon  
 https://github.com/fabianchoxD

 Universidad Surcolombiana 
 Tecnología en Desarrollo de Software
 Neiva - Huila  Colombia 
 2014 - 2 
 */

//====== Etherned libraries ===============

#include <SPI.h>
#include <Ethernet.h>
#include <TextFinder.h>

/* REMIND

Download the Arduino Tweet and TextFinder Libraries and put Twitter folder into
~/Documents/Arduino/libraries (Mac) 
or 
My Documents\Arduino\libraries\ (Windows). 

If there is no such folder, please create it. */


//========= Twitter  Libraries and Token ===========

#include <Twitter.h>
Twitter twitter("2785210158-0Zla9y89bHyb7hLLNlHh1K0QbTviSo3WVuYWTHT");

//======= Control Variables ========
int acumulador=0;
char milisegundos;
String data;

// ============= Servo Declaration ===================
#include <Servo.h>
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created  
int pos = 0;    // variable to store the servo position 


// ================== Feed Control =========================

long FISHFEEDER = 600000; //10 min between Feeding   // to 12 hours Between Feed .... FISHFEEDER = 43200000
long endtime;  // end time to Next Feed
long now;      // Works to Accumulate Time Since Adruino was Mode ON
int automatic; // Change the state at Data Base...  0 or 1 


// ============== Ethernet Settings ===============
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Localhsot at PC (no DNS)

char server[] = "192.168.0.26";    // name address for Localhsot at PC (using DNS)
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,1,3);

int automatic_state; // boolean ... if  == 1 execute feed and change the state of Database

/* Initialize the Ethernet client library
/with the IP address and port of the server 
 that you want to connect to (port 80 is default for HTTP):*/
EthernetClient client;  // Create a object client extends from Library Ethernet
TextFinder finder( client );  // Create a object finder extend from library Text Finder  And usea as parameter client.


// ================================ Void Setup ============================

void setup() {

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object   
  myservo.write(0);   // put servo at 0 position.
  
 // Open serial communications and wait for port to open: 
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:    
  // start the Ethernet connection:  
  Serial.println("connecting...");
  // Start Serial communication
  Serial.begin(9600);       
}

// ================== void loop ===========================

void loop()
{    
      now = millis();
      Serial.println(now);
      endtime = now + FISHFEEDER;
      milisegundos=now;
        while(now < endtime) {
           myservo.write(0);
            Serial.println(now);
     
       delay(3000);
       now = millis();   
       
  // if you get a connection, report back via serial:
  
    if (client.connect(server, 8080)) {
    Serial.println("connected");
    
    // Make a HTTP request:
    client.println("GET /FeedApp/Estado HTTP/1.1"); // server localhost url
    client.println("Host: 192.168.0.26"); // ip 
    client.println("Connection: close");
    client.println();
  } 
  else {
    
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
    // if connection successful create a Char c to read data from client.
   if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
      if (finder.find("{")) {
       long estado = finder.getValue();
      Serial.print("estado: ");
      Serial.println(estado);
      automatic_state = estado;
      Serial.println(automatic_state);     
      //Condicionales para saber si hay petición desde el Celular   
      // if the server's disconnected, stop the client:
      client.stop();
      client.flush();
        if (!client.connected()) {
          Serial.println();
          Serial.println("disconnecting.");
          client.stop();
          client.flush();
    // do nothing forevermore: 
  } 
  if(automatic_state==1){
        Feed();
        ChangeState();
        Publish0();
       }     
     }  
  }
    Feed();
    Publish1();
   }
   
//=================== void Methods =======================

    /* create a void method to feed   
    ( move the servo from 0 to 180 and then return to 0) */
  
    void Feed(){     
      for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
    {                                    // in steps of 1 degree 
      myservo.write(pos);    
                                         // tell servo to go to position in variable 'pos' 
      delay(15);                         // waits 15ms for the servo to reach the position 
    } 
      for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
    {                                
      myservo.write(pos);                // tell servo to go to position in variable 'pos' 
      delay(15);                         // waits 15ms for the servo to reach the position 
    } 
 }
 
   /* Create a void method to send a request at Servlet
   and Change the Database State*/
   
    void ChangeState(){
      data = "estado=0";      
           if (client.connect(server, 8080)) {
    Serial.println("connected to servlet");
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
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  client.stop();
client.flush();
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    client.flush();
  } 
    }
    
// Create a Void method to send a twitter message Option 1  

  void Publish0(){    
    String stringOne = "Thanks For Feed Me Manually My Lords @fabianchoxd @setoba1192 at ";
     stringOne += millis();
          stringOne += "Ms";     
        Serial.println(stringOne);        

    char charBuf[stringOne.length()+1];
      stringOne.toCharArray(charBuf, stringOne.length()+1) ;

            if (twitter.post(charBuf)) {
              int status = twitter.wait();
                if (status == 200) {
                   Serial.println("OK.");
                  } else {
              Serial.print("failed : code ");
              Serial.println(status);
              }
            } else {
          Serial.println("connection failed.");
         }
      }
      
// Create a Void method to send a twitter message Option 2

void Publish1(){
  String stringOne = "Thanks For Feed Me @arduino at ";
     stringOne += millis();
     stringOne += "Ms";
        Serial.println(stringOne);        

    char charBuf[stringOne.length()+1];
stringOne.toCharArray(charBuf, stringOne.length()+1) ;

            if (twitter.post(charBuf)) {
                int status = twitter.wait();
                  if (status == 200) {
                    Serial.println("OK.");
                  } else {
                Serial.print("failed : code ");
              Serial.println(status);
            }
          } else {
        Serial.println("connection failed.");
      }
    }

