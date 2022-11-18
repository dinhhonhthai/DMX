#include <Arduino.h>
#include <Conceptinetics.h>
#define DMX_MASTER_CHANNELS   512 
#define RXEN_PIN                2
DMX_Master        dmx_master ( DMX_MASTER_CHANNELS, RXEN_PIN );
#include <EtherCard.h>
#include <IPAddress.h>
// My IP address
static byte myip[] = { 192,168,4,10 };
// Gateway IP addresukms
static byte gwip[] = { 192,168,4,2 };


// Ethernet MAC address - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[520]; // TCP/IP send and receive buffer

// Callback that prints received packets to the serial port
void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len){
  int val1 = data[0]+data[1];
  int val2 =  data[519]+data[520];
  if(val1 == val2){
   for (uint16_t i = 0; i <521; i++){
  dmx_master.setChannelValue ( i+1, data[i]  );
//  delay(10);
}
  }
}
void setup() {             
  dmx_master.enable ();  
   if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
   // Serial.println(F("Failed to access Ethernet controller"));
     dmx_master.setChannelValue ( 2,  100 );
     ether.staticSetup(myip, gwip);
     ether.udpServerListenOnPort(&udpSerialPrint, 96);
}
void loop() 
{
  ether.packetLoop(ether.packetReceive());

}