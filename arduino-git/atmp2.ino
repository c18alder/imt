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
#define Tsensor_char1 7
#define Tsensor_char2 4  //Temp Sensors 74
#define Tsensor_0 0
#define Tsensor_1 1
#define Tsensor_2 2
#define Tsensor_3 3
#define Sensor_NC_char0 9
#define Sensor_NC_char1 9

#define Icurrent_char1 8
#define Icurrent_char2 4  //Current out of battery

#define SoC_char1 9
#define SoC_char2 4  //SoC of the Battery
#define SoC_perc 0   //% battery next 2 digits 
#define SoC_PWM 3  //  he 2 last digits are the PWM 


#define PWM_char1 9
#define PWM_char2 9  //Temp Sensors

#define BYTES_PER_LINE 6   //Including the  "\n" (new line)

//Static variables to store the last parameters received ( if correct )
static float cells_voltage[25];
static int sensors_temp[5];
static int pwm_ec=0;
static int battery_SoC=0;
static int PWM_battery_charger=0;
static int counter=0;
static int error_data=0;
static int atoi_buf[5];


void parse_store_data(int *buffer){              //function to parse the data and stores it in the corresponding static variable.
  
/////////////////////////////////////TEMOERATURE////////////////////////////////////////////////
   
      if(buffer[0]==Tsensor_char1 && buffer[1]==Tsensor_char2){  
                  
            if(buffer[2]>=0 && buffer[2]<=3 && buffer[3]==Sensor_NC_char1 && buffer[4]==Sensor_NC_char1){   //check for "sensor unconected" condition first.
               sensors_temp[ buffer[2] ]=99;
               //    Serial.print("\nTemp BMS: Sensor not connected");  
            }else{switch(buffer[2]){                                                                        
              
                         case Tsensor_0:     
                              sensors_temp[0]=(buffer[3]*10 ) + buffer[4] ;
                         break;
                         
                         case Tsensor_1:
                              sensors_temp[1]=(buffer[3]*10 ) + buffer[4] ;
                         break;
                         
                         case Tsensor_2:
                              sensors_temp[2]=(buffer[3]*10 ) + buffer[4] ;                    
                         break;
                         
                         case Tsensor_3:
                              sensors_temp[3]=(buffer[3]*10 ) + buffer[4] ;
                         break;
                         
                         default:
                         ;                            // TODO maybe implement some action if much incoherent data received
                }
            }
       }
/////////////////////////////////////CURRENT/////////////////////////////////////////////////     
       if(buffer[0]==Icurrent_char1 && buffer[1]==Icurrent_char2){
            
       }
/////////////////////////////////////SoC////////////////////////////////////////////////////
       if(buffer[0]==SoC_char1 && buffer[1]==SoC_char2){ 
            switch(buffer[2]){
              
               case SoC_perc:
               case 1:   //to include the 100%
                    battery_SoC=(buffer[2]*100) + (buffer[3]*10) + buffer[4];
               break;
               
               case SoC_PWM:
                    PWM_battery_charger=(buffer[3]*10) + buffer[4];
               break;
               default:
             ;                                       // TODO maybe implement some action if many useless data received
            }
       }

/////////////////////////////////////PWM////////////////////////////////////////////////////  
       if(buffer[0]==PWM_char1 && buffer[1]==PWM_char2){
            if(buffer[2]==0){
              pwm_ec=  (buffer[3]*10) + buffer[4];
            
            }else{
            //  Serial.print("\nPWM Descritor sent (99), but error in 3rd bit, not '0');
            }
       }
/////////////////////////////////////VOLTAGE CELLS///////////////////////////////////////////
       if(  (buffer[0]*10 + buffer[1])>=1 && (buffer[0]*10 + buffer[1])<=24 ){    // IF it is in between 1 and 24 
            float aux_buffer=0;
            aux_buffer=( (buffer[2]*100) + (buffer[3]*10) + buffer[4] ) / 200.0;
            cells_voltage[  (buffer[0]*10 + buffer[1]) -1  ]= aux_buffer;  
       }
    
}
///////////END OF PARSE AND STORE FUNCTION/////////////////
////////////////////////////////////////////////////////////////////////////////////////////


//CAREFFULL WITH PRINTING SO MUCH WITH "Serial.print", it is really RAM consuming, data might be lost when reading.  ( nevertheless this is just to test and debug on PC )

void Display_all_values(){             //DISPLAYS THE VALUES OF EACH PARAMETER EVERY TIMER INTERRUPTION. 
                                                     //ALSO CHECKS IF AT LEAST "counter" lines of bytes were received in that time, otherwise, BMS probably crashed.
   if (counter<10 && counter!=0){      //TODO: instead of displaying, this will be sent to the OPENMOTE.
    Serial.print("\n\n\nBMS HAS CRASHED, GOOD LUCK NEXT TIME \n\n\n");
    counter=0;
  }else{
      
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
  }
   counter=0;
}






//////////////////////////////////////////////////////////
////AUX Variables/////
int cont = 0;
static int cont_bytes_total = 0;
uint8_t aux[2];
static  char  buf_aux[BYTES_PER_LINE];

//////FUNCTIONS FOR SETTING TO SET USB CONNECTION PARAMETERS/////////////
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
////// ///////////////////////////////////////////////////////////////////////


///USB CLASS///
USB     Usb;
//USBHub     Hub(&Usb);
ACMAsyncOper  AsyncOper;
ACM           Acm(&Usb, &AsyncOper);
/////////////////////////////////////////////////////////////////////////////

/////////////FUNCTION THAT SEND START AND STOP COMMANDS IF USER ENTERS instruction to. //////////////
int send_start_stop_cmd(){
  
      uint8_t cmd_reference = Serial.read();
      uint8_t start_cmd[2] = {'0', '1'}; //Start CMD
      uint8_t stop_cmd[2] = {'0', '0'}; //Stop CMD

      if (cmd_reference == 'e') { //ASCII   E
        Serial.print("\n\n Sending start command: "); Serial.print((char)start_cmd[0]); Serial.println((char)start_cmd[1]);
        return Acm.SndData(2, start_cmd);     //send start cmd to BMS.
      } else if (cmd_reference == 's') {
        Serial.print("\n\nSending stop command:  "); Serial.print((char)stop_cmd[0]); Serial.println((char)stop_cmd[1]);
        return Acm.SndData(2, stop_cmd);      //send stop cmd to BMS.
      } else {                                 //invalid code by user.
        Serial.print("\nYou entered:   "); Serial.print((char)cmd_reference); Serial.print(" \n\n Enter 'e' to send START cmd or 's' to send STOP cmd. \n\n");
        return 0;  
      }
}
/////////////////////////////////////////////////////

///////////////////////////FUNCTION TO ENSURE 5 BYTES LINES/////////////////////////////////////////////
//static buf_aux is used.( This is the static buffer used to store the incoming bytes until the 5 of the line is reached 
//because Arduino is not really trustworthy that it will read 5 bytes at a time.. Sometimes it reads 2 and in the next loop 3 and the information would be lost after each loop if not static. 
//THIS IS NECESSARY ( or at least a good precaution) BECAUSE THE BUFFER READ IS REALLY CRAPY (IT Doesn't always read 5 at a time) . This ensures the 5 bytes after the \n. 
void store_until_5bytes(char *buf,int received){
  
      for (uint16_t i = 0; i < received; i++ ) { 

            if (cont_bytes_total>0 && cont_bytes_total<(BYTES_PER_LINE) && buf[i]!='\n' ){  
                  buf_aux[cont_bytes_total-1]=buf[i];
                  cont_bytes_total++;
             }else if ( cont_bytes_total==(BYTES_PER_LINE) &&  buf[i]=='\n'){
                  //Convert the 5 byte char array into a 5 byte integer array 
                    
                   atoi_buf[0]=buf_aux[0]-'0';        //INT = ATOI(const char *string)    so it can't return a value higher than 33.000 ... Besides, it can't be used just to read string[2], for ex.
                   atoi_buf[1]=atoi(buf_aux+1)/1000;
                   atoi_buf[2]=atoi(buf_aux+2)/100;
                   atoi_buf[3]=atoi(buf_aux+3)/10;
                   atoi_buf[4]=atoi(buf_aux+4);
 
                   parse_store_data(atoi_buf);
                   cont_bytes_total=0;
                   counter++;  
             }else if (cont_bytes_total==(BYTES_PER_LINE) &&  buf[i]!='\n'){
                   cont_bytes_total=0;   // some error in the BMS received data. Look for next data.
             }
             if(buf[i]=='\n'){
                  cont_bytes_total=1;
             }
             
             

    
       }   //END OF FOR 
}
///////////////////////////////////////////////////////////////////////


void setup()
{
  Serial.begin( 9600 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect.
#endif
  Serial.println("Start");
  if (Usb.Init() == -1)
    Serial.println("OSCOKIRQ failed to assert");
    
 //////////INITIALIZE static vectors///////
for(int i=0;i<24;i++){
  cells_voltage[i]=0;
  if(i<5){
    sensors_temp[i]=0;
  }
}
/////SET TIMER INTERRUPT and callback FUNCTION//////
Timer1.initialize(15000000); // 150000 = 150ms 
Timer1.attachInterrupt( Display_all_values);
//////

  delay( 200 );
  if(cont_bytes_total==0){    //cont_bytes_total is relative to how many bytes were read until reach the full line ( has to be static ).
                                                 
                  for(int i=0;i<BYTES_PER_LINE;i++){
                      buf_aux[i]=0;
                  }
  }
}



void loop()
{
  Usb.Task();         //Function that established the USB conection.

  if ( Usb.getUsbTaskState() == USB_STATE_RUNNING && cont == 0) {
       Serial.print("USB THEORICALLY RUNNING \n");
       cont++;
  }

  if ( Acm.isReady()) {  //IF CONNECTION ESTABLISHED 
       uint8_t rcode;      //error code. 
    
         
       if (Serial.available()) {  //IF USER ENTERED CODE.. 
            
           rcode = send_start_stop_cmd();
           if (rcode){
                 Serial.print("There was an error sending the data to the BMS. \n");
           }
       }
         
       delay(50);

       char  buf[64];
       uint16_t rcvd = 64;
       rcode = Acm.RcvData(&rcvd, buf);   //reads received data.          TODO: Implement it with a timer instead of looping with delays. 

       //if (rcode && rcode != hrNAK)
       //    ErrorMessage<uint8_t>(PSTR("Ret"), rcode);

       if ( rcvd ) { //more than zero bytes received             
           store_until_5bytes(buf,rcvd);    //make sure 5 bytes line is complete.   
       } //END OF IF (rcvd)
       delay(10);

    
   }//if( Usb.getUsbTaskState() == USB_STATE_RUNNING..
}


