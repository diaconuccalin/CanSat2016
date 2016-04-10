// Conversion factor - CPM to uSV/h
#define CONV_FACTOR 0.00812

// Variables
int geiger_input = 6;
long count = 0;
long countPerMinute = 0;
long timePrevious = 0;
long timePreviousMeassure = 0;
long time = 0;
long countPrevious = 0;
float radiationValue = 0.0;

void setup()
{
  pinMode(geiger_input, INPUT);
  digitalWrite(geiger_input,HIGH);
  
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(2),countPulse,FALLING);

}

void loop()
{
  if (millis()-timePreviousMeassure > 10000)
  {
    countPerMinute = 6*count;
    radiationValue = countPerMinute * CONV_FACTOR;
    timePreviousMeassure = millis();
    Serial.print("cpm = ");
    Serial.print(countPerMinute,DEC);
    Serial.print(" - ");
    Serial.print("uSv/h = ");
    Serial.println(radiationValue,4);
    
    count = 0;
  }
}

void countPulse()
{
  detachInterrupt(digitalPinToInterrupt(2));
  count++;
  while(digitalRead(2)==0){}
  attachInterrupt(digitalPinToInterrupt(2),countPulse,FALLING);
}
