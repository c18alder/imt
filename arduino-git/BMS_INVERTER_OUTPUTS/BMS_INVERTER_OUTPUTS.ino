int ack=0;
int speed=0;
int battery=100;
int kilometrage=0;

char cell_value[7];
char battery_aux[4];
char speed_aux[6];
char kilometrage_aux[4];
char put_together[20];  //format to send (key + value )
short int cells[25];
char auxy[5];


short int testing[11];
char *testing_aux;
/*char inData[20]; // Allocate some space for the stringextern short int cells_voltage_from_arduino[NUMBER_OF_CELLS];
short int cells_voltage_from_arduino[NUMBER_OF_CELLS];
char inChar; // Where to store the character read
byte index = 0; // Index into array; where to store the character
char *a;
char aux[20];
*/


void setup() {
 
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}


void loop() {

 
  //GENERATE NEW RANDOM VALUES
  speed = random(0, 120);
  battery = random(0, 100);
  kilometrage = kilometrage + random(0, 20);


  //BATTERY build package (add key) 
   sprintf(battery_aux, "%d", battery);    //integer_values to string 
   strcpy(put_together,"%bat=");           //add_key
   strcat(put_together,battery_aux);      //add_value
   strcat(put_together,"}");              //add_finish character
   //send battery status
   delay(4000);                           //SEND
   Serial.println(put_together);
    
  //SPEED build package (add key)
   sprintf(speed_aux, "%d", speed);
   strcpy(put_together,"km/h=");
   strcat(put_together,speed_aux);
   strcat(put_together,"}");
   //send speed status
   delay(4000);
   Serial.println(put_together);

     
  //KILOMETRAGE build package (add key)
   sprintf(kilometrage_aux, "%d", kilometrage);
   strcpy(put_together,"kms=");
   strcat(put_together,kilometrage_aux);
   strcat(put_together,"}");
   delay(4000);
   Serial.println(put_together);
   
  //CELLS build package (add key)
for (int i=0 ; i < 24 ; i++ ){
  
    sprintf(cell_value, "%d", random(0, 500));
    strcpy(put_together,"cs");
    sprintf(auxy, "%d", i);
    strcat(put_together,"=");
    strcat(put_together,auxy);
     strcat(put_together,".");  //numero de celda/tension
    strcat(put_together,cell_value);
    //delay(20); // not necessary.
    Serial.println(put_together);    
       
}
  delay(4000);

 
  
  
}
