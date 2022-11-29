#include <Arduino.h>
#include <Conceptinetics.h>
#define DMX_MASTER_CHANNELS   512 
#define RXEN_PIN                7


#include <EtherCard.h>
#include <IPAddress.h>
#include <EEPROM.h>
unsigned char cmd;
byte nhan,slave;
String add;
String tem_id;
boolean check_setid = false;
// My IP address
// static byte myip[] = { 192,168,1,200 };
// Gateway IP addresukms
static byte gwip[] = { 192,168,1,2 };
static char set_id[13]={'?','S','e','t','a','d','d','r','e','s','s','^','*'};
//16
 
// Ethernet MAC address - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };
 DMX_Master        dmx_master ( DMX_MASTER_CHANNELS, RXEN_PIN );
byte Ethernet::buffer[550]; // TCP/IP send and receive buffer
// slave = EEPROM.read(200);
// slave = 0;
// Callback that prints received packets to the serial port
void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len){
  unsigned long val1 = data[0]+data[1] + data[2]+data[3];
  unsigned long val2 =  data[517]+data[518]+data[519]+data[520];
  
    if (val1 == val2)
    {
      for (uint16_t i = 4; i < 517; i++)
      {
        dmx_master.setChannelValue(i - 3, data[i]);
      }
    }
 

  for (int i = 0; i < 12; i++)
  {
    if (data[i] != set_id[i])
      break;
    if (i == 11 && data[13] == set_id[12])
    {
      slave = (atoi(&data[12]));
      if (slave < 11 && slave > 0){
       slave = slave + 100;
      EEPROM.write(200, slave);
      }
      else{
       slave = 200;
         EEPROM.write(200, slave);
      }
    }
  }
}



void setup() {
  // EEPROM.write(200,200);
  // delay(10);
   pinMode(7,OUTPUT);
// Serial1.begin(9600);

  slave = EEPROM.read(200);
  if (slave > 110) slave = 200;
   static byte myip[] = { 192,168,1,slave };

  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    dmx_master.enable();
    // dmx_master.setChannelValue(2, 100);
  ether.staticSetup(myip, gwip);
  ether.udpServerListenOnPort(&udpSerialPrint, 96);
  ether.udpServerListenOnPort(&udpSerialPrint, 69);
}
void loop() 
{
  ether.packetLoop(ether.packetReceive());


}