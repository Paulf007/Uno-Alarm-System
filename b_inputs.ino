void setupInput(){
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].attach( BUTTON_PINS[i] , INPUT_PULLUP  );       //setup the bounce instance for the current button
    buttons[i].interval(25);              // interval in ms
  }
}


void read_input_pins (){
// Read Inputs and Report States
  for (int i = 0; i < NUM_BUTTONS; i++)  {
    // Update the Bounce instance :
    buttons[i].update();
    // If it fell, flag the need to toggle the LED
    if ( buttons[i].fell() ) {
      char stopic[40];
      sprintf(stopic ,"%s%d",outPinTopic,(i+1));
      client.publish(stopic, "ON");
      //relayLink(i);
      ; // sent to c_sw_relay_link
      //connect_relay(i); // Hard Coded relay links is now disabled
      //show_statR2(1,i); // send to OLED
      //Serial.println(i);
    }else if ( buttons[i].rose() ){
      char stopic[40];
      sprintf(stopic ,"%s%d",outPinTopic,(i+1));
      client.publish(stopic, "OFF");
      //Serial.println(i);
      //int_link(i); // sent to c_sw_relay_link
      //connect_relay(i);  // Hard Coded relay links is now disabled
      //show_statR2(0,i);// send to OLED

    }
  }

}
