#include <cdcacm.h>
#include <usbhub.h>
#include <TimerOne.h>
#include "pgmstrings.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>


#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) Serial.print(__VA_ARGS__)
#define PRINTFLN(...) Serial.println(__VA_ARGS__)
#else
#define PRINTF(...)
#define PRINTFLN(...)
#endif

#define SEND_START_COMMAND 1
#define SEND_STOP_COMMAND 2
#define ENABLE_AUTOMATIC_COMMUNICATION 3
#define AUTOMATIC_MODE 4

#define STATUS_READING 1  // reading from BMS
#define STATUS_WAITING 2  //waiting for CMD start 
#define AMOUNT_LINES_READ_BMS 30  //AMOUNT DATA TO READ FROM BMS EACH TIME ( bms provides about 33 variables... so 45 is a conservative number to secure )

//KEY VALUE FOR THE RESOURCE TO BE SENT :
#define CAR_SPEED_KEY "km/h"
#define CAR_SoC_KEY "%bat"
#define CAR_KILOMETRAGE_KEY "kms"
#define PWM_EC_KEY "pec"   //PWM Engine Controler
#define PWM_BC_KEY "pbc"  //PWM Battery Charger 
#define TEMP_1_KEY "ta"
#define TEMP_2_KEY "tb"
#define TEMP_3_KEY "tc"
#define TEMP_4_KEY "td"
#define CELL_VOLT_KEY "cs" // a "." separates the cell value and number---> ej:  cs=18.649    (cell value is not devided by 200 yet (done in server) TODO : maybe do it here.. 




//VARIABLES AND FUNCTIONS FOR PARSING DATA
//DESCRIPTORS
#define Tsensor_char1 7
#define Tsensor_char2 4  //Temp Sensors 74
#define Tsensor_0 0
#define Tsensor_1 1
#define Tsensor_2 2
#define Tsensor_3 3
#define Sensor_NC_char0 9    //Sensor Not Conected 99
#define Sensor_NC_char1 9

#define Icurrent_char1 8
#define Icurrent_char2 4  //Current out of battery 84

#define SoC_char1 9       //SoC of the Battery   94
#define SoC_char2 4  
#define SoC_perc 0   //% battery 
#define SoC_PWM 3  //  PWM Battery Charger


#define PWM_char1 9   //PWM Engine controller
#define PWM_char2 9  

/////////////////////////////////////////////////RESOURCES ID ////////////////////
#define FIRST_CELL_VOLTAGE_ID 1
#define LAST_CELL_VOLTAGE_ID 24
#define FIRST_TEMPERATURE_SENSOR_ID 25
#define LAST_TEMPERATURE_SENSOR_ID 28
#define PWM_EC_ID 29 
#define BATTERY_SOC_ID 30
#define PWM_BATTERY_CHARGER_ID 31
#define CAR_SPEED_ID 32
#define CAR_KILOMETRAGE_ID 33    
/////////////////////////////////////////////////////////////////////////////
#define BYTES_PER_LINE 6   //Including the  "\n" (new line)
#define NUM_OF_RESOURCES 34   // each cell is one.
#define NUM_CELLS 24
#define NUM_TEMPERATURES 4
#define NOTIFY_TOLERANCE 0.05    // 1 -> 100 %         .. don't notify SERVER if the change is not bigger than this .
#define BMS_REQUEST_TIME 15000000  //   useg   
#define PERIOD_KILOMETRAGE_LOW_SPEED ( 36/(BMS_REQUEST_TIME/1000000) )  // ( amount of timer interrupts to wait until requesting the kilometrage if driving less than 50km/h = 1 km every 72s )
#define PERIOD_KILOMETRAGE_HIGH_SPEED ( 15/(BMS_REQUEST_TIME/1000000) )  // ( amount of timer interrupts to wait until requesting the kilometrage if driving more than 50km/h.  120Kh/h = 1 km every 30s )  
#define CAR_IS_CHARGING 95     

//Static variables to store the last parameters received ( if correct )
static float cells_voltage[NUM_CELLS+1];
static int sensors_temp[NUM_TEMPERATURES+1];
static int pwm_ec = 0;
static int battery_SoC = 0;
static int PWM_battery_charger = 0;
static int battery_current = 0;
static int car_speed = 0;
static int car_kilometrage = 0;


//Static variables to store the previous parameters received ( the one currently stored in the server )

static float in_server_cells_voltage[25];
static int in_server_sensors_temp[5];
static int in_server_pwm_ec = 0;
static int in_server_battery_SoC = 0;
static int in_server_PWM_battery_charger = 0;
static int in_server_car_speed = 0;
static int in_server_car_kilometrage = 0;

static int datain_counter = 0;        //counts lines received. Used to get a loop of data from BMS ( more than a loop be secure )

static int error_data = 0;
static int atoi_buf[5];               //array used to handle the current line being received.
static short int reading_status = STATUS_WAITING;            // status of the BMS communication.
static int timer_interrupt_counter_EC = 0;

void parse_store_data(int *buffer) {             //function to parse the data and stores it in the corresponding static variable.

  /////////////////////////////////////TEMOERATURE////////////////////////////////////////////////

  if (buffer[0] == Tsensor_char1 && buffer[1] == Tsensor_char2) {

    if (buffer[2] >= 0 && buffer[2] <= 3 && buffer[3] == Sensor_NC_char1 && buffer[4] == Sensor_NC_char1) { //check for "sensor unconected" condition first.
      sensors_temp[ buffer[2] ] = 99;
      //    Serial.print("\nTemp BMS: Sensor not connected");
    } else {
      switch (buffer[2]) {

        case Tsensor_0:
          sensors_temp[0] = (buffer[3] * 10 ) + buffer[4] ;
          break;

        case Tsensor_1:
          sensors_temp[1] = (buffer[3] * 10 ) + buffer[4] ;
          break;

        case Tsensor_2:
          sensors_temp[2] = (buffer[3] * 10 ) + buffer[4] ;
          break;

        case Tsensor_3:
          sensors_temp[3] = (buffer[3] * 10 ) + buffer[4] ;
          break;

        default:
          ;                            // TODO maybe implement some action if much incoherent data received
      }
    }
  }
  /////////////////////////////////////CURRENT/////////////////////////////////////////////////
  if (buffer[0] == Icurrent_char1 && buffer[1] == Icurrent_char2) {

  }
  /////////////////////////////////////SoC////////////////////////////////////////////////////
  if (buffer[0] == SoC_char1 && buffer[1] == SoC_char2) {
    switch (buffer[2]) {
      case SoC_perc:
      case 1:   //to include the 100%
        battery_SoC = (buffer[2] * 100) + (buffer[3] * 10) + buffer[4];
        break;

      case SoC_PWM:
        PWM_battery_charger = (buffer[3] * 10) + buffer[4];
        break;
      default:
        ;                                       // TODO maybe implement some action if many useless data received
    }
  }

  /////////////////////////////////////PWM////////////////////////////////////////////////////
  if (buffer[0] == PWM_char1 && buffer[1] == PWM_char2) {
    if (buffer[2] == 0) {
      pwm_ec =  (buffer[3] * 10) + buffer[4];

    } else {
      //  Serial.print("\nPWM Descritor sent (99), but error in 3rd bit, not '0');
    }
  }
  /////////////////////////////////////VOLTAGE CELLS///////////////////////////////////////////
  if (  (buffer[0] * 10 + buffer[1]) >= 1 && (buffer[0] * 10 + buffer[1]) <= 24 ) { // IF it is in between 1 and 24
    float aux_buffer = 0;
    aux_buffer = ( (buffer[2] * 100) + (buffer[3] * 10) + buffer[4] ) / 200.0;
    cells_voltage[  (buffer[0] * 10 + buffer[1]) - 1  ] = aux_buffer;
  }

}
///////////END OF PARSE AND STORE FUNCTION/////////////////
////////////////////////////////////////////////////////////////////////////////////////////


//CAREFFULL WITH PRINTING SO MUCH WITH "Serial.print", it is really RAM consuming, data might be lost when reading.  ( nevertheless this is just to test and debug on PC )

void Display_all_values() {            //DISPLAYS THE VALUES OF EACH PARAMETER EVERY TIMER INTERRUPTION.
  //ALSO CHECKS IF AT LEAST "counter" lines of bytes were received in that time, otherwise, BMS probably crashed.

   ///FROM BMS
    PRINTF("\n           Temp of bat 1 ( °C ):   "); PRINTF(in_server_sensors_temp[0]);
    PRINTF("\n           Temp of bat 2 ( °C ):   "); PRINTF(in_server_sensors_temp[1]);
    PRINTF("\n           Temp of bat 3 ( °C ):   "); PRINTF(in_server_sensors_temp[2]);
    PRINTF("\n           T_BMS ( °C ):   "); PRINTF(in_server_sensors_temp[3]);
    PRINTF("\n           SoC ( % ):   "); PRINTF(in_server_battery_SoC);
    PRINTF("\n           PWM to Ba Ch ( % ):   "); PRINTF(in_server_PWM_battery_charger);
    PRINTF("\n           PWM to EC ( % ):   "); PRINTFLN(in_server_pwm_ec);
    for (int j = 0; j < 24; j++) {
      PRINTF("           Cell "); PRINTF(j); PRINTF(" ( V ): "); PRINTFLN(in_server_cells_voltage[j]); 
    }
    ///FROM ENGINE CONTROLLER


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

/////////////FUNCTION THAT SEND START AND STOP COMMANDS IF USER ENTERS instruction to (to debug manually ). Also sets the automatic functioning if USER enables it //////////////
uint8_t send_start_stop_cmd(short int start_or_stop) {
  uint8_t rcode = 0;
  uint8_t start_cmd[2] = {'0', '1'}; //Start CMD
  uint8_t stop_cmd[2] = {'0', '0'}; //Stop CMD

  if (start_or_stop == SEND_START_COMMAND && ( reading_status == STATUS_WAITING || reading_status == AUTOMATIC_MODE) ) {

    //Serial.print("\n\n Sending start command: "); Serial.print((char)start_cmd[0]); Serial.println((char)start_cmd[1]);
    rcode = Acm.SndData(2, start_cmd);
    if (!rcode) {
      reading_status = STATUS_READING;
    }
    return  rcode;    //send start cmd to BMS.

  } else if (start_or_stop == SEND_STOP_COMMAND && (reading_status == STATUS_READING || reading_status == AUTOMATIC_MODE) ) {

    //Serial.print("\n\nSending stop command:  "); Serial.print((char)stop_cmd[0]); Serial.println((char)stop_cmd[1]);
    rcode = Acm.SndData(2, stop_cmd);      //send stop cmd to BMS.
    if (!rcode) {
      reading_status = STATUS_WAITING;
    }
    return  rcode;
  } else if (start_or_stop == ENABLE_AUTOMATIC_COMMUNICATION) {
       if( reading_status == STATUS_READING ){
             //Serial.print("\n\nSending stop command:  "); Serial.print((char)stop_cmd[0]); Serial.println((char)stop_cmd[1]);
             rcode = Acm.SndData(2, stop_cmd);      //send stop cmd to BMS.
             if (!rcode) {
                  reading_status = STATUS_WAITING;
             }
       }
  reading_status = AUTOMATIC_MODE;
  //Serial.print("\nAutomatic BMS and EC communication started\n"); //for example , this lines takes around  45ms to be printed. Really slow. But the code moves forward meanwhile.
  //delay(45);
  Timer1.initialize(BMS_REQUEST_TIME); //   us
  Timer1.attachInterrupt( handle_request_timer_interrupt); 
   
  }
 



  
  return  rcode;
}
/////////////////////////////////////////////////////

///////////////////////////FUNCTION TO ENSURE 5 BYTES LINES/////////////////////////////////////////////
//static buf_aux is used.( This is the static buffer used to store the incoming bytes until the 5 of the line is reached
//because Arduino is not really trustworthy that it will read 5 bytes at a time.. Sometimes it reads 2 and in the next loop 3 and the information would be lost after each loop if not static.
//THIS IS NECESSARY ( or at least a good precaution) BECAUSE THE BUFFER READ IS REALLY CRAPY (IT Doesn't always read 5 at a time) . This ensures the 5 bytes after the \n.
void wait_full_line_and_store(char *buf, int received) {

  for (uint16_t i = 0; i < received; i++ ) {

    if (cont_bytes_total > 0 && cont_bytes_total < (BYTES_PER_LINE) && buf[i] != '\n' ) {
      buf_aux[cont_bytes_total - 1] = buf[i];
      cont_bytes_total++;
    } else if ( cont_bytes_total == (BYTES_PER_LINE) &&  buf[i] == '\n') {
      //Convert the 5 byte char array into a 5 byte integer array

      atoi_buf[0] = buf_aux[0] - '0';    //INT = ATOI(const char *string)    so it can't return a value higher than 33.000 ... Besides, it can't be used just to read string[2], for ex.
      atoi_buf[1] = atoi(buf_aux + 1) / 1000;
      atoi_buf[2] = atoi(buf_aux + 2) / 100;
      atoi_buf[3] = atoi(buf_aux + 3) / 10;
      atoi_buf[4] = atoi(buf_aux + 4);

      parse_store_data(atoi_buf);
      cont_bytes_total = 0;
      datain_counter++;
    } else if (cont_bytes_total == (BYTES_PER_LINE) &&  buf[i] != '\n') {
      cont_bytes_total = 0; // some error in the BMS received data. Look for next data.
    }
    if (buf[i] == '\n') {
      cont_bytes_total = 1;
    }




  }   //END OF FOR
}
///////////////////////////////////////////////////////////////////////

int compare_server_vs_new(int id_counter) {   //ANALYZES IF any data has changed enough to update the server... returns the corresponding resourse ID. 0 otherwise.

  if (id_counter >= FIRST_CELL_VOLTAGE_ID && id_counter <= LAST_CELL_VOLTAGE_ID) {                        //TODO: different criterias can be implemented for each resource tolerance. 
      
   
    if ( ( (in_server_cells_voltage[id_counter - 1] * (1 - NOTIFY_TOLERANCE) ) >= cells_voltage[id_counter - 1] ) || ( (in_server_cells_voltage[id_counter - 1] * (1 + NOTIFY_TOLERANCE) ) <= cells_voltage[id_counter - 1] ) ){
        
      if( !(in_server_cells_voltage[id_counter - 1]==0 &&  cells_voltage[id_counter - 1]==0) ){
         return true;
      }
    } else {
      
      return false;
    }

  } else if (id_counter >= FIRST_TEMPERATURE_SENSOR_ID && id_counter <= LAST_TEMPERATURE_SENSOR_ID) {

    if (  ( (in_server_sensors_temp[id_counter - FIRST_TEMPERATURE_SENSOR_ID] * (1.0 - NOTIFY_TOLERANCE) ) >= sensors_temp[id_counter - FIRST_TEMPERATURE_SENSOR_ID] )|| ( (in_server_sensors_temp[id_counter - FIRST_TEMPERATURE_SENSOR_ID] * (1.0 + NOTIFY_TOLERANCE) ) <= sensors_temp[id_counter - FIRST_TEMPERATURE_SENSOR_ID] ) ) {
      if( !(in_server_sensors_temp[id_counter - FIRST_TEMPERATURE_SENSOR_ID]==0 &&  sensors_temp[id_counter - FIRST_TEMPERATURE_SENSOR_ID]==0) ){
         return true;
      }
    } else {
      return false;
    }

  } else if (id_counter == PWM_EC_ID) {
    if (  (in_server_pwm_ec * (1.0 - NOTIFY_TOLERANCE) ) >= pwm_ec || (in_server_pwm_ec * (1.0 + NOTIFY_TOLERANCE) ) <= pwm_ec ) {
       if( !(in_server_pwm_ec==0 &&  pwm_ec==0) ){
         return true;
      }
    } else {
      return false;
    }
  } else if (id_counter == BATTERY_SOC_ID) {
    if (  (in_server_battery_SoC * (1.0 - NOTIFY_TOLERANCE) ) >= battery_SoC || (in_server_battery_SoC * (1.0 + NOTIFY_TOLERANCE) ) <= battery_SoC ) {
      if( !(in_server_battery_SoC==0 &&  battery_SoC==0) ){
         return true;
      }
    } else {
      return false;
    }
  } else if (id_counter == PWM_BATTERY_CHARGER_ID) {
    if (  (in_server_PWM_battery_charger * (1.0 - NOTIFY_TOLERANCE) ) >= PWM_battery_charger || (in_server_PWM_battery_charger * (1.0 + NOTIFY_TOLERANCE) ) <= PWM_battery_charger ) {
      if( !(in_server_PWM_battery_charger==0 &&  PWM_battery_charger==0) ){
         return true;
      }
    } else {
      return false;
    }
  } else if (id_counter == CAR_SPEED_ID) {
    if (  (in_server_car_speed * (1.0 - NOTIFY_TOLERANCE) ) >= car_speed || (in_server_car_speed * (1.0 + NOTIFY_TOLERANCE) ) <= car_speed ) {
      if( !(in_server_car_speed==0 &&  car_speed==0) ){
         return true;
      }
    } else {
      return false;
    }
  } else if (id_counter == CAR_KILOMETRAGE_ID) {
    if (  (in_server_car_kilometrage * (1.0 - NOTIFY_TOLERANCE) ) >= car_kilometrage || (in_server_car_kilometrage * (1.0 + NOTIFY_TOLERANCE) ) <= car_kilometrage ) {
      if( !(in_server_car_kilometrage==0 &&  car_kilometrage==0) ){
          return true;
      }
    } else {
      return false;
    }
  }


  return false;
}
void replace_server_with_new(int resource_id) {
  
  if (resource_id >= FIRST_CELL_VOLTAGE_ID && resource_id <= LAST_CELL_VOLTAGE_ID) {
    in_server_cells_voltage[resource_id - 1] = cells_voltage[resource_id - 1];
  
  } else if (resource_id >= 25 && resource_id <= 28) {
    in_server_sensors_temp[resource_id - FIRST_TEMPERATURE_SENSOR_ID] = sensors_temp[resource_id - FIRST_TEMPERATURE_SENSOR_ID];

  } else if (resource_id == PWM_EC_ID) {
    in_server_pwm_ec = pwm_ec;
  } else if (resource_id == BATTERY_SOC_ID) {
    in_server_battery_SoC = battery_SoC;
  } else if (resource_id == PWM_BATTERY_CHARGER_ID) {
    in_server_PWM_battery_charger = PWM_battery_charger;
  } else if (resource_id == CAR_SPEED_ID) {
    in_server_car_speed = car_speed;
  } else if (resource_id == CAR_KILOMETRAGE_ID) {
    in_server_car_kilometrage = car_kilometrage;
  }
}
void notify_server_of_change(int resource_id) {                     //FUNCTION TO ADD KEY TO RESOURCE AND NOTIFY THE SERVER OF A NEW RESOURCE TO UPDATE.
       short int rcode=0;
       char put_together[10];
       char auxiliar[4];
       
       if (resource_id >= FIRST_CELL_VOLTAGE_ID && resource_id <= LAST_CELL_VOLTAGE_ID) {   //sends the format cs=cn.cv cn=cell number cv=cell value * 100(integer) 
          
          
          int aux_float_int=in_server_cells_voltage[resource_id - 1]*100;
          strcpy(put_together,CELL_VOLT_KEY);
          sprintf(auxiliar, "%d", resource_id);
          strcat(put_together,"=");
          strcat(put_together,auxiliar);
          strcat(put_together,".");  //numero de celda/tension
          sprintf(auxiliar, "%d", aux_float_int);
          strcat(put_together,auxiliar);   
          
          
       }else if (resource_id >= FIRST_TEMPERATURE_SENSOR_ID && resource_id <= LAST_TEMPERATURE_SENSOR_ID) {
          if(resource_id==FIRST_TEMPERATURE_SENSOR_ID)
                strcpy(put_together,TEMP_1_KEY);
          if(resource_id==FIRST_TEMPERATURE_SENSOR_ID+1)
                 strcpy(put_together,TEMP_2_KEY);
          if(resource_id==FIRST_TEMPERATURE_SENSOR_ID+2)
                 strcpy(put_together,TEMP_3_KEY);
          if(resource_id==LAST_TEMPERATURE_SENSOR_ID)
                 strcpy(put_together,TEMP_4_KEY);
    
          strcat(put_together,"=");
          sprintf(auxiliar, "%d", in_server_sensors_temp[resource_id - FIRST_TEMPERATURE_SENSOR_ID]);
          strcat(put_together,auxiliar);   
          
       }else if (resource_id == PWM_EC_ID) {
          strcpy(put_together,PWM_EC_KEY);
          strcat(put_together,"=");
          sprintf(auxiliar, "%d", in_server_pwm_ec);
          strcat(put_together,auxiliar);      
            
       }else if (resource_id == BATTERY_SOC_ID) {
          strcpy(put_together,CAR_SoC_KEY);
          strcat(put_together,"=");
          sprintf(auxiliar, "%d", in_server_battery_SoC);
          strcat(put_together,auxiliar);        
          
       }else if (resource_id == PWM_BATTERY_CHARGER_ID) {
          strcpy(put_together,PWM_BC_KEY);
          strcat(put_together,"=");
          sprintf(auxiliar, "%d", in_server_PWM_battery_charger);
          strcat(put_together,auxiliar);   
               
       }else if (resource_id == CAR_SPEED_ID) {
          strcpy(put_together,CAR_SPEED_KEY);
          strcat(put_together,"=");
          sprintf(auxiliar, "%d", in_server_car_speed);
          strcat(put_together,auxiliar);        
       
       }else if (resource_id == CAR_KILOMETRAGE_ID) {
          strcpy(put_together,CAR_KILOMETRAGE_KEY);
          strcat(put_together,"=");
          sprintf(auxiliar, "%d", in_server_car_kilometrage);
          strcat(put_together,auxiliar);        
       }else{
          //ERROR IN resource_id
          rcode=1;
       }
       if(!rcode){
           Serial.println(put_together);
           delay(10);
       }
       


}




void compare_replace_notify() {

  for (int resource_ID = 1; resource_ID <= NUM_OF_RESOURCES; resource_ID++) {
    if ( compare_server_vs_new(resource_ID) ) {
      replace_server_with_new(resource_ID);
      notify_server_of_change(resource_ID);
    }
  }
}
int engine_controller_request_necessary(){   //OPTIMIZATION CRITERIAS FOR EC REQUEST GO HERE. returns true if necessary, 0 otherwise.    
    if(in_server_PWM_battery_charger==CAR_IS_CHARGING ){    //BATTERY CHARGING. 
         return 0;
    }else if(in_server_car_speed==0){   //Car not moving.
         return CAR_SPEED_ID;
    }else if(  (timer_interrupt_counter_EC == PERIOD_KILOMETRAGE_HIGH_SPEED)  &&  in_server_car_speed>50 ){   //car goes > 50km/h and enough time passed since last request.
         timer_interrupt_counter_EC=0;
         return CAR_SPEED_ID+CAR_KILOMETRAGE_ID;
    }else if(  (timer_interrupt_counter_EC == PERIOD_KILOMETRAGE_LOW_SPEED)  &&  in_server_car_speed<50 ){
          timer_interrupt_counter_EC=0;
          return CAR_SPEED_ID+CAR_KILOMETRAGE_ID;
    }else if(timer_interrupt_counter_EC == PERIOD_KILOMETRAGE_LOW_SPEED){
          timer_interrupt_counter_EC=0;
          return CAR_SPEED_ID+CAR_KILOMETRAGE_ID;
    }else{
          return CAR_SPEED_ID;
    }
      
}

int handle_request_timer_interrupt() {   //ANALYZES THE OPTIMIZATION CONDITIONS AND MAKES A BMS OR EC REQUEST IF NEEDED. This is the callback function for the timer interruption (automatic communication).

     timer_interrupt_counter_EC++;       
     
       //BMS REQUEST 
    if ( Acm.isReady()){                   
          int rcode=0;
          rcode = send_start_stop_cmd(SEND_START_COMMAND);
          
          while(datain_counter!=AMOUNT_LINES_READ_BMS && reading_status == STATUS_READING){      //READS A LOOP OF DATA
                 int rcode=0;
                
                char  buf[20];
                uint16_t rcvd = 20;
                rcode = Acm.RcvData(&rcvd, buf);   //reads received data.          
            
                if ( rcvd ) { //more than zero bytes received
                  wait_full_line_and_store(buf, rcvd);   //makes sure 5 bytes line is complete (necessary with arduino's buffer and serial prints probable problems ) and stores the parsed data in memory.
                } 
                delay(10);
              
           }

           rcode = send_start_stop_cmd(SEND_STOP_COMMAND);
            delay(30);
           compare_replace_notify();   
           datain_counter=0;
           
           Display_all_values();  //to see it in monitor. Not necessary and memory consuming!. Use to debug.
    }
      //ENGINE CONTROLLER  REQUEST  ( Optimized ) 
      
      int ec_which_resource=engine_controller_request_necessary(); //0 if none. CAR_SPEED_ID if just that is needed. CAR_KILOMETRAGE_ID if just that is needed. CAR_SPEED_ID+CAR_KILOMETRAGE_ID if both needed. 
      if (  ec_which_resource== ( CAR_SPEED_ID+CAR_KILOMETRAGE_ID )  ){ 
           //REQUEST BOTH
      }else if(  ec_which_resource== CAR_KILOMETRAGE_ID ){
          //REQUEST car_kilometrage
      }else if( ec_which_resource== CAR_SPEED_ID ){
          //REQUEST car_speed
      }else{
          //REQUEST none.
      }
    
}

void setup()
{
 // Serial.begin( 9600 );
  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect.
#endif
  Serial.println("Start");
  if (Usb.Init() == -1)
    Serial.println("OSCOKIRQ failed to assert");

  //////////INITIALIZE static vectors///////
  for (int i = 0; i < 24; i++) {
    cells_voltage[i] = 0;
    if (i < 5) {
      sensors_temp[i] = 0;
    }
  }
  


  delay( 200 );
  if (cont_bytes_total == 0) { //cont_bytes_total is relative to how many bytes were read until reach the full line ( has to be static ).


    for (int i = 0; i < BYTES_PER_LINE; i++) {
      buf_aux[i] = 0;
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
    uint8_t rcode;      //error code variable ( 0 if no error )


    if (Serial.available()) {  //IF USER ENTERED CODE..
      uint8_t cmd_reference = Serial.read();
      short int type_of_cmd = 0;
      if (cmd_reference == 'e') {
        type_of_cmd = SEND_START_COMMAND;
      } else if (cmd_reference == 's') {
        type_of_cmd = SEND_STOP_COMMAND;
      }else if( cmd_reference == 't'){
        type_of_cmd = ENABLE_AUTOMATIC_COMMUNICATION;    //JUST TO DEBUG
      }else {                                 //invalid code by user.
        Serial.print("\nYou entered:   "); Serial.print((char)cmd_reference); Serial.print(" \n\n Enter 'e' = START, 's' = STOP and 't' = automatic \n\n");
      }

      rcode = send_start_stop_cmd(type_of_cmd);
      if (rcode) {
        Serial.print("There was an error sending the data to the BMS. \n");
      }
    }     // End of IF user enters commands.

    delay(50);

    //


      if(reading_status==STATUS_READING){      //FOR MANUAL OPERATION .. in automatic, the send cmd is sent before exiting the interruption so STATUS_READING is not true here.
                int rcode=0;
                char  buf[20];
                uint16_t rcvd = 20;
                rcode = Acm.RcvData(&rcvd, buf);   //reads received data.          
            
                if ( rcvd ) { //more than zero bytes received
                  wait_full_line_and_store(buf, rcvd);   //makes sure 5 bytes line is complete (necessary with arduino's buffer and serial prints probable problems ) and stores the parsed data in memory.
                } 
                delay(10);

           if(  !(datain_counter%25) && datain_counter>0 ){
          
           compare_replace_notify();
           }
           if(   !(datain_counter%200) && datain_counter>0  ){
           Display_all_values();  //to see it in monitor. Not necessary and memory consuming!. Use to debug.
           }
      }
           

  }//if( Usb.getUsbTaskState() == USB_STATE_RUNNING..


}



