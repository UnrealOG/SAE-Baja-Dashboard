 #define Hall_Sensor A0
#define Hall_Sensor_D 7

int val1=0;
int val2=0;
unsigned long time1=0;
unsigned long time2=0;
const long interval=1000;
unsigned long delta=0;
long rpm=0;
bool last = false;
bool zero = false;
long last_time = millis();

void setup(){
  Serial.begin(9600);
  pinMode(Hall_Sensor_D,INPUT);
  pinMode(13,OUTPUT);
  time1=millis();
}

void loop(){
  /*
   * Fix math to accurately measure rpm (not just size of magnet)
   */
  if(!digitalRead(Hall_Sensor_D)) {
    if(!last) {
      delta = millis() - last_time;
      rpm = 60000 / delta;
      Serial.println(rpm);
      zero = false;
      last = true;
      last_time = millis();
    }
  }
  else {
    last = false;
    //if(!zero && millis() - last_time > 60000/
  }
   
  //val1=analogRead(Hall_Sensor);
  //Serial.print(val1);
//  time1=millis();
//  val1=digitalRead(Hall_Sensor_D);  //reads sensor once
//  digitalWrite(13, val1);
//  while(val1==digitalRead(Hall_Sensor_D)){}   //delays until a change in the hall effect sensor
//  while(val1!=digitalRead(Hall_Sensor_D)){}   //delays until the value is back to what it was (one full rotation)
//  digitalWrite(13, val1);
//  time2=millis();
//  delta=(time2-time1);
//  //Serial.println(delta);
//  rpm=(long) 60000/delta;
//  /*delta=time2-time1;
//  rpm=(time2/delta)*60;*/
//  Serial.println(rpm);
  

  
  //Serial.print("\t");
  
  //Serial.println(val2);
}
