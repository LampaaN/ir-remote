#include <IRLibDecodeBase.h>  //We need both the coding and
#include <IRLibSendBase.h> // sending base classes
#include <IRLib_P01_NEC.h>
#include <IRLib_P02_Sony.h>
#include <IRLib_P03_RC5.h>
#include <IRLib_P04_RC6.h>
#include <IRLib_P05_Panasonic_Old.h>
#include <IRLib_P07_NECx.h>
#include <IRLibCombo.h>
#include <IRLibRecv.h>
//#include <IRLibAll.h> //Low memory disabled

#define POWER 0
#define MUTE 1
#define SOURCE 3
#define CH1 4
#define UP 5
#define DOWN 6
#define RIGHT 7
#define LEFT 8
#define OK 9
#define LINE1 10
IRsend mySender;

IRdecode myDecoder;
IRrecv myReceiver(3); //Receiver pin 7

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("READY");
  myReceiver.enableIRIn();
}


const uint16_t inputLength = 512;
void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {

    static char input[inputLength];
    static uint16_t i;


    char c = Serial.read();

    if ( c != '\r' && c != '\n' && i < inputLength - 1) {
      input[i++] = c;
    } else {
      input[i] = '\0';
      i = 0;

      char* command = "TV";
      Serial.print("Command: ");
      Serial.println(input);
      if (!strncmp(input, command, strlen(command))) {
        int8_t cmd_ex = input[strlen(command) + 1] - 48; //ASCII
        Serial.println(cmd_ex);

        switch (cmd_ex) {
          case POWER:
            //Codes are 0x10XX or 0x18XX if you press it twice
            //mySender.send(RC6, 0xc, 13); //Standby (13)(0xc)
            
            mySender.send(RC6, 0x1000c, 13); //Standby (13)(0xc)
            Serial.println("POWER");
            break;
          case MUTE:
            mySender.send(RC6, 0xd, 13); //Mute (14)(0xd)
            Serial.println("MUTE");
            break;
          case SOURCE:
            mySender.send(RC6, 0x38, 13); //Switch RF inputs (40)(0x28) NOT SURE IF CORRECT FOR SOURCE
            Serial.println("CH1");
            break;
          case UP:
            mySender.send(RC6, 0x58, 13); //Channel 1 (1)(0x1)
            Serial.println("UP");
            break;
          case DOWN:
            mySender.send(RC6, 0x59, 13); //Channel 1 (1)(0x1)
            Serial.println("DOWN");
            break;
          case LEFT:
            mySender.send(RC6, 0x5A, 13); //Channel 1 (1)(0x1)
            Serial.println("LEFT");
            break;
          case RIGHT:
            mySender.send(RC6, 0x5B, 13); //Channel 1 (1)(0x1)
            Serial.println("DOWN");
            break;
          case OK:
            mySender.send(RC6, 0x5C, 13); //Channel 1 (1)(0x1)
            Serial.println("OK");
            break;
          case LINE1:
            mySender.send(RC6, 0xD1, 13); //Channel 1 (1)(0x1)
            Serial.println("LINE1");
            break;
        }
      }
      memset(input, '\0', sizeof(input));
    }


    //Must be 13 bits
    //mySender.send(RC5, 0x180c,13); //Standby (13)(0xc)
  }
  if (myReceiver.getResults()) {
    myDecoder.decode();
    uint8_t codeProtocol = myDecoder.protocolNum;
    uint32_t codeValue = myDecoder.value;
    //if (codeProtocol != UNKNOWN){
      //uint8_t codeBits = myDecoder.bits;
      Serial.print("Received value: 0x");
      Serial.print(codeValue, HEX);
      Serial.print(" with protocol: ");
      Serial.println(Pnames(codeProtocol));
      
   // }
    myReceiver.enableIRIn();
  }
}
