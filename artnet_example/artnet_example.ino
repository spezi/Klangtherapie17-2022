#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define WSout1 4 
#define WSout2 4 
#define WSout3 4
#define WSout4 4 

#define WSbit1 (1<<WSout1)
#define WSbit2 (1<<WSout2)
#define WSbit3 (1<<WSout3)
#define WSbit4 (1<<WSout4) 

// ARTNET CODES
#define ARTNET_DATA 0x50
#define ARTNET_POLL 0x20
#define ARTNET_POLL_REPLY 0x21
#define ARTNET_PORT 6454
#define ARTNET_HEADER 17

WiFiUDP udp;

//universe 0
uint8_t uniData1[510];
uint8_t universe1 = 0;
uint16_t uniSize1;

//universe 1
uint8_t uniData2[510];
uint8_t universe2 = 1;
uint16_t uniSize2;

//universe 3
uint8_t uniData3[510];
uint8_t universe3 = 2;
uint16_t uniSize3;

//universe 4
uint8_t uniData4[510];
uint8_t universe4 = 3;
uint16_t uniSize4;

//universe 5
uint8_t uniData5[510];
uint8_t universe5 = 4;
uint16_t uniSize5;

uint8_t hData[ARTNET_HEADER + 1];
uint8_t net = 0;
uint8_t subnet = 0;

const char* ssid     = "--";
const char* password = "--";


IPAddress local_ip(192, 168, 1, 237);
IPAddress gateway_ip(192, 168, 1, 1);
IPAddress subnet_ip(255, 255, 255, 0);

void setup() {

  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); 
    WiFi.config(local_ip, gateway_ip, subnet_ip);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  udp.begin(ARTNET_PORT); // Open ArtNet port

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  pinMode(WSout1, OUTPUT); 
  pinMode(LED_BUILTIN, OUTPUT); 
  
  
}


void   sendWS() {
  uint32_t writeBits;
  uint8_t  bitMask, time;
  os_intr_lock();
  for (uint16_t t = 0; t < uniSize1; t++) { // outer loop counting bytes
    bitMask = 0x80;
    while (bitMask) {
      // time=0ns : start by setting bit on
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000304, WSbit1 );  // do ON bits // T=0
      }
      if ( uniData1[t] & bitMask ) {
        writeBits = 0;  // if this is a '1' keep the on time on for longer, so dont write an off bit
      }
      else {
        writeBits = WSbit1;  // else it must be a zero, so write the off bit !
      }
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, writeBits );  // do OFF bits // T='0' time 350ns
      }
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, WSbit1 );  // switch all bits off  T='1' time 700ns
      }
      // end of bite write time=1250ns
      bitMask >>= 1;
    }
  }
    for (uint16_t t = 0; t < uniSize2; t++) { // outer loop counting bytes
    bitMask = 0x80;
    while (bitMask) {
      // time=0ns : start by setting bit on
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000304, WSbit2 );  // do ON bits // T=0
      }
      if ( uniData2[t] & bitMask ) {
        writeBits = 0;  // if this is a '1' keep the on time on for longer, so dont write an off bit
      }
      else {
        writeBits = WSbit2;  // else it must be a zero, so write the off bit !
      }
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, writeBits );  // do OFF bits // T='0' time 350ns
      }
      time =6;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, WSbit2 );  // switch all bits off  T='1' time 700ns
      }
      // end of bite write time=1250ns
      bitMask >>= 1;
    }
  }
    for (uint16_t t = 0; t < uniSize3; t++) { // outer loop counting bytes
    bitMask = 0x80;
    while (bitMask) {
      // time=0ns : start by setting bit on
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000304, WSbit3 );  // do ON bits // T=0
      }
      if ( uniData3[t] & bitMask ) {
        writeBits = 0;  // if this is a '1' keep the on time on for longer, so dont write an off bit
      }
      else {
        writeBits = WSbit3;  // else it must be a zero, so write the off bit !
      }
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, writeBits );  // do OFF bits // T='0' time 350ns
      }
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, WSbit3 );  // switch all bits off  T='1' time 700ns
      }
      // end of bite write time=1250ns
      bitMask >>= 1;
    }
  }
    for (uint16_t t = 0; t < uniSize4; t++) { // outer loop counting bytes
    bitMask = 0x80;
    while (bitMask) {
      // time=0ns : start by setting bit on
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000304, WSbit4 );  // do ON bits // T=0
      }
      if ( uniData4[t] & bitMask ) {
        writeBits = 0;  // if this is a '1' keep the on time on for longer, so dont write an off bit
      }
      else {
        writeBits = WSbit4;  // else it must be a zero, so write the off bit !
      }
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, writeBits );  // do OFF bits // T='0' time 350ns
      }
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, WSbit4 );  // switch all bits off  T='1' time 700ns
      }
      // end of bite write time=1250ns
      bitMask >>= 1;
    }
  }
  os_intr_unlock();
}

 
void loop() {
  if (udp.parsePacket()) {
  //  digitalWrite(LED_BUILTIN,LOW);
    udp.read(hData, ARTNET_HEADER + 1);
      if ( hData[8] == 0x00 && hData[9] == ARTNET_DATA && hData[15] == net ) {

        if ( hData[14] == (subnet << 4) + universe1 ) { // UNIVERSE One
          if (!uniSize1) {
            uniSize1 = (hData[16] << 8) + (hData[17]);
          }
          udp.read(uniData1, uniSize1);
        }

        if ( hData[14] == (subnet << 4) + universe2 ) { // UNIVERSE two
          if (!uniSize2) {
            uniSize2 = (hData[16] << 8) + (hData[17]);
          }
          udp.read(uniData2, uniSize2);
        }

        if ( hData[14] == (subnet << 4) + universe3 ) { // UNIVERSE THREEE
          if (!uniSize3) {
            uniSize3 = (hData[16] << 8) + (hData[17]);
          }
          udp.read(uniData3, uniSize3);
        }
        if ( hData[14] == (subnet << 4) + universe4 ) { // UNIVERSE FOUR
          if (!uniSize4) {
            uniSize4 = (hData[16] << 8) + (hData[17]);
          }
          udp.read(uniData4, uniSize4);
        }
          sendWS(); 
      } // if Artnet Data

}


    //digitalWrite(LED_BUILTIN,HIGH);
}
