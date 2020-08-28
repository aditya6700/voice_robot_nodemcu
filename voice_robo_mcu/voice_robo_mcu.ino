#include <ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "your ssid";
const char* password = "your password"
String  command =""; // Command received from Android device

//  Motor Control Pins
int rp = 13;   // D7
int rm = 15;  // D8
int lp = 0;  // D3
int lm = 2;  // D4

//HC-SR04
const int trigPin = 12;  // D6
const int echoPin = 14;  // D5
long duration;
int distance;

#include <Servo.h>
Servo ser_r;
Servo ser_l;

int pos = 0;

void setup()
{
  Serial.begin(9600);

  connectWiFi();
  server.begin();
  
  pinMode(rp, OUTPUT);
  pinMode(rm, OUTPUT);
  pinMode(lp, OUTPUT);
  pinMode(lm, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  digitalWrite(rp,LOW);
  digitalWrite(rm,LOW);
  digitalWrite(lp,LOW);
  digitalWrite(lm,LOW);

 ser_r.attach(5);   //D1
 ser_l.attach(4);   //D2
      
  
}

void loop()
{
    client = server.available();
    if (!client) return; 
    command = checkClient ();

       if (command == "forward" || command == "go front" || command == "come forward") 
         fwd();
       else if (command == "reverse" || command == "come back"|| command == "go back") 
         rev();
       else if (command == "left"    || command == "go left"   || command == "go left")
         lef();    
       else if (command == "right"   || command == "go right"  || command == "turn right") 
         rih();     
       else if (command == "stop"    || command == "aagu" || command == "agu")     
         st0p();
       else if (command == "hands up"    || command == "up")     
         hu();
       else if (command == "hands down"    || command == "down")     
         hd();         
     else{
      
    sendBackEcho(command); // send command echo back to android device
    command = "";   

    
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance = 25)
    st0p();
     }
} 

void fwd()
{
  // forward
  digitalWrite(rp, HIGH);
  digitalWrite(rm, LOW);
  digitalWrite(lp, HIGH);
  digitalWrite(lm, LOW);
}

void rev()
{
  // reverse
  digitalWrite(rm, HIGH);
  digitalWrite(rp, LOW);
  digitalWrite(lm, HIGH);
  digitalWrite(lp, LOW);
}

void rih()
{
  // right
  digitalWrite(rp, LOW);
  digitalWrite(rm, HIGH);
  digitalWrite(lp, HIGH);
  digitalWrite(lm, LOW);
}

void lef()
{
  // left
  digitalWrite(rp, HIGH);
  digitalWrite(rm, LOW);
  digitalWrite(lp, LOW);
  digitalWrite(lm, HIGH);
}

void st0p()
{
  digitalWrite(rp, HIGH);
  digitalWrite(rm, HIGH);
  digitalWrite(lp, HIGH);
  digitalWrite(lm, HIGH);
}


void hu()
{
   
    ser_r.write(pos=120);
    ser_l.write(pos=120);
     
}


void hd()
{
      
      ser_r.write(pos=0);
      ser_l.write(pos=0);
      
}



/* connecting WiFi */
void connectWiFi()
{
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
}

/* check command received from Android Device */
String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}

/* send command echo back to android device */
void sendBackEcho(String echo)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println(echo);
  client.println("</html>");
  client.stop();
  delay(1);
}
