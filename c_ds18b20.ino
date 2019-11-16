int dssensor (){
sensors.requestTemperatures();
//Serial.print("Temperature for the device 1 (index 0) is: ");
dstemp = sensors.getTempCByIndex(0); 
return dstemp;  
}

void setupDSB18 (){
    byte i;
  byte dsAddress[8];
  Serial.println( "Searching for DS18B20..." );
  ds.reset_search();  // Start the search with the first device
  if( !ds.search(dsAddress) )
  {
  Serial.println( "none found. Using default MAC address." );
  } else {
   // Serial.println( "success. Setting MAC address:" );
   // Serial.print( " DS18B20 ROM  =" );
    for( i = 0; i < 8; i++)
    {
    //  Serial.write(' ');
   //   Serial.print( dsAddress[i], HEX );
    }
   // Serial.println();
    
    // Offset array to skip DS18B20 family code, and skip mac[0]
    mac[1] = dsAddress[3];
    mac[2] = dsAddress[4];
    mac[3] = dsAddress[5];
    mac[4] = dsAddress[6];
    mac[5] = dsAddress[7];
  }
  
 // Serial.print( " Ethernet MAC =" );
 // for( i = 0; i < 6; i++ )
  {
 //   Serial.write( ' ' );
 //   Serial.print( mac[i], HEX );
  }
//  Serial.println();

  // start the Ethernet connection:
//  Serial.print( "Eth: " );
  Ethernet.begin( mac );
//  Serial.print( "My IP address: " );
//  Serial.println( Ethernet.localIP() );
}
