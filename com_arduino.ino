#define DATA_BUFFER_LENGTH 128


typedef enum {
  IS_ALIVE   = 0x0,
  RESET      = 0x1,    
  SET_PIN    = 0x30,
  SET_PWM    = 0x31,
  GET_TEMP   = 0x40,
  SET_SSR    = 0x41,
  WATER_FLOW = 0x42  
} opcode_t;


void reset(void) {
}


void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);  

  reset();
}


void set_pin(byte* data, byte datalen) {
  if (datalen == 2) {
    byte pin = data[0];
    pin -= 0x30;
    byte value = data[1];
    value -= 0x30;
    digitalWrite(pin, value?HIGH:LOW); 
  
  }
}

void set_pwm(byte* data, byte datalen) {
  if (datalen == 2) {
    byte pin = data[0];
    pin -= 0x30;
    byte value = data[1];
    value -= 0x30;
    analogWrite(pin, value?HIGH:LOW); 
  }
}

void loop() {
  /*  check if data has been sent from the computer: */
  /* read the most recent byte */

  byte data[DATA_BUFFER_LENGTH];
  byte opcode  = 0;
  byte datalen = 0;
  byte status  = 0;
    
  status = Serial.readBytes((char*)&opcode, 1);
  if (status) {
    status = Serial.readBytes((char*)&datalen, 1);
    datalen -= 0x30;
  }
  
  if (status && datalen < DATA_BUFFER_LENGTH) 
    status = Serial.readBytes((char*)&data, datalen);
  
  if (status && datalen < DATA_BUFFER_LENGTH) {  
    switch(opcode){
      case IS_ALIVE: Serial.write(0x01);
      break;
      case RESET: reset();
      break;
      
      case SET_PIN: set_pin(data, datalen);
      break; 
      case SET_PWM: set_pwm(data, datalen);
      break; 
    }

  }  


}

