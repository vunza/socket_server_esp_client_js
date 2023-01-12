/*------------------------------------------------------------------------------
  07/01/2018
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: webserver_websockets.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to transfer data between a web server
  and a web client in real-time using websockets.
  https://youtu.be/ROeT-gyYZfw

  Do you like my videos? You can support the channel:
  https://patreon.com/acrobotic
  https://paypal.me/acrobotic
  ------------------------------------------------------------------------------
  Please consider buying products from ACROBOTIC to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use. 

  https://acrobotic.com/
  https://amazon.com/acrobotic
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

//ESP8266WebServer server;
WebSocketsServer webSocket = WebSocketsServer(3000);

#define PIN_LED 2

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){

  if(type == WStype_TEXT){

    if(payload[0] == '#'){     
     
      uint16_t brightness = (uint16_t) strtol((const char *) &payload[1], NULL, 10);
      brightness = 1024 - brightness;
      Serial.print("brightness = ");
      Serial.println(brightness);
            
    }
    else{

      if( strcmp( (const char*)payload, "toggle") == 0){
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));

        char estado[4];

        if(digitalRead(PIN_LED)){ // (*J*) - ESP12e -- lógica invertida
          strncpy(estado, "OFF", sizeof(estado));
        }
        else{
          strncpy(estado, "ON", sizeof(estado));
        }

        webSocket.broadcastTXT(estado);
      }

      Serial.println((const char*)payload);
    }
  }
  
}


const char* ssid = "Welwitschia Mirabilis";
const char* password = "tigre?2018@";
//const char* ssid = "TPLINK";
//const char* password = "gregorio@2012";


/*char webpage[] PROGMEM = R"=====(
<html>
<head>
  <script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':3000/');
      Socket.onmessage = function(event){
        document.getElementById("rxConsole").value += event.data;
      }
    }
    function sendText(){
      Socket.send(document.getElementById("txBar").value);
      document.getElementById("txBar").value = "";
    }
    function sendBrightness(){
      Socket.send("#"+document.getElementById("brightness").value);
    }    
  </script>
</head>
<body onload="javascript:init()">
  <div>
    <textarea id="rxConsole"></textarea>
  </div>
  <hr/>
  <div>
    <input type="text" id="txBar" onkeydown="if(event.keyCode == 13) sendText();" />
  </div>
  <hr/>
  <div>
    <input type="range" min="0" max="1023" value="512" id="brightness" oninput="sendBrightness()" />
  </div>  
</body>
</html>
)=====";*/


void setup(){

  WiFi.begin(ssid,password);

  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);

  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  /*server.on("/",[](){
    server.send_P(200, "text/html", webpage);  
  });*/

  //server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop(){

  webSocket.loop();
  //server.handleClient();

  if(Serial.available() > 0){
    char c[] = {(char)Serial.read()};
    webSocket.broadcastTXT(c, sizeof(c));
  }

}

