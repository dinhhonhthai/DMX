#include <SPI.h>

#include <EtherCard.h>
#include <IPAddress.h>
//#include <BlynkSimpleEsp8266.h>

// My IP address
static byte myip[] = { 192,168,1,200 };

// Gateway IP addresukms
static byte gwip[] = { 192,168,1,2 };


// Ethernet MAC address - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[1024]; // TCP/IP send and receive buffer

// Callback that prints received packets to the serial port
void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len){
  Serial.write(data,len);
}

void setup(){
  Serial.begin(250000);
  pinMode(7,OUTPUT);
  digitalWrite(7,1);
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));

  ether.staticSetup(myip, gwip);

  // ether.printIp("IP:  ", ether.myip);
  // ether.printIp("GW:  ", ether.gwip);
  // ether.printIp("DNS: ", ether.dnsip);

  // Register udpSerialPrint() to port 1337
  ether.udpServerListenOnPort(&udpSerialPrint, 96);

  // Register udpSerialPrint() to port 42.
  ether.udpServerListenOnPort(&udpSerialPrint, 69);
}

void loop(){
  // This must be called for ethercard functions to work.
  ether.packetLoop(ether.packetReceive());
}