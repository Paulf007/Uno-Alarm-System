int dssensor (){
sensors.requestTemperatures();
//Serial.print("Temperature for the device 1 (index 0) is: ");
dstemp = sensors.getTempCByIndex(0); 
return dstemp;  
}
