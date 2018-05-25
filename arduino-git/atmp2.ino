#include <cdcacm.h>
#include <usbhub.h>
#include <TimerOne.h>
#include "pgmstrings.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>


//VARIABLES AND FUNCTIONS FOR PARSING DATA 
//DESCRIPTORS
#define Tsensor_char1 '7'
#define Tsensor_char2 '4'  //Temp Sensors 74
#define Tsensor_0 '0'
#define Tsensor_1 '1'
#define Tsensor_2 '2'
#define Tsensor_3 '3'


#define Icurrent_char1 '8'
#define Icurrent_char2 '4'  //Current out of battery

#define SoC_char1 '9'
#define SoC_char2 '4'  //SoC of the Battery
#define SoC_perc '0'   //% battery next 2 digits 
#define SoC_PWM '3'   //  he 2 last digits are the PWM 


#define PWM_char1 '9'
#define PWM_char2 '9'  //Temp Sensors

#define BYTES_PER_LINE 6   //WITH THE \n

static float cells_voltage[25];
static int sensors_temp[5];
static int pwm_ec=0;
static int battery_SoC=0;
static int PWM_battery_charger=0;
static int counter=0;


void parse_data(uint8_t *buffer){
/////////////////////////////////////TEMOERATURE
       if(buffer[0]==Tsensor_char1 && buffer[1]==Tsensor_char2){
            switch(buffer[2]){
               case Tsensor_0:
                    if(buffer[3] == '9' && buffer[4] == '9'){
                      sensors_temp[0]=99;
                  //    Serial.print("\nTemp BMS: Sensor not connected");
                    }else{
                    sensors_temp[0]=((buffer[3]-48)*10) + (buffer[4]-48);
                  //  Serial.print("\nTemp Bat 1: ");Serial.print( (char)buffer[3] );Serial.print( (char)buffer[4] );Serial.print("\n");
                    }
               break;
               case Tsensor_1:
                    if(buffer[3] == '9' && buffer[4] == '9'){
                      sensors_temp[1]=99;
                   //   Serial.print("\nTemp BMS: Sensor not connected");
                    }else{
                    sensors_temp[1]=((buffer[3]-48)*10) + (buffer[4]-48);
                  //  Serial.print("\nTemp Bat 2: ");Serial.print( (char)buffer[3] );Serial.print( (char)buffer[4] );Serial.print("\n");
                    }
               break;
               case Tsensor_2:
                     if(buffer[3] == '9' && buffer[4] == '9'){
                     sensors_temp[2]=99;
                 //         Serial.print("\nTemp BMS: Sensor not connected");
                    }else{
                    sensors_temp[2]=((buffer[3]-48)*10) + (buffer[4]-48);
                   // Serial.print("\nTemp Bat 3: ");Serial.print( (char)buffer[3] );Serial.print( (char)buffer[4] );Serial.print("\n");
                    }
               break;
               case Tsensor_3:
                    if(buffer[3] == '9' && buffer[4] == '9'){
                      sensors_temp[3]=99;
                //      Serial.print("\nTemp BMS: Sensor not connected");
                    }else{
                    sensors_temp[3]=((buffer[3]-48)*10) + (buffer[4]-48);
                 //   Serial.print("\nTemp BMS: ");Serial.print( (char)buffer[3] );Serial.print( (char)buffer[4] );Serial.print("\n");
                    }
               break;
               default:
               ;
            }
       }
/////////////////////////////////////CURRENT     
       if(buffer[0]==Icurrent_char1 && buffer[1]==Icurrent_char2){
            
       }
/////////////////////////////////////SoC
       if(buffer[0]==SoC_char1 && buffer[1]==SoC_char2){
            switch(buffer[2]){
               case SoC_perc:
                    battery_SoC=((buffer[3]-48)*10) + (buffer[4]-48);

                 //   Serial.print("\nSoC (%): ");Serial.print( (char)buffer[3] );Serial.print( (char)buffer[4] );Serial.print("\n");
               break;
               case SoC_PWM:
                    PWM_battery_charger=((buffer[3]-48)*10) + (buffer[4]-48);
                //    Serial.print("\nPWM sent to Battery Charger (%): ");Serial.print( (char)buffer[3] );Serial.print( (char)buffer[4] );Serial.print("\n");
               break;
               default:
             ;//      Serial.print("\nEntered SoC code ( 94 ) but 3rd digit is neither 0 nor 3, it is: ");Serial.print( (char)buffer[2] );Serial.print("\n");
            }
       }

/////////////////////////////////////PWM   
       if(buffer[0]==PWM_char1 && buffer[1]==PWM_char2){
            if(buffer[2]=='0'){
              pwm_ec=  ((buffer[3]-48)*10) + (buffer[4]-48);
             // Serial.print("\nPWM sent to the EC:  ");Serial.print( (char)buffer[3] );Serial.print( (char)buffer[4] );Serial.print("\n");
            }else{
            //  Serial.print("\nPWM Descritor sent (99), but error in 3rd bit, not '0':  ");Serial.print((char)buffer[2]);Serial.print("\n");
            }
       }
/////////////////////////////////////VOLTAGE CELLS
       if(  (  buffer[0]==49  &&  buffer[1]>=48 && buffer[1]<=57)  ||  ( buffer[0]==50 &&  buffer[1]>=48 && buffer[1]<=52  )   ||  (buffer[0]==48 &&  buffer[1]>=49 && buffer[1]<=57) ){    // IF it is in between 1 and 24 
            float aux_buffer=0;
            buffer[2]=buffer[2]-48;  //Get the integer value of the 3 last digits .
            buffer[3]=buffer[3]-48;
            buffer[4]=buffer[4]-48;
            aux_buffer=( (buffer[2]*100) + (buffer[3]*10) + buffer[4] ) / 200.0;
            
            cells_voltage[  ( ((buffer[0]-48)*10) + (buffer[1]-48) )  ]= aux_buffer;  
            //Serial.print("\nCell ");Serial.print( (char)buffer[0] );Serial.print( (char)buffer[1] );Serial.print(" voltage is: ");Serial.print(aux_buffer);Serial.print("\n");
           // Serial.print("\nCell ");Serial.print( (char)buffer[0] );Serial.print( (char)buffer[1] );Serial.print(" voltage is: ");Serial.print( cells_voltage[  ( ((buffer[0]-48)*10) + (buffer[1]-48) )  ]);Serial.print("\n");
       }
    
}


void Display_all_values(){

  if (counter<20 && counter!=0){
    Serial.print("\n\n\nBMS HAS CRASHED, GOOD LUCK NEXT TIME \n\n\n");
    counter=0;
  }else{

  //Serial.print("\n\n\n All VALUES DISPLAYED:   ");
  Serial.print("\n           Temperatures of battery 1 ( °C ):   "); Serial.print(sensors_temp[0]);
  Serial.print("\n           Temperatures of battery 2 ( °C ):   "); Serial.print(sensors_temp[1]);
  Serial.print("\n           Temperatures of battery 3 ( °C ):   "); Serial.print(sensors_temp[2]);
  Serial.print("\n           T_BMS ( °C ):   "); Serial.print(sensors_temp[3]);
  Serial.print("\n           SoC ( % ):   "); Serial.print(battery_SoC);
  Serial.print("\n           PWM to Battery Charger ( % ):   ");Serial.print(PWM_battery_charger);
  Serial.print("\n           PWM to EC ( % ):   ");Serial.print(pwm_ec);
  Serial.print("\n");
  for(int j=0;j<24;j++){
    Serial.print("           Cell ");Serial.print(j);Serial.print(" voltage LvL ( V ):  ");Serial.print(cells_voltage[j]);Serial.print("\n");
  }
  Serial.print("\n\n\n\n\n\n\n");
  }
   counter=0;
}






//////////////////////////////////////////////////////////
int cont = 0;
static int cont_bytes = 0;
static int cont_bytes_total = 0;
uint8_t aux[2];
static   int8_t  buf_aux[BYTES_PER_LINE];


class ACMAsyncOper : public CDCAsyncOper
{
  public:
    uint8_t OnInit(ACM *pacm);
};

uint8_t ACMAsyncOper::OnInit(ACM *pacm)
{
  uint8_t rcode;
  // Set DTR = 1 RTS=1
  rcode = pacm->SetControlLineState(3);

  if (rcode)
  {
    ErrorMessage<uint8_t>(PSTR("SetControlLineState"), rcode);
    return rcode;
  }

  LINE_CODING  lc;
  lc.dwDTERate  = 9600;
  lc.bCharFormat  = 0;
  lc.bParityType  = 0;
  lc.bDataBits  = 8;

  rcode = pacm->SetLineCoding(&lc);

  if (rcode)
    ErrorMessage<uint8_t>(PSTR("SetLineCoding"), rcode);

  return rcode;
}

USB     Usb;
//USBHub     Hub(&Usb);
ACMAsyncOper  AsyncOper;
ACM           Acm(&Usb, &AsyncOper);

void setup()
{
  Serial.begin( 9600 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSCOKIRQ failed to assert");
    
 ////////////////////INITIALIZE 
for(int i=0;i<24;i++){
  cells_voltage[i]=0;
  if(i<5){
    sensors_temp[i]=0;
  }
}
/////////////////////SET TIMER TO DISPLAY ALL VALUES
Timer1.initialize(15000000); // 150000 = 150ms 
Timer1.attachInterrupt( Display_all_values);

  delay( 200 );
  
}

void loop()
{
  Usb.Task();

  if ( Usb.getUsbTaskState() == USB_STATE_RUNNING && cont == 0) {
  //  Serial.print("USB THEORICALLY RUNNING \n");
    cont++;
  }

  if ( Acm.isReady()) {
    uint8_t rcode;

    //Read from arduino Keyboard ( screen ), and send command accordingly.
    if (Serial.available()) {

      uint8_t cmd_reference = Serial.read();
      uint8_t start_cmd[2] = {48, 49}; //ASCII FOR 01
      uint8_t stop_cmd[2] = {48, 48}; //ASCII FOR 00

      if (cmd_reference == 101) { //ASCII   E
        Serial.print("\n\n Sending start command: "); Serial.print((char)start_cmd[0]); Serial.print((char)start_cmd[1]); Serial.print("\n\n");
        rcode = Acm.SndData(2, start_cmd);
      } else if (cmd_reference == 115) { //ASCII    S
        Serial.print("\n\nSending stop command:  "); Serial.print((char)stop_cmd[0]); Serial.print((char)stop_cmd[1]); Serial.print("\n\n");
        rcode = Acm.SndData(2, stop_cmd);
      } else {
        Serial.print("\nYou entered:   "); Serial.print((char)cmd_reference); Serial.print(" \n\n Enter 'e' to send START cmd or 's' to send STOP cmd. \n\n");
      }



      if (rcode)
        Serial.print("There was an error sending the data to the BMS \n");
      ErrorMessage<uint8_t>(PSTR("SndData"), rcode);
    }//if(Serial.available()...

    delay(50);

    /* reading the phone */
    /* buffer size must be greater or equal to max.packet size */
    /* it it set to 64 (largest possible max.packet size) here, can be tuned down
      for particular endpoint */
    if(cont_bytes==0 && cont_bytes_total==0){  
      for(int i=0;i<BYTES_PER_LINE;i++){
    buf_aux[i]=0;
      }
    }
    
    int8_t  buf[64];
    uint16_t rcvd = 64;
    rcode = Acm.RcvData(&rcvd, buf);

  
 
  
    if (rcode && rcode != hrNAK)
        ErrorMessage<uint8_t>(PSTR("Ret"), rcode);

    if ( rcvd ) { //more than zero bytes received

            
      for (uint16_t i = 0; i < rcvd; i++ ) { 

           // Serial.print((char)buf[i]);

            if(cont_bytes==1){     //THIS IS NECESSARY IN ARDUINO BECAUSE THE BUFFER READ IS REALLY CRAPY (IT Doesn't always read 5 at a time) . This ensures that the 2 bytes after the \n are the DESCRIPTOR. 
                buf_aux[cont_bytes_total]=buf[i];
                cont_bytes=2;
                cont_bytes_total++;
            }else if(cont_bytes==2){
                buf_aux[cont_bytes_total]=buf[i];
                cont_bytes=0;
                cont_bytes_total++;

              //  Serial.print(" ");Serial.print((char)buf_aux[0]);Serial.print((char)buf_aux[1]);Serial.print(" ");
               
            }else if(cont_bytes_total==(BYTES_PER_LINE-1)){
              
                 cont_bytes_total=0; 
                                           
            }else if(cont_bytes_total>1){
                 buf_aux[cont_bytes_total]=buf[i];
                 cont_bytes_total++;
                 
            }
            if(buf[i]==10){ // "\n" next line of data starts in the next i.  This is to know where the next line starts
              //  Serial.print("\n\n\n");
                    cont_bytes=1;
                    cont_bytes_total=0;
                    
            }
         if( cont_bytes_total==(BYTES_PER_LINE-1) ){          //ONCE THE 5 BYTES ARE IN... CALL THE PARSING FUNCTION
           
          parse_data(buf_aux);
             
          }

      
       }   
       
       counter++;   
    }
    
    delay(10);

    
  }//if( Usb.getUsbTaskState() == USB_STATE_RUNNING..
}


