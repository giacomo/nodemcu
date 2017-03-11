#include <ESP8266WiFi.h>

const char* ssid = "YOUR_NETWORK_NAME";
const char* password = "YOUR_NETWORK_PASSWORD";

int Speaker = D7; // GPIO13
WiFiServer server(80);

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int c = 262;
int d = 294;
int e = 330;
int f = 349;
int g = 392;
int a = 440;
int b = 494;
int bf = 466;
int C = 523;

int frequency[56] = {
  c, d, f, d, a, a, g, c, d, f, d, g, g, f, e, d, c, d, f, d, f, g, e, d, c, c, g, f,
  c, d, f, d, a, a, g, c, d, f, d, C, e, f, e, d, c, d, f, d, f, g, e, d, c, c, g, f
};
int duration[56] = {
  120, 120, 120, 120, 360, 360, 720, 120, 120, 120, 120, 360, 360, 480, 120, 240, 120, 120, 120, 120,
  480, 240, 360, 120, 480, 240, 480, 960,
  120, 120, 120, 120, 360, 360, 720, 120, 120, 120, 120, 360, 360, 480, 120, 240, 120, 120, 120, 120,
  480, 240, 360, 120, 480, 240, 480, 960
};

int melody[78] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[78] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

void superMario() {
  for (int i = 0; i < 78; i++) { //run loop for all values of i 0 to __
    if (melody[i] > 0) {
      tone(Speaker, melody[i]); //play tones in order from the frequency array
    }
    delay(1000 / tempo[i] * 1.3);   //play all above tones for the corresponding duration in the duration array
    noTone(Speaker);   //turn off speaker for 30ms before looping back to the next note
    delay(10);
  }
}

void rickRoll() {
  for (int i = 0; i < 56; i++) { //run loop for all values of i 0 to __
    tone(Speaker, frequency[i]); //play tones in order from the frequency array
    delay(duration[i]);   //play all above tones for the corresponding duration in the duration array
    noTone(Speaker);   //turn off speaker for 30ms before looping back to the next note
    delay(10);
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(Speaker, OUTPUT);
  digitalWrite(Speaker, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int value = LOW;
  if (request.indexOf("/SND=SM") != -1)  {
    superMario();
    value = HIGH;
  }
  if (request.indexOf("/SND=RR") != -1)  {
    rickRoll();
    value = HIGH;
  }
  // Set Speaker according to the request
  //digitalWrite(Speaker, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Jukebox");
  client.println("<br><br>");
  client.println("<a href=\"/SND=RR\"\"><button>Rick Roll</button></a><br />");
  client.println("<a href=\"/SND=SM\"\"><button>Super Mario</button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
