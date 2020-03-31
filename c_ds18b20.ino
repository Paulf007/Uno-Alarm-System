int dssensor (){
sensors.requestTemperatures();
//Serial.print("Temperature for the device 1 (index 0) is: ");
dstemp = sensors.getTempCByIndex(0); 
return dstemp;  
}

void checkKick(){
 if (millis()- previousKick >= kickInterval){  // TimeOut was reached and board needs to be reset - MQTT Meassage was not recieved in time
         //Serial.println("No Kick Recieved Restart Board");
         wdt_enable(WDTO_1S);
         previousKick = millis(); 
         //delay(5000);
 }
}
