#include <SPI.h>
#include <Ethernet.h> // version IDE 0022
#include <Z_OSC.h>


byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC };
byte myIp[]  = { 192, 168, 69, 91 };
int  serverPort = 8000;
int more = 7;           // Choose led9, which also a debug led on the board
int less = 8;
//int off = 2;
int actual_value = 0;
int wait_time = 500;

Z_OSCServer server;

Z_OSCMessage *rcvMes;

void setup(){
    pinMode(more, OUTPUT);
    pinMode(less, OUTPUT);
    //pinMode(off, OUTPUT);
    Serial.begin(19200);
    Serial.println("Starting");
    //digitalWrite(off, HIGH);
    delay(wait_time);
    digitalWrite(more, LOW);
    digitalWrite(less, LOW);
    Ethernet.begin(myMac ,myIp);
    server.sockOpen(serverPort);
}

void loop(){

    if(server.available()){

        rcvMes=server.getMessage();
        int value;

        //value = int(25 * rcvMes->getFloat(0));
        value = int(rcvMes->getInteger32(0));
        String address = String(rcvMes->getZ_OSCAddress());
        Serial.println(address);
        Serial.println(value);

        //if (address.equals("/bearstech/ceiling")) { 
        //    if (value > actual_value) {
        //        for (int a = actual_value; a <= value; a ++) {
        //            digitalWrite(more, HIGH);
        //            Serial.println("+");
        //            delay(wait_time);
        //            digitalWrite(more, LOW);
        //            delay(wait_time);
        //        }
        //        actual_value = value;
        //    }
        //    if (value < actual_value) {
        //        for (int a = actual_value; a >= value; a --) {
        //            digitalWrite(less, HIGH);
        //            Serial.println("-");
        //            delay(wait_time);
        //            digitalWrite(less, LOW);
        //            delay(wait_time);
        //        }
        //        actual_value = value;
        //    }

        if (address.equals("/bearstech/ceiling/more")) {
            if (value == 1) {
                digitalWrite(more, HIGH);
                Serial.println("+");
                delay(wait_time);
                digitalWrite(more, LOW);
                delay(wait_time);
            }
        }
        if (address.equals("/bearstech/ceiling/less")) {
            if (value == 1) {
                digitalWrite(less, HIGH);
                Serial.println("-");
                delay(wait_time);
                digitalWrite(less, LOW);
                delay(wait_time);
            }
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

