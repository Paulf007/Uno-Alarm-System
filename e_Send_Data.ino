void send_data (){
 if (millis()- newTmqtt >= 300000) {
    send_tele_state ();
 newTmqtt = millis();
    }
  
}

// convert IP-address to a string for in this case MQTT
char* ip2CharArray(IPAddress IP) {
  static char a[16];
  sprintf(a, "%d.%d.%d.%d", IP[0], IP[1], IP[2], IP[3]);
  return a;
}


void sendStatusData(byte stat){
  //Serial.print(F("MQTT Command Recv:"));
  //Serial.println(stat);
  if (stat == 1){ // 49 is the byte version of 1
  // Serial.println("Case 1 Run");
      char status_0[30]="";
    sprintf(status_0 ,"%s%s%s","stat/",topic,"/STATUS");
    StaticJsonDocument<100> doc;
      JsonObject Status  = doc.createNestedObject("Status");  
      Status["Module"] = 1;
      JsonArray  FriendlyName  = Status.createNestedArray("FriendlyName");
      FriendlyName.add(Fname);
       Status["Topic"] = topic;  
      char buffer[80];
      serializeJson(doc, buffer);
      size_t n = serializeJson(doc, buffer);
      client.publish(status_0, buffer,n);
      doc.clear();

  } else if (stat == 5){ 
      char status_5[30]="";
      sprintf(status_5 ,"%s%s%s","stat/",topic,"/STATUS5");
      StaticJsonDocument<50> doc5;
      JsonObject Status5  = doc5.createNestedObject("StatusNET");  
      Status5["IPAddress"] =ip2CharArray(Ethernet.localIP());
      Status5["Mac"] = macS;
      char buffer5[75];
      serializeJson(doc5, buffer5);
      size_t n5 = serializeJson(doc5, buffer5);
      client.publish(status_5, buffer5,n5);
    doc5.clear();
  }
    Serial.println(stat);
    
  }

  void sensorData (){
      char sensor[30]="";
      sprintf(sensor,"%s%s%s","tele/",topic,"/SENSOR");
StaticJsonDocument<80> doc;
  JsonObject Status  = doc.createNestedObject("DS18B20");  
    Status["temp"] = dssensor ();
    doc["ldr"] = analogRead(photocellPin); ;
      char buffer[80];
      serializeJson(doc, buffer);
      size_t n = serializeJson(doc, buffer);
      client.publish(sensor, buffer,n);
      doc.clear();
}

void send_tele_state (){
    char state[30]="";
    sprintf(state,"%s%s%s","tele/",topic,"/STATE");
      StaticJsonDocument<20> doc;
      doc["ver"] = ver;
      char buffer[20];
      serializeJson(doc, buffer);
      size_t n = serializeJson(doc, buffer);
      client.publish(state, buffer,n); 
      doc.clear();
      sensorData () ; 
}
