void LEDflash(){
 if(MQTTdisconnect != 1){ 
    unsigned long currentMillis = millis();

  if (currentMillis - LEDpreviousMillis >= LEDinterval) {
    // save the last time you blinked the LED
    LEDpreviousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(8, ledState);
    }
  }
}

void start_mqtt (){
  client.setServer(server, 1883);
  client.setCallback(callback);
  lastReconnectAttempt = 0;
}

boolean reconnect() {
 if (client.connect("wsAlarm",LWT , 1 , 1 , "Offline")){
//   if (client.connect("workShopAlarm")) {
    // Once connected, publish an announcement...
    //Serial.println("MQTT Connected");
    client.publish(LWT,"Online", true);
    connect_timeout = 0 ;
    MQTTdisconnect = 1 ;
    // ... and resubscribe
    client.subscribe(cmdTopic);
    digitalWrite(8, HIGH);
    Serial.println("MQTT Connected");
  }
  return client.connected();
}

void reset_arduino_mqtt (){
 
  if (connect_timeout > 5){
    //Serial.println(connect_timeout);
    wdt_enable(WDTO_2S);
    delay (3000);
  }
}


void check_mqtt (){
  if (!client.connected()) {
    MQTTdisconnect = 0 ;  
    //digitalWrite(8, LOW);  
    long now = millis();
 
    if (now - lastReconnectAttempt > 3000) {
      //digitalWrite(8, HIGH);
      connect_timeout ++ ;
      Serial.println(connect_timeout);
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // Client connected
    //digitalWrite(8, HIGH);
    //Serial.println("MQTT Connected");
    //client.subscribe(statTopic);
    client.loop();
  }
}

// convert IP-address to a string for in this case MQTT
char* ip2CharArray(IPAddress IP) {
  static char a[16];
  sprintf(a, "%d.%d.%d.%d", IP[0], IP[1], IP[2], IP[3]);
  return a;
}

void sendStatusData(byte stat){
  Serial.print("MQTT Command Recv:");
  Serial.println(stat);

  
  if (stat == 49){ // 49 is the byte version of 1
  // Serial.println("Case 1 Run");
    StaticJsonDocument<100> doc;
      JsonObject Status  = doc.createNestedObject("Status");  
      Status["Module"] = 1;
      JsonArray  FriendlyName  = Status.createNestedArray("FriendlyName");
      FriendlyName.add(Fname);
       Status["Topic"] = topic;  
      char buffer[80];
      serializeJson(doc, buffer);
      size_t n = serializeJson(doc, buffer);
      client.publish(statTopic, buffer,n);
     //Serial.print("freeMemory()=");
   // Serial.println(freeMemory());
    //doc.clear();
    checkMemory();
  } else if (stat == 53){ // is the byte version of 5
      //Serial.println("Case 5 Run");
      StaticJsonDocument<50> doc5;
      JsonObject Status5  = doc5.createNestedObject("StatusNET");  
      Status5["IPAddress"] =ip2CharArray(Ethernet.localIP());
      Status5["Mac"] = macS;
      char buffer5[75];
      serializeJson(doc5, buffer5);
      size_t n5 = serializeJson(doc5, buffer5);
      client.publish(status_5, buffer5,n5);
    //Serial.print("freeMemory()=");
    //Serial.println(freeMemory());
    checkMemory();
    doc5.clear();
  }
    Serial.println(stat);
    
  }
  
void sensorData (){
  StaticJsonDocument<80> doc;
//doc["ver"] = ver;
//doc["ip"] = ip2CharArray(Ethernet.localIP());
JsonObject Status  = doc.createNestedObject("DS18B20");  
Status["temp"] = dssensor ();
doc["ldr"] = analogRead(photocellPin); ;
//doc["dev"] = devName;
//doc["con"] = connect_timeout;
//doc["ave"] = average;

  char buffer[80];
  serializeJson(doc, buffer);
 // Serial.println("Sending message to MQTT topic..");
 // serializeJson(doc, Serial);
 // Serial.println();
 size_t n = serializeJson(doc, buffer);
client.publish(sensorTopic, buffer,n);
}

void send_tele_state (){

 StaticJsonDocument<20> doc;
   doc["ver"] = ver;
   char buffer[20];
  serializeJson(doc, buffer);
 size_t n = serializeJson(doc, buffer);
client.publish(recState, buffer,n);  
}

void send_data (){

   if (millis()- newTmqtt >= 300000) {
    send_tele_state ();
    sensorData () ;

// sendData (average,rpm,maxRpm);
newTmqtt = millis();
       //testJson ();
    }
  
}

void callback(char* topic, byte* payload, unsigned int length) {
/*  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
*/
   
if(strcmp(topic, cmdTopic) == 0){
  //Serial.print ("Command topic recived:");
  //Serial.println((char)payload[0]);
  byte a = (char)payload[0];
  sendStatusData((char)payload[0]) ; 
  if ((char)payload[0] == '1'){
    sendStatusData('1');
  } else if ((char)payload[0] == '5'){
   sendStatusData('5') ; 
   Serial.println("Data 5 Sent");
  }
}



}
