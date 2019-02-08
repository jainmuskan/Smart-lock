#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "tqb-WiFi";
const char* password =  "thingQbator123";
const char* mqttServer = "192.168.1.147";
const int mqttPort = 1883;
const char* mqttUser = "muskan";
const char* mqttPassword = "muskan";
const char* topic = "test";
const int buzzer = D6;
const int PIEZO_PIN = A0;
const int lock= D1;
int i;
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  pinMode(buzzer,OUTPUT);
  pinMode(lock,OUTPUT);
analogWrite(buzzer,0);
digitalWrite(lock,0);
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("test", "Hello from ESP8266");
  client.subscribe("test");
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

   if((char)payload[0]=='O'&& (char)payload[1]=='N')
    {
      digitalWrite(lock,1);
      delay(7000);
      digitalWrite(lock,0);
    }

    else if((char)payload[0]=='C' && (char)payload[1]=='L')
    {
      digitalWrite(lock,1);
      delay(4000);
      digitalWrite(lock,0);
      }
 
 
  Serial.println();
  Serial.println("-----------------------");
 
}

 
void loop() {
  client.loop();
  int old = analogRead(PIEZO_PIN);
  delay(5);
  int next= analogRead(PIEZO_PIN);
  int difference=old-next;
  Serial.println(difference); // Print the voltage.
  if(difference<=-100||difference>=100)
  {
    analogWrite(buzzer,120);
    client.publish(topic, "breakin");          //Publish to MQTT client
    delay(3000);
    }
    analogWrite(buzzer,0); 
      }
    //}
    
