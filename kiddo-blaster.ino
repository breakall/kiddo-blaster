/* Kiddo Blaster
 * An infrared remote controlled MP3 player.
 *
 * Infrared receiver: CHQ1838
 * MP3 player: YX5300
 *
 * Based on:
 * IRrecvDemo (IRremoteESP8266 v1.2) https://github.com/markszabo/IRremoteESP8266
 * ArduinoSerialMP3Player https://github.com/salvadorrueda/ArduinoSerialMP3Player
 * 
 * Assumes two folders on the SD card inserted into the YX5300 named "01" and "02".
 * MP3s in the folders should have a prefix in the name of "001", "002", and so on.
 */

// wifi off -- see setup()
#include <ESP8266WiFi.h>


// INFRARED 
#include <IRremoteESP8266.h>
int RECV_PIN = 2; // GPIO 2 is labeled D4 on the ESP12E board
IRrecv irrecv(RECV_PIN);
decode_results results;


// YX5300 MP3
#include <SoftwareSerial.h>
#define RX_PIN 13  // D7
#define TX_PIN 15  // D8
SoftwareSerial mp3(RX_PIN, TX_PIN);


// IR remote map - BUTTON_X_Y from top left
#define BUTTON_BRIGHTNESS_UP 0x8503705D //brightness up
#define BUTTON_BRIGHTNESS_DOWN 0xDEB0C861 //brightness down
#define BUTTON_OFF 0xD4DD0381 //OFF
#define BUTTON_ON 0xCE1972FD //ON

#define BUTTON_1_0 0xE85952E1 //red
#define BUTTON_1_1 0x78CDA4DD //green
#define BUTTON_1_2 0xA2672345 //blue
#define BUTTON_1_3 0x9BA392C1 //white

#define BUTTON_2_0 0xD3FD9A81 //
#define BUTTON_2_1 0x6471EC7D //
#define BUTTON_2_2 0x9D52009D //
#define BUTTON_FLASH 0xF7D02F // CE1972FD? FLASH

#define BUTTON_3_0 0x84044BBD //
#define BUTTON_3_1 0x14789DB9 //
//#define BUTTON_3_2 
#define BUTTON_STROBE 0xF7F00F //STROBE
//
//#define BUTTON_4_0 
//#define BUTTON_4_1 
//#define BUTTON_4_2 
#define BUTTON_FADE 0xF7C837// FADE
//
//#define BUTTON_5_0 
//#define BUTTON_5_1 
//#define BUTTON_5_2 
//#define BUTTON_SMOOTH 0xF7E817 //SMOOTH


static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
static uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY
String mp3Answer;                // Answer from the MP3.
String sanswer(void);
String sbyte2hex(uint8_t b);


/************ Command byte **************************/
#define CMD_NEXT_SONG     0X01  // Play next song.
#define CMD_PREV_SONG     0X02  // Play previous song.
#define CMD_PLAY_W_INDEX  0X03
#define CMD_VOLUME_UP     0X04
#define CMD_VOLUME_DOWN   0X05
#define CMD_SET_VOLUME    0X06

#define CMD_SNG_CYCL_PLAY 0X08  // Single Cycle Play.
#define CMD_SEL_DEV       0X09
#define CMD_SLEEP_MODE    0X0A
#define CMD_WAKE_UP       0X0B
#define CMD_RESET         0X0C
#define CMD_PLAY          0X0D
#define CMD_PAUSE         0X0E
#define CMD_PLAY_FOLDER_FILE 0X0F

#define CMD_STOP_PLAY     0X16  // Stop playing continuously. 
#define CMD_FOLDER_CYCLE  0X17
#define CMD_SHUFFLE_PLAY  0x18 //
#define CMD_SET_SNGL_CYCL 0X19 // Set single cycle.

#define CMD_SET_DAC 0X1A
#define DAC_ON  0X00
#define DAC_OFF 0X01

#define CMD_PLAY_W_VOL    0X22
#define CMD_PLAYING_N     0x4C
#define CMD_QUERY_STATUS      0x42
#define CMD_QUERY_VOLUME      0x43
#define CMD_QUERY_FLDR_TRACKS 0x4e
#define CMD_QUERY_TOT_TRACKS  0x48
#define CMD_QUERY_FLDR_COUNT  0x4f

/************ Opitons **************************/
#define DEV_TF            0X02


//################################################################

void setup()
{
  // turn wifi off to save power
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
  
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  // setup the mp3 player
  mp3.begin(9600);

  delay(500);

  sendCommand(CMD_SEL_DEV, 0, DEV_TF);
  delay(500);
}

//################################################################

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);

    switch(results.value)
    {
      case BUTTON_BRIGHTNESS_UP:
        sendCommand(CMD_PREV_SONG);                 //PREV
        break;
      case BUTTON_BRIGHTNESS_DOWN:
        sendCommand(CMD_NEXT_SONG);                 //NEXT
        break;
      case BUTTON_OFF:
        sendCommand(CMD_RESET);                     //reset
        break;
      case BUTTON_ON:
        sendCommand(CMD_FOLDER_CYCLE, 1, 0);        //play sleepy time folder with all 3 albums
        break;
      case BUTTON_1_3:
        sendCommand(CMD_FOLDER_CYCLE, 2, 0);        //play storybook folder folder
        break;
      case BUTTON_FLASH:
        sendCommand(CMD_FOLDER_CYCLE, 3, 0);        //play Come to the Cradle album
        break; 
      case BUTTON_STROBE:
        sendCommand(CMD_FOLDER_CYCLE, 4, 0);        //play Sleep Sound in Jesus album
        break;
      case BUTTON_FADE:
        sendCommand(CMD_FOLDER_CYCLE, 5, 0);        //play I Love You More album
        break;
      
      //play individual story books
      
      case BUTTON_1_0:
        sendCommand(CMD_PLAY_FOLDER_FILE, 2, 1);    
        break;
      case BUTTON_1_1:
        sendCommand(CMD_PLAY_FOLDER_FILE, 2, 2);    
        break;
      case BUTTON_1_2:
        sendCommand(CMD_PLAY_FOLDER_FILE, 2, 3);    
        break;
      case BUTTON_2_0:
        sendCommand(CMD_PLAY_FOLDER_FILE, 2, 4);    
        break;
      case BUTTON_2_1:
        sendCommand(CMD_PLAY_FOLDER_FILE, 2, 5);    
        break;
      case BUTTON_2_2:
        sendCommand(CMD_PLAY_FOLDER_FILE, 2, 6);    
        break;
      case BUTTON_3_0:
        sendCommand(CMD_PLAY_FOLDER_FILE, 2, 7);    
        break;
      case BUTTON_3_1:
        sendCommand(CMD_PLAY_FOLDER_FILE, 2, 8);    
        break;
      
    } 
    
    irrecv.resume(); // Receive the next value
  }
  delay(100);

  //for serial debugging
  char c = ' ';
  // check for command on the debug serial
  if ( Serial.available() )
  {
    c = Serial.read();
    Serial.println("Received: " + c);
    sendMP3Command(c); 
  }
  
  // check for any feedback from the mp3 player
  if (mp3.available())
  {
    Serial.println(decodeMP3Answer());
  }
  delay(100);
  
}


/********************************************************************************/
/*Function sendMP3Command: seek for a 'c' command and send it to MP3  */
/*Parameter: c. Code for the MP3 Command, 'h' for help.                                                                                                         */
/*Return:  void                                                                */

void sendMP3Command(char c) {
  
  switch (c) {
    case '?':
    case 'h':
      Serial.println("HELP  ");
      Serial.println(" p = Play");
      Serial.println(" P = Pause");
      Serial.println(" > = Next");
      Serial.println(" < = Previous");
      Serial.println(" s = Stop Play"); 
      Serial.println(" + = Volume UP");
      Serial.println(" - = Volume DOWN");
      Serial.println(" c = Query current file");
      Serial.println(" q = Query status");
      Serial.println(" v = Query volume");
      Serial.println(" x = Query folder count");
      Serial.println(" t = Query total file count");
      Serial.println(" f = Play folder 1.");
      Serial.println(" S = Sleep");
      Serial.println(" W = Wake up");
      Serial.println(" r = Reset");
      break;


    case 'p':
      Serial.println("Play ");
      sendCommand(CMD_PLAY);
      break;

    case 'P':
      Serial.println("Pause");
      sendCommand(CMD_PAUSE);
      break;

    case '>':
      Serial.println("Next");
      sendCommand(CMD_NEXT_SONG);
      sendCommand(CMD_PLAYING_N); // ask for the number of file is playing
      break;

    case '<':
      Serial.println("Previous");
      sendCommand(CMD_PREV_SONG);
      sendCommand(CMD_PLAYING_N); // ask for the number of file is playing
      break;

    case 's':
      Serial.println("Stop Play");
      sendCommand(CMD_STOP_PLAY);
      break;


    case '+':
      Serial.println("Volume Up");
      sendCommand(CMD_VOLUME_UP);
      break;

    case '-':
      Serial.println("Volume Down");
      sendCommand(CMD_VOLUME_DOWN);
      break;

    case 'c':
      Serial.println("Query current file");
      sendCommand(CMD_PLAYING_N);
      break;

    case 'q':
      Serial.println("Query status");
      sendCommand(CMD_QUERY_STATUS);
      break;

    case 'v':
      Serial.println("Query volume");
      sendCommand(CMD_QUERY_VOLUME);
      break;

    case 'x':
      Serial.println("Query folder count");
      sendCommand(CMD_QUERY_FLDR_COUNT);
      break;

    case 't':
      Serial.println("Query total file count");
      sendCommand(CMD_QUERY_TOT_TRACKS);
      break;

    case 'S':
      Serial.println("Sleep");
      sendCommand(CMD_SLEEP_MODE);
      break;

    case 'W':
      Serial.println("Wake up");
      sendCommand(CMD_WAKE_UP);
      break;

    case 'r':
      Serial.println("Reset");
      sendCommand(CMD_RESET);
      break;

  }
}



/********************************************************************************/
/*Function decodeMP3Answer: Decode MP3 answer.                                  */
/*Parameter:-void                                                               */
/*Return: The                                                  */

String decodeMP3Answer() {
  String decodedMP3Answer = "";

  decodedMP3Answer += sanswer();

  switch (ansbuf[3]) {
    case 0x3A:
      decodedMP3Answer += " -> Memory card inserted.";
      break;
    case 0x3D:
      decodedMP3Answer += " -> Completed play num " + String(ansbuf[6], DEC);
      break;
    case 0x40:
      decodedMP3Answer += " -> Error";
      break;
    case 0x41:
      decodedMP3Answer += " -> Data received correctly. ";
      break;
    case 0x42:
      decodedMP3Answer += " -> Status playing: " + String(ansbuf[6], DEC);
      break;
    case 0x48:
      decodedMP3Answer += " -> File count: " + String(ansbuf[6], DEC);
      break;
    case 0x4C:
      decodedMP3Answer += " -> Playing: " + String(ansbuf[6], DEC);
      break;
    case 0x4E:
      decodedMP3Answer += " -> Folder file count: " + String(ansbuf[6], DEC);
      break;
    case 0x4F:
      decodedMP3Answer += " -> Folder count: " + String(ansbuf[6], DEC);
      break;
    default: 
      decodedMP3Answer += " -> Unknown response";
  }

  return decodedMP3Answer;
}


/********************************************************************************/
/*Function: Send command to the MP3                                             */
/*Parameter: byte command                                                       */
/*Parameter: byte dat1 parameter for the command                                */
/*Parameter: byte dat2 parameter for the command                                */

void sendCommand(byte command){
  sendCommand(command, 0, 0);
}

void sendCommand(byte command, byte dat1, byte dat2){
  delay(20);
  Send_buf[0] = 0x7E;    //
  Send_buf[1] = 0xFF;    //
  Send_buf[2] = 0x06;    // Len
  Send_buf[3] = command; //
  Send_buf[4] = 0x01;    // 0x00 NO, 0x01 feedback
  Send_buf[5] = dat1;    // datah
  Send_buf[6] = dat2;    // datal
  Send_buf[7] = 0xEF;    //
  Serial.print("Sending: ");
  for (uint8_t i = 0; i < 8; i++)
  {
    mp3.write(Send_buf[i]) ;
    Serial.print(sbyte2hex(Send_buf[i]));
  }
  Serial.println();
}



/********************************************************************************/
/*Function: sbyte2hex. Returns a byte data in HEX format.                       */
/*Parameter:- uint8_t b. Byte to convert to HEX.                                */
/*Return: String                                                                */


String sbyte2hex(uint8_t b)
{
  String shex;

  shex = "0X";

  if (b < 16) shex += "0";
  shex += String(b, HEX);
  shex += " ";
  return shex;
}


/********************************************************************************/
/*Function: shex2int. Returns a int from an HEX string.                         */
/*Parameter: s. char *s to convert to HEX.                                      */
/*Parameter: n. char *s' length.                                                */
/*Return: int                                                                   */

int shex2int(char *s, int n){
  int r = 0;
  for (int i=0; i<n; i++){
     if(s[i]>='0' && s[i]<='9'){
      r *= 16; 
      r +=s[i]-'0';
     }else if(s[i]>='A' && s[i]<='F'){
      r *= 16;
      r += (s[i] - 'A') + 10;
     }
  }
  return r;
}


/********************************************************************************/
/*Function: sanswer. Returns a String answer from mp3 UART module.              */
/*Parameter:- uint8_t b. void.                                                  */
/*Return: String. If the answer is well formated answer.                        */

String sanswer(void)
{
  uint8_t i = 0;
  String mp3answer = "";

  // Get only 10 Bytes
  while (mp3.available() && (i < 10))
  {
    uint8_t b = mp3.read();
    ansbuf[i] = b;
    i++;

    mp3answer += sbyte2hex(b);
  }

  // if the answer format is correct.
  if ((ansbuf[0] == 0x7E) && (ansbuf[9] == 0xEF))
  {
    return mp3answer;
  }

  return "???: " + mp3answer;
}
