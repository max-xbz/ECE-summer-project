#include <SoftwareSerial.h>
SoftwareSerial esp8266Module(10, 11);
int ledPin = 4;
String ssid="MAX's iPhone";
String password="19921800";
bool DEBUG = true;
bool CONNECTED = false;
void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  esp8266Module.begin(9600);
  delay(5000);
  clearBuffer();
  pinMode(ledPin, OUTPUT);

 // myexec("AT");
 // myexec("AT+RST");
//  myexec("AT+CWMODE=3");
//  connectToWifi(ssid,password);
  
  myexec("AT+CIPMUX=0");
  



  
  
}

String myexec(String cmd, int milliSec){
  Serial.println("issuing cmd: "+ cmd);
  esp8266Module.println(cmd);
  delay(milliSec);
  String result = "";
  while(esp8266Module.available() > 0){
    char c = esp8266Module.read();
    result = result + c;
  }

  if(DEBUG) Serial.println(result);
  return result;
}


String myexec(String cmd){
  Serial.println("issuing cmd: "+ cmd);
  esp8266Module.println(cmd);
  delay(5000);
  String result = "";
  while(esp8266Module.available() > 0){
    char c = esp8266Module.read();
    result = result + c;
  }

  if(DEBUG) Serial.println(result);
  return result;
}

void loop() {

    TCPConnect("www.runxiflute.com");
  
  String res = myexec("AT+CIPSEND=50",1000);
  if(res.indexOf(">") != -1){
      esp8266Module.print("GET /test.html HTTP/1.1\r\nHost: runxiflute.com");
      while(esp8266Module.available() == 0){
      esp8266Module.print("\r\n");  
      delay(100);
      }
  }

  //delay(1000);
  String result;
  int i =0;
  int poundCnt = 0;
  while(true){
    //i++;
    //if(i%1000) Serial.println("outside i: "+ i);
   // if(i==100000) break;
   while(esp8266Module.available() > 0){
       //Serial.println("inside:"+ i);
       char c = esp8266Module.read();
       Serial.print(c);
       if(c == '#'){
        poundCnt++;
       }
       if(poundCnt == 1){
        result += c;
       }
       if(poundCnt == 2){
        //Serial.println("inside:"+ i);
        break;
       }  
   }
    if(poundCnt == 2){
        Serial.println("inside:"+ i);
        poundCnt = 0;
        break;
    }  
   
  }
   //delay(100);
  Serial.println(result);
  if(result.indexOf("on") != -1){
    ledTurnOn();
    }else{
      ledTurnOff();
    }
  
  

}

void TCPConnect(String URL) {
//   String connectStatus = myexec("AT+CIPCLOSE=?");
//   if (connectStatus.indexOf("OK") == -1) {
//      myexec("AT+CIPCLOSE");
//   }
    myexec("AT+CIPMUX=0");
  
    while(CONNECTED == false){
       String res = myexec("AT+CIPSTART=\"TCP\",\""+URL+"\",80",2000);
       res.toLowerCase();
       if(res.indexOf("error") == -1){
            CONNECTED = true;
            break;
       }
    }
  
}
void clearBuffer(){
  while(esp8266Module.available() > 0){
    esp8266Module.read();
    
  }
}
void connectToWifi(String ssid, String password) {
  myexec("AT+CWQAP");
  myexec("AT+CWJAP=\""+ssid+"\",\""+password+"\"");
  
}
void ledTurnOn()
{
  digitalWrite(ledPin, HIGH);              
}
void ledTurnOff() {
  digitalWrite(ledPin, LOW);
}
void sendmes() {
  
}

