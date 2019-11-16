void checkMemory(){
  //Serial.println(getPSTR("Old way to force String to Flash")); // forced to be compiled into and read   
  //Serial.println(F("New way to force String to Flash")); // forced to be compiled into and read   
  Serial.print(F("Free RAM = ")); //F function does the same and is now a built in library, in IDE > 1.0.0
  Serial.println(freeMemory(), DEC);  // print how much RAM is available.
}
