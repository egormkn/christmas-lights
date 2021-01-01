#include <Arduino.h>
#include <Dhcp.h>
#include <Ethernet.h>
#include <FastLED.h>
#include <RTClib.h>
#include <Wire.h>

#define WIDTH  20
#define HEIGHT 10

#define NUM_LEDS (WIDTH * HEIGHT)

#define DATA_PIN 6

#define USE_SERIAL 0

typedef uint32_t Timestamp;

Timestamp old_timestamp;

// #pragma pack(push, 1)
static union {
  struct Data { 
    Timestamp current_timestamp;
    CRGB leds[NUM_LEDS];
  } data;
  byte buffer[sizeof(data.current_timestamp) + sizeof(data.leds)];
};
// #pragma pack(pop)

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

unsigned int localPort = 8888; // Local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // Initialize leds
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(data.leds, NUM_LEDS).setCorrection(TypicalPixelString);
  FastLED.setBrightness(20);
  FastLED.clear(true);
  FastLED.show();

#if USE_SERIAL
  // Open serial communications and wait for port to open
  Serial.begin(9600);

  // Wait for serial port to connect. Needed for native USB port only
  while (!Serial) {}
#endif 
  // You can use Ethernet.init(pin) to configure the CS pin
  // Ethernet.init(10);  // Most Arduino shields
  // Ethernet.init(5);   // MKR ETH shield
  // Ethernet.init(0);   // Teensy 2.0
  // Ethernet.init(20);  // Teensy++ 2.0
  // Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  // Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // Start the Ethernet connection:
#if USE_SERIAL
  Serial.println("Initialize Ethernet with DHCP:");
#endif
  if (Ethernet.begin(mac) == 0) {
#if USE_SERIAL
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected");
    }
#endif
    // No point in carrying on, so do nothing forevermore:
    while (true) {
      delay(1);
    }
  }

#if USE_SERIAL
  // Print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
#endif

  // Start UDP
  Udp.begin(localPort);
}

void loop() {
  switch (Ethernet.maintain()) {
#if USE_SERIAL
    case DHCP_CHECK_RENEW_FAIL:
      Serial.println("Error: renewed fail");
      break;

    case DHCP_CHECK_RENEW_OK:
      Serial.println("Renewed success");
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    case DHCP_CHECK_REBIND_FAIL:
      Serial.println("Error: rebind fail");
      break;

    case DHCP_CHECK_REBIND_OK:
      Serial.println("Rebind success");
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;
#endif

    default: // Nothing happened
      break;
  }

  // If there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
#if USE_SERIAL
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; ++i) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());
#endif
    // Read the packet into buffer
    Udp.read(buffer, sizeof(buffer));
    FastLED.show();
    //if (data.current_timestamp > old_timestamp) {
      // old_timestamp = data.current_timestamp;
      // Serial.println(old_timestamp);
      
    //}

    // Send a reply to the IP address and port that sent us the packet we received
    // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    // Udp.write(ReplyBuffer);
    // Udp.endPacket();
  }
}
