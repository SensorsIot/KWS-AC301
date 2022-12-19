//Test code to see if I can get the KWS-AC301L-100A power meter to respond to a MODBUS master quarry.

const byte numChars = 20;//none of the received hex byte requests from the power meter have more then 7 bytes. NOT TRUE
uint8_t receivedChars[numChars]; // an array to store the received data
uint8_t message[8] =  {0x02, 0x03, 0x00, 0x0E, 0x00, 0x01}; //transmit data to meter, element 3 is swapped with get_data[z].command_HEX
uint8_t size;//size of message to meter

byte result[8]; //result is stored in this array
boolean data_found;//looking for reply to a message
boolean newData;//looking for reply to a message
uint32_t time_to_reply;//what is message to reply time?

struct KWS_301L_command
{
  uint8_t command_HEX;
  uint8_t div_by_10;
  String command_type;
};

KWS_301L_command get_data[38];

void setup()
{
  Serial.begin(115200);
  Serial1.begin(9600); // USART0 for RS485 to AC301L-100A power meter on MOD BUS type interface
  Serial.println("<Arduino is ready from setup line 34>");
  get_data[0].command_HEX = 0x01;
  get_data[0].div_by_10 = 2;
  get_data[0].command_type = "Rated Voltage";

  get_data[1].command_HEX = 0x02;
  get_data[1].div_by_10 = 3;
  get_data[1].command_type = "Rated current";

  get_data[2].command_HEX = 0x03;
  get_data[2].div_by_10 = 1;
  get_data[2].command_type = "Active power under 0.5L";

  get_data[3].command_HEX = 0x04;
  get_data[3].div_by_10 = 1;
  get_data[3].command_type = "Reactive power under 0.5L";

  get_data[4].command_HEX = 0x05;
  get_data[4].div_by_10 = 0;
  get_data[4].command_type = "Hardware version low";

  get_data[5].command_HEX = 0x06;
  get_data[5].div_by_10 = 0;
  get_data[5].command_type = "Hardware version high";

  get_data[6].command_HEX = 0x07;
  get_data[6].div_by_10 = 0;
  get_data[6].command_type = "Software version low";

  get_data[7].command_HEX = 0x08;
  get_data[7].div_by_10 = 0;
  get_data[7].command_type = "Software version high";

  get_data[8].command_HEX = 0x09;
  get_data[8].div_by_10 = 0;
  get_data[8].command_type = "Protocol version low";

  get_data[9].command_HEX = 0x0A;
  get_data[9].div_by_10 = 0;
  get_data[9].command_type = "Protocol version high";

  get_data[10].command_HEX = 0x0B;
  get_data[10].div_by_10 = 0;
  get_data[10].command_type = "Module type";

  get_data[11].command_HEX = 0x0C;
  get_data[11].div_by_10 = 0;
  get_data[11].command_type = "comm addr & baud status";

  get_data[12].command_HEX = 0x0D;
  get_data[12].div_by_10 = 0;
  get_data[12].command_type = "Pulse constant";

  get_data[13].command_HEX = 0x0E;
  get_data[13].div_by_10 = 1;
  get_data[13].command_type = "Channel Voltage";

  get_data[14].command_HEX = 0x0F;
  get_data[14].div_by_10 = 3;
  get_data[14].command_type = "Channel current low";

  get_data[15].command_HEX = 0x10;
  get_data[15].div_by_10 = 3;
  get_data[15].command_type = "Channel current high";

  get_data[16].command_HEX = 0x11;
  get_data[16].div_by_10 = 1;
  get_data[16].command_type = "Channel active power low Watts";

  get_data[17].command_HEX = 0x12;
  get_data[17].div_by_10 = 4;
  get_data[17].command_type = "Channel active power high";

  get_data[18].command_HEX = 0x13;
  get_data[18].div_by_10 = 4;
  get_data[18].command_type = "Channel reactive power low";

  get_data[19].command_HEX = 0x14;
  get_data[19].div_by_10 = 1;
  get_data[19].command_type = "Channel reactive power high";

  get_data[20].command_HEX = 0x15;
  get_data[20].div_by_10 = 1;
  get_data[20].command_type = "Channel apparent power low";

  get_data[21].command_HEX = 0x16;
  get_data[21].div_by_10 = 4;
  get_data[21].command_type = "Channel apparent power high";

  get_data[22].command_HEX = 0x17;
  get_data[22].div_by_10 = 0;
  get_data[22].command_type = "Channel energy low";

  get_data[23].command_HEX = 0x18;
  get_data[23].div_by_10 = 3;
  get_data[23].command_type = "Channel energy high";

  get_data[24].command_HEX = 0x19;
  get_data[24].div_by_10 = 0;
  get_data[24].command_type = "Operating minutes";

  get_data[25].command_HEX = 0x1A;
  get_data[25].div_by_10 = 0;
  get_data[25].command_type = "Current external temperature";

  get_data[26].command_HEX = 0x1B;
  get_data[26].div_by_10 = 0;
  get_data[26].command_type = "Current internal temperature";

  get_data[27].command_HEX = 0x1C;
  get_data[27].div_by_10 = 0;
  get_data[27].command_type = "RTC battery voltage";

  get_data[28].command_HEX = 0x1D;
  get_data[28].div_by_10 = 2;
  get_data[28].command_type = "Power factor";

  get_data[29].command_HEX = 0x1E;
  get_data[29].div_by_10 = 1;
  get_data[29].command_type = "Voltage Frequency";

  get_data[30].command_HEX = 0x1F;
  get_data[30].div_by_10 = 0;
  get_data[30].command_type = "Alarm status byte";

  get_data[31].command_HEX = 0x28;
  get_data[31].div_by_10 = 0;
  get_data[31].command_type = "RTC clock year";

  get_data[32].command_HEX = 0x29;
  get_data[32].div_by_10 = 0;
  get_data[32].command_type = "RTC clock month";

  get_data[33].command_HEX = 0x2A;
  get_data[33].div_by_10 = 0;
  get_data[33].command_type = "RTC clock day";

  get_data[34].command_HEX = 0x2B;
  get_data[34].div_by_10 = 0;
  get_data[34].command_type = "RTC clock Hour";

  get_data[35].command_HEX = 0x2C;
  get_data[35].div_by_10 = 0;
  get_data[35].command_type = "RTC clock Minute";

  get_data[36].command_HEX = 0x2D;
  get_data[36].div_by_10 = 0;
  get_data[36].command_type = "RTC clock Second";

  get_data[37].command_HEX = 0x00;
  get_data[37].div_by_10 = 0;
  get_data[37].command_type = "Write parameter password";
}
int j;
void loop()
{
  /*
    j=37;
    message[3] = get_data[37].command_HEX;//is this a write unlock?
    ModRTU_CRC(message, result);      //call (modbusdatastream[]) this inserts the CRC in last two bytes
    delay(650);//send new request every half second minimum is ~85ms as response time is ~80ms from request to send
    serial_flush_buffer();//clear out buffer
    Serial1.write(message, sizeof(message));//write the unlock for write
    delay(100);
    Serial.print("Data sent  ->");//print the bytes of the data sent
    print_hex_MODBUS();
    receive_Data();//see if there is data
    j=31;
    message[3] = 0x28;//write year 2022
    message[4] = 0x20;//write year 2022
    message[5] = 0x22;//write year 2022
    ModRTU_CRC(message, result);      //call (modbusdatastream[]) this inserts the CRC in last two bytes
    Serial1.write(message, sizeof(message));
    Serial.print("Data sent  ->");//print the bytes of the data sent
    print_hex_MODBUS();
    delay(100);
    receive_Data();//see if there is data
    j=32;
    message[3] = 0x29;//write month
    message[4] = 0x00;//write month
    message[5] = 0x01;//write month
    ModRTU_CRC(message, result);      //call (modbusdatastream[]) this inserts the CRC in last two bytes
    Serial1.write(message, sizeof(message));
    Serial.print("Data sent  ->");//print the bytes of the data sent
    print_hex_MODBUS();
    delay(100);
    receive_Data();//see if there is data
    j=33;
    message[3] = 0x2A;//write day of month
    message[4] = 0x00;//write day of month
    message[5] = 0x01;//write day of month
    ModRTU_CRC(message, result);      //call (modbusdatastream[]) this inserts the CRC in last two bytes
    Serial1.write(message, sizeof(message));
    Serial.print("Data sent  ->");//print the bytes of the data sent
    print_hex_MODBUS();
    delay(100);
    receive_Data();//see if there is data
    j=34;
    message[3] = 0x2B;//write hour
    message[4] = 0x00;//write hour
    message[5] = 0x16;//write hour
    ModRTU_CRC(message, result);      //call (modbusdatastream[]) this inserts the CRC in last two bytes
    Serial1.write(message, sizeof(message));
    Serial.print("Data sent  ->");//print the bytes of the data sent
    print_hex_MODBUS();
    delay(100);
    receive_Data();//see if there is data
    j=35;
    message[3] = 0x2C;//write minutes
    message[4] = 0x00;//write minutes
    message[5] = 0x1A;//write minutes
    ModRTU_CRC(message, result);      //call (modbusdatastream[]) this inserts the CRC in last two bytes
    Serial1.write(message, sizeof(message));
    Serial.print("Data sent  ->");//print the bytes of the data sent
    print_hex_MODBUS();
    delay(100);
    receive_Data();//see if there is data
    j=37;
    message[3] = 0x00;//write lock for write
    message[4] = 0x00;//write lock for write
    message[5] = 0x00;//write lock for write
    ModRTU_CRC(message, result);      //call (modbusdatastream[]) this inserts the CRC in last two bytes
    Serial1.write(message, sizeof(message));
    Serial.print("Data sent  ->");//print the bytes of the data sent
    print_hex_MODBUS();
    delay(100);
    receive_Data();//see if there is data
    message[5] = 0x01;//set this back to 1 word reply
  */
  for (j = 0; j <= 36; j++)
    //So run through the data that the LianKe EElectric program puts out
  {
    if (j == 0)  message[3] =  get_data[0].command_HEX;
    if (j == 1)  message[3] =  get_data[1].command_HEX;
    if (j == 2)  message[3] =  get_data[2].command_HEX;
    if (j == 3)  message[3] =  get_data[3].command_HEX;
    if (j == 4)  message[3] =  get_data[4].command_HEX;
    if (j == 5)  message[3] =  get_data[5].command_HEX;
    if (j == 6)  message[3] =  get_data[6].command_HEX;
    if (j == 7)  message[3] =  get_data[7].command_HEX;
    if (j == 8)  message[3] =  get_data[8].command_HEX;
    if (j == 9)  message[3] =  get_data[9].command_HEX;
    if (j == 10) message[3] = get_data[10].command_HEX;
    if (j == 11) message[3] = get_data[11].command_HEX;
    if (j == 12) message[3] = get_data[12].command_HEX;
    if (j == 13) message[3] = get_data[13].command_HEX;
    if (j == 14) message[3] = get_data[14].command_HEX;
    if (j == 15) message[3] = get_data[15].command_HEX;
    if (j == 16) message[3] = get_data[16].command_HEX;
    if (j == 17) message[3] = get_data[17].command_HEX;
    if (j == 18) message[3] = get_data[18].command_HEX;
    if (j == 19) message[3] = get_data[19].command_HEX;
    if (j == 20) message[3] = get_data[20].command_HEX;
    if (j == 21) message[3] = get_data[21].command_HEX;
    if (j == 22) message[3] = get_data[22].command_HEX;
    if (j == 23) message[3] = get_data[23].command_HEX;
    if (j == 24) message[3] = get_data[24].command_HEX;
    if (j == 25) message[3] = get_data[25].command_HEX;
    if (j == 26) message[3] = get_data[26].command_HEX;
    if (j == 27) message[3] = get_data[27].command_HEX;
    if (j == 28) message[3] = get_data[28].command_HEX;
    if (j == 29) message[3] = get_data[29].command_HEX;
    if (j == 30) message[3] = get_data[30].command_HEX;
    if (j == 31) message[3] = get_data[31].command_HEX;
    if (j == 32) message[3] = get_data[32].command_HEX;
    if (j == 33) message[3] = get_data[33].command_HEX;
    if (j == 34) message[3] = get_data[34].command_HEX;
    if (j == 35) message[3] = get_data[35].command_HEX;
    if (j == 36) message[3] = get_data[36].command_HEX;

    size = 8;

    ModRTU_CRC(message, size);  //this inserts the CRC in last two bytes
    delay(85);//send new request every 85ms as response time is ~80ms from request to send
    serial_flush_buffer();//clear out buffer
    //ModRTU_CRC_check(message,result); //returns byte array
    Serial1.write(message, sizeof(message));
    print_hex_MODBUS();
    /*
        Serial.print("Data sent  ->");//print the bytes of the data sent
        for (uint8_t i = 0; i < sizeof(message); i++)//
        {
          if (message[i] < 16) Serial.print("0");
          Serial.print(message[i], HEX);
          Serial.print(" ");//space between hex bytes
        }
        Serial.print("  ");
    */
    receive_Data();//see if there is data
    //void showNewData();
    //delay(650);//send new request every half second minimum is ~85ms as response time is ~80ms from request to send
  }
  delay(5000);//delay 5 seconds between sending the requests
  Serial.println("");
}

void print_hex_MODBUS()
{
  Serial.print(" Data sent  ->0x");
  for (uint8_t i = 0; i < sizeof(message); i++)//
  {
    if (message[i] < 16) Serial.print("0");
    Serial.print(message[i], HEX);
    if (i != size - 1) //if there's another byte, we want the "0x" prefix
    {
      Serial.print(", 0x");
    }
  }//i for loop
  Serial.print(" -->");
}

void serial_flush_buffer()
{
  while (Serial.read() >= 0)
    ; // do nothing
}

void receive_Data()
{ //need to time out after some 8 bit no response after bytes start coming in
  data_found = false;
  uint32_t zero_timer = millis();//want to know how long from send message to receive
  boolean no_reply_timeout = false;
  uint8_t rc;
  uint32_t last_rx;
  //Serial.println("in read data routine");
  while (no_reply_timeout == false && data_found == false)
  {
    if (Serial1.available())
    {
      Serial.print("got data ");
      time_to_reply = millis() - zero_timer;
      data_found = true;
      last_rx = millis();//read the timer to see when the meter stops sending
    }
    else
    {
      uint32_t time_since_message = millis() - zero_timer;
      if  (time_since_message > 1450)
      {
        no_reply_timeout = true;
      }
    }
  }

  //Serial.print("no reply timeout=");
  //Serial.print(no_reply_timeout, DEC);
  //Serial.print("  data found=");
  //Serial.println(data_found, DEC);


  if (data_found == true)
  { //read in response
    Serial.print("RX t=");
    Serial.print(time_to_reply);
    Serial.print(" data=");
    uint8_t k = 0;//array index for read characters
    while (millis() - last_rx < 4)  //the bytes are close together, bit time of 9600 baud is .104 millsecond
    {
      if (Serial1.available())
      {
        rc = Serial1.read();
        //Serial.println(rc, HEX);
        receivedChars[k] = rc;
        k++;
        last_rx = millis();//read the timer to see when the meter stops sending
      }
    }
    for (uint8_t i = 0; i < k; i++)//
    {
      if (receivedChars[i] < 16) Serial.print("0");
      Serial.print(receivedChars[i], HEX);
      if (i != k - 1) //if there's another byte, we want the "0x" prefix
      {
        Serial.print(", 0x");
      }
    }
    Serial.print(" data=");
    int A = (receivedChars[3] << 8 | receivedChars[4]);

    Serial.print(get_data[j].command_type);
    Serial.print("-->");
    float Data = (float) A / pow(10, get_data[j].div_by_10);
    Serial.println(Data);
    for (uint8_t i = 0; i < sizeof(receivedChars); i++)//
    {
      receivedChars[i] = 0x00;
    }
  }
  else {
    Serial.println("No data");
  }
}




void showNewData()
{
  if (newData == true) {
    Serial.print("data ");
    for (uint8_t i = 0; i < sizeof(receivedChars); i++)//Is there a CR-LF sent, I hope not.
    {
      if (receivedChars[i] < 16) Serial.print("0");
      Serial.print(receivedChars[i], HEX);
    }
    Serial.println("");
  }
}

//Function for CRC insertion
byte ModRTU_CRC(uint8_t message[], uint8_t size) //, uint8_t result[]) //returns byte array
{
  uint16_t crc = 0xFFFF;//initialize
  for (int pos = 0; pos < size - 2; pos++) //size is data array length so pos ranges 0 to size-2 as array index
  {
    //result[pos] = message[pos];
    crc ^= (uint16_t)message[pos];       // XOR byte into least sig. byte of crc
    for (int i = 8; i != 0; i--)
    { // Loop over each bit
      if ((crc & 0x0001) != 0)        // If the LSB is set
      {
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;                // this is reflection of 0x8005
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  message[size - 2] = lowByte(crc);
  message[size - 1] = highByte(crc);
}

//Function for CRC
byte ModRTU_CRC_check(byte message[], byte result[]) //returns byte array
{
  uint16_t crc = 0xFFFF;//initialize
  for (int pos = 0; pos < 8; pos++)
  { //8 is data array length so pos ranges 0 to 7 as array index
    result[pos] = message[pos];
    crc ^= (uint16_t)message[pos];       // XOR byte into least sig. byte of crc
    for (int i = 8; i != 0; i--)
    { // Loop over each bit
      if ((crc & 0x0001) != 0)
      { // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;                // this is reflection of 0x8005
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  Serial.print("CRC-");
  Serial.print(lowByte(crc), HEX);
  Serial.println(highByte(crc), HEX);

}
