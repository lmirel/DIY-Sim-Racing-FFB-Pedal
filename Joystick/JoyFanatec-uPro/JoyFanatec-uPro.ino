/*
  Software serial multiple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.
12:58:23.904 -> HS recv bytes:1 : A 
12:58:23.904 -> HS recv bytes:1 : 5 
12:58:23.904 -> HS recv bytes:1 : 7B 2 FF 0 0 0 0 0 0 0 26 7D 
12:58:23.904 -> HS recv bytes:1 : 7B 0 0 0 0 0 0 0 0 0 AA 7D 
12:58:23.904 -> HS recv bytes:1 : 7B 3 0 0 0 0 0 0 0 0 5F 7D 
12:58:23.904 -> HS sent bytes: 36
12:58:23.904 -> HS done.
12:58:23.904 -> 
12:58:23.904 -> HS recv bytes:12 : 7B 10 0 0 0 0 0 0 0 0 10 7D 
12:58:24.366 -> HS recv bytes:12 : 7B 0 0 0 0 0 0 0 0 0 AA 7D 
12:58:24.366 -> HS recv bytes:12 : 7B 3 0 0 0 0 0 0 0 0 5F 7D 
12:58:24.875 -> HS recv bytes:12 : 7B 0 0 0 0 0 0 0 0 0 AA 7D 
12:58:24.875 -> HS recv bytes:12 : 7B 3 0 0 0 0 0 0 0 0 5F 7D 
12:58:25.392 -> HS recv bytes:12 : 7B 0 0 0 0 0 0 0 0 0 AA 7D 
12:58:25.392 -> HS recv bytes:12 : 7B 3 0 0 0 0 0 0 0 0 5F 7D 
12:58:25.883 -> HS recv bytes:13 : 7B 0 0 0 0 0 0 0 0 0 AA 7D 
12:58:25.883 -> HS recv bytes:12 : 7B 3 0 0 0 0 0 0 0 0 5F 7D 
12:58:26.398 -> HS recv bytes:12 : 7B 10 0 0 0 0 0 0 0 0 10 7D 
12:58:26.398 -> HS recv bytes:12 : 7B 0 0 0 0 0 0 0 0 0 AA 7D 
12:58:26.398 -> HS recv bytes:12 : 7B 3 0 0 0 0 0 0 0 0 5F 7D 
12:58:26.894 -> HS recv bytes:12 : 7B 0 0 0 0 0 0 0 0 0 AA 7D 
12:58:26.894 -> HS recv bytes:12 : 7B 3 0 0 0 0 0 0 0 0 5F 7D 
12:58:27.410 -> HS recv bytes:12 : 7B 0 0 0 0 0 0 0 0 0 AA 7D 
12:58:27.410 -> HS recv bytes:12 : 7B 3 0 0 0 0 0 0 0 0 5F 7D 
12:58:27.925 -> HS recv bytes:12 : 7B 0 0 0 0 0 0 0 0 0 AA 7D 
12:58:27.925 -> HS recv bytes:12 : 7B 3 0 0 0 0 0 0 0 0 5F 7D 
12:58:28.392 -> HS recv bytes:12 : 7B 0 0 0 0 0 0 0 0 0 AA 7D 
12:58:28.392 -> HS recv bytes:12 : 7B 3 0 0 0 0 0 0 0 0 5F 7D 
12:58:28.910 -> HS recv bytes:12 : 7B 10 0 0 0 0 0 0 0 0 10 7D 


 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 18); // RX, TX

// https://github.com/lmirel/fanatec-pedal-emulator

unsigned char hs_rx_0a[] = {0x0a};
unsigned char hs_tx_0a[] = {0x1a};
unsigned char hs_rx_05[] = {0x05};
unsigned char hs_tx_05[] = {0x15};
unsigned char hs_rx_7b[] = {
				0x7B, 0x02, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x7D,
				0x7B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0x7D,
				0x7B, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5F, 0x7D,
			};
unsigned char hs_tx_7b[] = {
				0x7B, 0x05, 0x06, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6D, 0x7D,
				0x7B, 0x07, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x7D,
				0x7B, 0x08, 0x01, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0x7D,
			};

void make_crc_table(unsigned char ch);


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(250000);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("");
  Serial.print("sizeof char: ");
  Serial.println(sizeof(char));
  Serial.print("sizeof short: ");
  Serial.println(sizeof(short));
  Serial.print("sizeof int: ");
  Serial.println(sizeof(int));
  Serial.print("sizeof long: ");
  Serial.println(sizeof(long));
  Serial.print("sizeof long long: ");
  Serial.println(sizeof(long long));
  Serial.print("sizeof float: ");
  Serial.println(sizeof(float));
  Serial.print("sizeof double: ");
  Serial.println(sizeof(double));
  make_crc_table(0x8c);
  Serial.println("Fanatec HS start");

  // set the data rate for the SoftwareSerial port
  Serial1.begin(250000);
  //mySerial.println("Hello, world?");
}

unsigned char crc_table[256];
//CRC table: 
//0x0, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83, 0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41, 0x9D, 0xC3, 0x21, 0x7F, 0xFC, 0xA2, 0x40, 0x1E, 0x5F, 0x1, 0xE3, 0xBD, 0x3E, 0x60, 0x82, 0xDC, 0x23, 0x7D, 0x9F, 0xC1, 0x42, 0x1C, 0xFE, 0xA0, 0xE1, 0xBF, 0x5D, 0x3, 0x80, 0xDE, 0x3C, 0x62, 0xBE, 0xE0, 0x2, 0x5C, 0xDF, 0x81, 0x63, 0x3D, 0x7C, 0x22, 0xC0, 0x9E, 0x1D, 0x43, 0xA1, 0xFF, 0x46, 0x18, 0xFA, 0xA4, 0x27, 0x79, 0x9B, 0xC5, 0x84, 0xDA, 0x38, 0x66, 0xE5, 0xBB, 0x59, 0x7, 0xDB, 0x85, 0x67, 0x39, 0xBA, 0xE4, 0x6, 0x58, 0x19, 0x47, 0xA5, 0xFB, 0x78, 0x26, 0xC4, 0x9A, 0x65, 0x3B, 0xD9, 0x87, 0x4, 0x5A, 0xB8, 0xE6, 0xA7, 0xF9, 0x1B, 0x45, 0xC6, 0x98, 0x7A, 0x24, 0xF8, 0xA6, 0x44, 0x1A, 0x99, 0xC7, 0x25, 0x7B, 0x3A, 0x64, 0x86, 0xD8, 0x5B, 0x5, 0xE7, 0xB9, 0x8C, 0xD2, 0x30, 0x6E, 0xED, 0xB3, 0x51, 0xF, 0x4E, 0x10, 0xF2, 0xAC, 0x2F, 0x71, 0x93, 0xCD, 0x11, 0x4F, 0xAD, 0xF3, 0x70, 0x2E, 0xCC, 0x92, 0xD3, 0x8D, 0x6F, 0x31, 0xB2, 0xEC, 0xE, 0x50, 0xAF, 0xF1, 0x13, 0x4D, 0xCE, 0x90, 0x72, 0x2C, 0x6D, 0x33, 0xD1, 0x8F, 0xC, 0x52, 0xB0, 0xEE, 0x32, 0x6C, 0x8E, 0xD0, 0x53, 0xD, 0xEF, 0xB1, 0xF0, 0xAE, 0x4C, 0x12, 0x91, 0xCF, 0x2D, 0x73, 0xCA, 0x94, 0x76, 0x28, 0xAB, 0xF5, 0x17, 0x49, 0x8, 0x56, 0xB4, 0xEA, 0x69, 0x37, 0xD5, 0x8B, 0x57, 0x9, 0xEB, 0xB5, 0x36, 0x68, 0x8A, 0xD4, 0x95, 0xCB, 0x29, 0x77, 0xF4, 0xAA, 0x48, 0x16, 0xE9, 0xB7, 0x55, 0xB, 0x88, 0xD6, 0x34, 0x6A, 0x2B, 0x75, 0x97, 0xC9, 0x4A, 0x14, 0xF6, 0xA8, 0x74, 0x2A, 0xC8, 0x96, 0x15, 0x4B, 0xA9, 0xF7, 0xB6, 0xE8, 0xA, 0x54, 0xD7, 0x89, 0x6B, 0x35

void make_crc_table(unsigned char ch)
{
  Serial.println("CRC table: ");
  for (int i = 0; i < 256; i++)
  {
    unsigned char crc = i;
    for (int j = 0; j < 8; j++)
    {
      unsigned char bit = ((crc & 0x01) != 0);
      crc >>= 1;
      if (bit)
        crc ^= ch;
    }
    crc_table[i] = crc;
    Serial.print(", 0x");
    Serial.print(crc_table[i], HEX);
  }
  Serial.println("");
}

unsigned char gen_crc(unsigned char *buf, int bl)
{
  unsigned char crc = 0xff;
  for (int i = 0; i < bl; i++)
  {
    crc = crc_table[buf[i]^crc];
  }
  return crc;
}
unsigned char pedals[] = {
  0x7b, 
  0x01, 
  0x10, 0xf0, //throttle
  0x00, 0xff, //brake
  0x10, 0x30, //clutch
  0x10, 0x40, //handbrake
  0x00,       //CRC
  0x7d
  };
#if 0//CRC
func make_le_table(poly byte) []byte {
	table := make([]byte, 256)
	for i := 0; i < 256; i++ {
		crc := byte(i)
		for j := 0; j < 8; j++ {
			bit := (crc & 0x01) != 0
			crc >>= 1
			if bit {
				crc ^= poly
			}
		}
		table[i] = crc
	}
	return table
}

var crc_table = make_le_table(0x8c)

func GenerateCRC(input []byte) byte {
	var crc byte = 0xff

	for _, b := range input {
		crc = crc_table[b^crc]
	}

	return crc
}

type Pedals struct {
	Throttle  uint16
	Brake     uint16
	Clutch    uint16
	Handbrake uint16
}

func (p *Pedals) CreatePacket() []byte {

	buf := make([]byte, 0, 11)

	buf = append(buf, 0x7b)
	buf = append(buf, 0x01) // send the pedals
	buf = binary.LittleEndian.AppendUint16(buf, p.Throttle)
	buf = binary.LittleEndian.AppendUint16(buf, p.Brake)
	buf = binary.LittleEndian.AppendUint16(buf, p.Clutch)
	buf = binary.LittleEndian.AppendUint16(buf, p.Handbrake)

	crc := GenerateCRC(buf[1:])

	buf = append(buf, crc)
	buf = append(buf, 0x7d)

	return buf
}
#endif

#define HS_STEP_0A 0
#define HS_STEP_05 1
#define HS_STEP_7B 2
#define HS_STEP_OK 3

#define HS_BTS_0A 1
#define HS_BTS_0A 1
#define HS_BTS_7B 36
char hs_step = HS_STEP_0A;

//return 1 until we finish processing handshake
int hs_process()
{
  int nb = Serial1.available();
  static int ndb = HS_BTS_7B;
  static int rdb = 0;
  byte ch = 0;

  if (hs_step == HS_STEP_OK)
  {
    if (nb >= 12)
    {
      Serial.println("");
      Serial.print("HS recv bytes:");
      Serial.print(nb);
      Serial.print(" : ");
      ch = Serial1.read();
      Serial.print(ch, HEX);
      Serial.print(" ");
      if (ch == 0x7b)
      {
        ch = Serial1.read();
        Serial.print(ch, HEX);
        Serial.print(" ");
        switch (ch)
        {
          case 0x03:
            Serial1.write(hs_tx_7b+24, 12);
            break;
          case 0x00:
            Serial1.write(hs_tx_7b+12, 12);
            break;
          case 0x10:
          case 0x02://it is over!
          default:
            ;//nada
        }
      }
      else
      {
        //reset HS
          Serial.println("HS error: restaring");
          Serial1.end();
          Serial1.begin(115200);
          hs_step = HS_STEP_0A;
          return 1;
      }
      for (int i = 0; i < 10; i++)
      {
        ch = Serial1.read();
        Serial.print(ch, HEX);
        Serial.print(" ");
      }
    }
    return 0;
  }
  else if (nb == 0)
    return 1;

  Serial.println("");
  Serial.print("HS recv bytes:");
  Serial.print(nb);
  Serial.print(" : ");

  switch (hs_step)
  {
    case HS_STEP_0A:
      ch = Serial1.read();
      Serial.print(ch, HEX);
      Serial.print(" ");
      if (ch == 0x0a)
      {
        Serial1.write(0x1a);
        hs_step = HS_STEP_05;
      }
    break;
    case HS_STEP_05:
      ch = Serial1.read();
      Serial.print(ch, HEX);
      Serial.print(" ");
      if (ch == 0x05)
      {
        Serial1.write(0x15);
        hs_step = HS_STEP_7B;
        //change baudrate to 115200
        Serial1.end();
        Serial1.begin(115200);
        ndb = HS_BTS_7B;
        rdb = 0;
      }
    break;
    case HS_STEP_7B:
      while (Serial1.available() && ndb)
      {
        ch = Serial1.read();
        Serial.print(ch, HEX);
        Serial.print(" ");
        if (ch != hs_rx_7b[rdb])
        {
          Serial.println("HS error: restaring");
          Serial1.end();
          Serial1.begin(115200);
          hs_step = HS_STEP_0A;
          return 1;
        }
        rdb++;
        ndb--;
      }
      if (ndb == 0)
      {
        nb = Serial1.write(hs_tx_7b, sizeof(hs_tx_7b));
        hs_step = HS_STEP_OK;
        Serial.println("");
        Serial.print("HS sent bytes: ");
        Serial.println(nb);
        Serial.println("HS done.");
      }
    break;
  }
  return 1;
}

unsigned short gpv = 0;
unsigned char ppv=0;
void loop() { // run over and over
  unsigned short *pv = (unsigned short *)(pedals+2);
  int wb = 0;
  if (hs_process() == 0)
  {
    //*pv = gpv++;
    pedals[2]=0;
    pedals[3]=ppv++;
    //send pedal data
    pedals[10] = gen_crc(pedals + 1, sizeof(pedals) - 1);
    wb = Serial1.write(pedals, sizeof(pedals));
    //delay(50);
    Serial.print("WB: wrote ");
    Serial.print(ppv, DEC);
    Serial.print(" : bytes ");
    Serial.println(wb, DEC);
  }
}
