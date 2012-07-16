#include <SPI.h>
#include <Ethernet.h> // version IDE 0022

#include <Z_OSC.h>


byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myIp[]  = { 192, 168, 69, 90 };
int  serverPort = 8000;
int led1 = 9;          // Choose led9, which also a debug led on the board
int led2 = 6;
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

Z_OSCServer server;

Z_OSCMessage *rcvMes;

void setup(){
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    Serial.begin(19200);
    Serial.println("Starting");
    Ethernet.begin(myMac ,myIp);
    server.sockOpen(serverPort);

}

void loop(){

    if(server.available()){

        rcvMes=server.getMessage();
        float value;

        value = rcvMes->getFloat(0);
        String address = String(rcvMes->getZ_OSCAddress());
        Serial.println(address);
        Serial.println(address.equals("/bearstech/hans"));
        brightness = int(value * 255);
        // problème d'arrondi, forçage de la valeur 1 envoyée par osc à 255 pour éviter que les led ne s'éteignent lorsque le slider est au max 
        if (value == 1) {
            brightness = 255;
        }
        Serial.println(brightness);
        if (address.equals("/bearstech/hans")) {
            analogWrite(led1, brightness);
            analogWrite(led2, brightness);
        }
    }
}


void logMessage(){
    uint16_t i;
    byte *ip=rcvMes->getIpAddress();

    long int intValue;
    float floatValue;
    char *stringValue;

    Serial.print(ip[0],DEC);
    Serial.print(".");
    Serial.print(ip[1],DEC);
    Serial.print(".");
    Serial.print(ip[2],DEC);
    Serial.print(".");
    Serial.print(ip[3],DEC);
    Serial.print(":");

    Serial.print(rcvMes->getPortNumber());
    Serial.print(" ");
    Serial.print(rcvMes->getZ_OSCAddress());
    Serial.print(" ");
    Serial.print(rcvMes->getTypeTags());
    Serial.print("--");

    for(i=0 ; i<rcvMes->getArgsNum(); i++){

        switch( rcvMes->getTypeTag(i) ){

            case 'i':
                intValue = rcvMes->getInteger32(i);

                Serial.print(intValue);
                Serial.print(" ");
                break;


            case 'f':
                floatValue = rcvMes->getFloat(i);

                Serial.print(floatValue);
                Serial.print(" ");
                break;


            case 's':
                stringValue = rcvMes->getString(i);

                Serial.print(stringValue);
                Serial.print(" ");
                break;

        }


    }
    Serial.println("");
}

