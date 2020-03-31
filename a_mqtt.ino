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

// ------------------- Convert Payload to Int to use later ------------------
  char myNewArray[length+1];
  for (int i=0;i<length;i++) {
    myNewArray[i] = (char)payload[i];
  }
 myNewArray[length] = NULL;
 int command = atoi(myNewArray);  // now an unsigned long
    // Serial.println(F("Command:"));
    // Serial.println(command); 
if(strcmp(topic, cmdTopic) == 0){
  //Serial.print ("Command topic recived:");
  //Serial.println((char)payload[0]);
  //byte a = (char)payload[0];
  //sendStatusData((char)payload[0]) ; 
  if (command == 1){
    sendStatusData(1);
  } else if (command == 5){
   sendStatusData(5); 
    }else if (command == 10){
        Serial.println("Kick Recieved");
        previousKick = millis(); 
    }
  }
}


void start_mqtt (){
  client.setServer(server, 1883);
  client.setCallback(callback);
  lastReconnectAttempt = 0;
}

boolean reconnect() {
      char lwt[30]="";
      sprintf(lwt ,"%s%s%s","tele/",topic,"/LWT");  
 if (client.connect(topic,lwt , 1 , 1 , "Offline")){
      char sub[30]="";
      sprintf(sub ,"%s%s%s","cmnd/",topic,"/#");

    client.publish(lwt,"Online",true);
    connect_timeout = 0 ;
    MQTTdisconnect = 1 ;
    // ... and resubscribe
    client.subscribe(sub);
    digitalWrite(8, HIGH);
    Serial.println("MQTT Connected");
    reportInputStates();
    send_tele_state ();
    //reportInputStates();
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
   client.loop();
  }
}



  
