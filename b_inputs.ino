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
    if ( buttons[i].fell() ) {
      char stopic[40];
      sprintf(stopic ,"%s%d",outPinTopic,(i+1));
      client.publish(stopic, "ON");
    }else if ( buttons[i].rose() ){
      char stopic[40];
      sprintf(stopic ,"%s%d",outPinTopic,(i+1));
      client.publish(stopic, "OFF");

    }
  }
}


void reportInputStates(){
//char stopic[40];  
//char* state = "";
  for (int thisPin = 0; thisPin < NUM_BUTTONS; thisPin++) {
//  sprintf(topic ,"%s%s%s%d","stat/",topic,"/SWITCH",(thisPin+1));    
  int i = BUTTON_PINS[thisPin];
    int state = digitalRead(i) ;
   // Serial.println(state); 
    if (state == 1){
      publishPinState(thisPin,"OFF");
    }else{
      publishPinState(thisPin,"ON");
    }

  }
}



void publishPinState(int pinNbr, char* state) {
  char topic[40];
  sprintf(topic ,"%s%d",outPinTopic,(pinNbr+1)); 
  client.publish(topic, state);
  //Serial.println("Send");
  }
