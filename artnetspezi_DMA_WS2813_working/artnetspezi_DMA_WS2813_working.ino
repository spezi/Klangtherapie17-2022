
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>  
#include <ArduinoOTA.h>

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
const uint16_t PixelCount = 300;
const uint8_t PixelPin = 3; 
//NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2813Method> strip(PixelCount, PixelPin);
NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(PixelCount, PixelPin); //WS2812B

// ARTNET CODES
#define ARTNET_DATA 0x50
#define ARTNET_POLL 0x20
#define ARTNET_POLL_REPLY 0x21
#define ARTNET_PORT 6454
#define ARTNET_HEADER 17

#define TPM2_PORT 6454
#define MAX_BUFFER_TPM2 360
#define TPM2_START 18
uint8_t tpm2Packet[MAX_BUFFER_TPM2];
uint8_t packetSize;
uint8_t* data; 

// Network Settings
char ssid[] = "eyelan";  
char pass[] = "";    
     
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

void setup()
{

  Serial.begin(115200);
  delay(random(3000, 7000)); // random Delay for connection

  IPAddress ip(10, 0, 0, 10);
  IPAddress gateway(10, 0, 0, 1);
  IPAddress subnet(255, 255, 0, 0);
  WiFi.config(ip, gateway, subnet);
  //WiFi.begin(ssid, pass);
  WiFi.begin(ssid);
  WiFi.mode(WIFI_STA);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(random(200,500));
  }

  //Serial.println(WiFi.localIP());
  
  udp.begin(TPM2_PORT);
  strip.Begin();

  //let first pixel glow white to indicate connection
  strip.SetPixelColor(0, RgbColor (100, 100, 100));  
  strip.Show();

  
  //////////// OTA Stuff
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("LEDStripe10");

  // No authentication by default
  ArduinoOTA.setPassword("admin1234(#+--)");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  /////////////
  
}

void   sendStripe() {
  for (uint16_t t = 0; t < uniSize1; t++) { // outer loop counting bytes
      strip.SetPixelColor(t, RgbColor (uniData1[t], uniData1[(t + 1)], uniData1[(t + 2)]));  
  }

  strip.Show();
  
}
  

void loop()
{ 

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
          sendStripe(); 
      } // if Artnet Data
  }
  
 // if (WiFi.status() == WL_CONNECTED) {
 
 /*   packetSize = udp.parsePacket();
    if(packetSize) {
     udp.read(tpm2Packet, MAX_BUFFER_TPM2);
     data = tpm2Packet + TPM2_START;
       for (int i = 0; i < PixelCount; i++)
        {
        strip.SetPixelColor(i, RgbColor (data[(i * 3)], data[(i * 3 + 1)], data[(i * 3 + 2)]));  
        }   
        strip.Show();
  }*/
  /*
  else { // if connection is Lost
      IPAddress ip(192, 168, 0, 80);
      IPAddress gateway(192, 168, 0, 1);
      IPAddress subnet(255, 255, 255, 0);
      WiFi.config(ip, gateway, subnet);
      WiFi.begin(ssid, pass);
      WiFi.mode(WIFI_STA);
      delay(500);
      udp.begin(TPM2_PORT);  
  }
  */
  /////////OTA Stuff
  ArduinoOTA.handle();
  ////////
  
}
