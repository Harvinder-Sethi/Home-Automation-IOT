
#include <SoftwareSerial.h>
#include <espduino.h>
#include <rest.h>


SoftwareSerial espPort(10, 11); // RX, TX
ESP esp(&espPort, &Serial, 9);
REST rest(&esp);
boolean wifiConnected = false;

const int ESP8266_CHPD = 9;

const int relay1 =  13;
const int relay2 =  3;
const int relay3 =  4;
const int relay4 =  5;
boolean data1 = false;
boolean data2 = false;
boolean data3 = false;
boolean data4 = false;
int loop_count = 0;

char response[266];
char buff[64];
String str_html,str_body,str_bulb1,str_bulb2,str_bulb3,str_bulb4;
String strData_Last1,strData_Last2;

void(* resetFunc) (void) = 0;                          //declare reset function at address 0


void clearBuffer(void) {
       for (int i = 0;i<266;i++ ) {
         response[i]=0;
       }
}

void wifiCb(void* response)
{
  uint32_t status;
  RESPONSE res(response);

  if(res.getArgc() == 1) {
    res.popArgs((uint8_t*)&status, 4);
    if(status == STATION_GOT_IP) {
      Serial.println("WIFI CONNECTED");
     
      wifiConnected = true;
    } else {
      wifiConnected = false;
    }    
  }
}

void setup() {
  

  pinMode(relay1, OUTPUT); 
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  
  digitalWrite(relay1,LOW);
  digitalWrite(relay2,LOW);
  digitalWrite(relay3,LOW);
  digitalWrite(relay4,LOW);
  
  Serial.begin(19200);
  espPort.begin(19200);
  
  esp.enable();
  delay(500);
  esp.reset();
  delay(500);
  while(!esp.ready());

  Serial.println("ARDUINO: setup rest client");
  if(!rest.begin("amkr.co.nf")) {
    Serial.println("ARDUINO: failed to setup rest client");
    while(1);
  }

  /*setup wifi*/
  Serial.println("ARDUINO: setup wifi");
  esp.wifiCb.attach(&wifiCb);

  //esp.wifiConnect("NEXTRA6946","02042082");
 esp.wifiConnect("sanjeet","012345678");
  Serial.println("ARDUINO: system started");
  
  
}

void loop() {
 
loop_start:  
  
  esp.process();  
  
  if(wifiConnected) {   
    
    char str_field1[6] , str_field2[6] ;
           
      sprintf(buff, "/file_to_read.html");                                    // field x last Data
      Serial.println(buff);
           
      rest.get((const char*)buff);
      //Serial.println("ARDUINO: send get");

      if(rest.getResponse(response, 266) == HTTP_STATUS_OK){
      
            //Serial.println("ARDUINO: GET successful");
            //Serial.println(response);
                              
            str_html = "";
            str_body = "";
            str_bulb1 = "";  
            str_bulb2 = ""; 
            str_bulb3 = ""; 
            str_bulb4 = ""; 
            getData();            //GET DATA                    
                 
          //Control OutPut
          if (str_bulb1 == "<b>1</b>" ){
            digitalWrite(relay1,HIGH);
            data1 = true;
            //Serial.println("HIGH");                     
          }
           
          if (str_bulb1 == "<b>0</b>" ){
            digitalWrite(relay1,LOW);
            data1 = false;
            //Serial.println("HIGH");                     
          }

          if (str_bulb2 == "<b>1</b>" ){
            digitalWrite(relay2,HIGH);
            data2 = true;
            //Serial.println("HIGH");                     
          }
           
          if (str_bulb2 == "<b>0</b>" ){
            digitalWrite(relay2,LOW);
            data2 = false;
            //Serial.println("HIGH");                     
          }
          
          if (str_bulb3 == "<b>1</b>" ){
            digitalWrite(relay3,HIGH);
            data3 = true;
            //Serial.println("HIGH");                     
          }
           
          if (str_bulb3 == "<b>0</b>" ){
            digitalWrite(relay2,LOW);
            data3 = false;
            //Serial.println("HIGH");                     
          }
          
          if (str_bulb4 == "<b>1</b>" ){
            digitalWrite(relay4,HIGH);
            data4 = true;
            //Serial.println("HIGH");                     
          }
           
          if (str_bulb4 == "<b>0</b>" ){
            digitalWrite(relay4,LOW);
            data4 = false;
            //Serial.println("HIGH");                     
          }
      }  
     else{  
       //hardReset();
       //resetFunc();
     }
     
     
    /*  delay(10000);  
      sprintf(buff, "/channels/106223/fields/2/last");              // field x last Data
      Serial.println(buff);
           
      rest.get((const char*)buff);
     // Serial.println("ARDUINO: send get");

      if(rest.getResponse(response, 266) == HTTP_STATUS_OK){
      
        //Serial.println("ARDUINO: GET successful");
        //Serial.println(response);
         
        strId = "";  strData = "";  strCode = "";  
        getData();                                                // GET DATA
      
      
      //Control OutPut
      if (strData == "1" || strData == "1.0" || strData == "1.00"){
        digitalWrite(relay2,HIGH);
        data2 = true;        
      }
       
      if (strData == "0" || strData == "-1" || strData == "0.1" || strData == "0.10"){
        digitalWrite(relay2,LOW);
        data2 = false;
        //Serial.println("LOW");       
      }     
      } 
      else{  
         hardReset();
         resetFunc();
      
          clearBuffer();  
        //delay(10000);
            
          goto loop_start;   
       
     }  */  
      delay(1000);      
      loop_count++;
      Serial.println("LOOP : ");
      Serial.println(loop_count);
      
      
      
      // Check Status UPDATE ThingSpeak
      
      if(loop_count == 6 ){
        loop_count = 0;
      
      if(data1) {       
           dtostrf(1, 1, 1, str_field1);     
      }else{
         dtostrf(0.1, 2, 1, str_field1);    
      }
      
      if(data2) {
           dtostrf(1, 1, 1, str_field2);
      }else{
         dtostrf(0.1, 2, 1, str_field2);   
      }
        
          //sprintf(buff, "//update?key=7F51GLZSCWRK7S4H&field1=%s&field2=%s",str_field1,str_field2);              // field x last Data
          //Serial.println(buff);
           
          rest.get((const char*)buff);
          Serial.println("ARDUINO: send Update Data");

          if(rest.getResponse(response, 266) == HTTP_STATUS_OK){
      
            Serial.println("ARDUINO: GET successful");
            //Serial.println(response);
         
            str_html = "";  str_body = "";  str_bulb1 = "";  str_bulb2 = ""; str_bulb3 = "";str_bulb4 = "";
            getData();                                                // GET DATA
          }
      
      delay(5000);
      
      }
  
  
  } //if WifiConnected
  
  else{
    
     //hardReset();
     //resetFunc();
    
  }
  
} 
  
  void getData(){
   int i=0,j=0,k=0; 
     
         for (i = 0; i < 10; i++){         
//          Serial.print("Data");
//          Serial.print(i);
//          Serial.print(" : ");
//          Serial.print(response[i]);
//          Serial.println("");
        
          
          if((response[i] == '\r') || (response[i] == '\n')) { 
          }
          else{          
             str_html += response[i];       
          }
          
          if (response[i] == '\n'){ 
               i++;         
               break;
          }                    
        }
        
        Serial.println("");
        Serial.print("HTML : ");
        Serial.print(str_html);
           
         for (j = i; j < (i+20); j++){
          
          if((response[j] == '\r') || (response[j] == '\n')) { 
          }
          else{          
             str_body += response[j];       
          }
          
          if (response[j] == '\n'){ 
                j++;      
                break;
          }                     
        }
        
        Serial.println("");
        Serial.print("Body : ");
        Serial.print(str_body);
        //strData_Last = strData ;
        //Serial.println(strData.length());
                        
         for (k = j; k < (j+10); k++){        
              
          if((response[k] == '\r') || (response[k] == '\n')) { 
          }
          else{          
             str_bulb1 += response[k];       
          }
          
          if (response[k] == '\n'){                       
                break;
          }                     
        }
        
        Serial.println("");
        Serial.print("Bulb 1 : ");
        Serial.print(str_bulb1);    

    for (k = j; k < (j+10); k++){        
              
          if((response[k] == '\r') || (response[k] == '\n')) { 
          }
          else{          
             str_bulb2 += response[k];       
          }
          
          if (response[k] == '\n'){                       
                break;
          }                     
        }
        
        Serial.println("");
        Serial.print("Bulb 2 : ");
        Serial.print(str_bulb2); 

        for (k = j; k < (j+10); k++){        
              
          if((response[k] == '\r') || (response[k] == '\n')) { 
          }
          else{          
             str_bulb3 += response[k];       
          }
          
          if (response[k] == '\n'){                       
                break;
          }                     
        }
        
        Serial.println("");
        Serial.print("Bulb 3 : ");
        Serial.print(str_bulb3); 

        for (k = j; k < (j+10); k++){        
              
          if((response[k] == '\r') || (response[k] == '\n')) { 
          }
          else{          
             str_bulb4 += response[k];       
          }
          
          if (response[k] == '\n'){                       
                break;
          }                     
        }
        
        Serial.println("");
        Serial.print("Bulb 4 : ");
        Serial.print(str_bulb4); 
          
        Serial.println("");
               
      }
    
  
  boolean hardReset() {
  String tmpData;
  
  digitalWrite(ESP8266_CHPD,LOW);
  delay(2000);
  digitalWrite(ESP8266_CHPD,HIGH);
  //delay(1000);
    
}

