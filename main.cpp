#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WiFiClientSecure.h>

#define LEDC_CHANNEL_0     0
#define LEDC_CHANNEL_1     1
#define LEDC_CHANNEL_2     2
#define LEDC_CHANNEL_3     3
#define LEDC_CHANNEL_4     4

#define PIN_WARM_WHITE 32
#define PIN_COLD_WHITE  25
#define PIN_RED 26
#define PIN_BLUE 33
#define PIN_GREEN 27


const char* ssid = "YourWiFI";
const char* password = "password";

AsyncWebServer server(80);

int currentR = 0;
int currentG = 0;
int currentB = 0;
int currentBrightness = 255;

void changeColor(String colorString) {
  long number = strtol(&colorString[1], NULL, 16);
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;

  ledcWrite(LEDC_CHANNEL_0, r * currentBrightness / 255);
  ledcWrite(LEDC_CHANNEL_1, g * currentBrightness / 255);
  ledcWrite(LEDC_CHANNEL_2, b * currentBrightness / 255);

  // Set warm and cold white LEDs to zero
  ledcWrite(LEDC_CHANNEL_3, 0);
  ledcWrite(LEDC_CHANNEL_4, 0);

  currentR = r;
  currentG = g;
  currentB = b;
}

void setWhiteColor(String colorString, bool isWarm) {
  long number = strtol(&colorString[1], NULL, 16);
  int w = number >> 16;

  if (isWarm) {
    ledcWrite(LEDC_CHANNEL_3, w * currentBrightness / 255);
    ledcWrite(LEDC_CHANNEL_4, 0);  // Turn off cold white
  } else {
    ledcWrite(LEDC_CHANNEL_3, 0);  // Turn off warm white
    ledcWrite(LEDC_CHANNEL_4, w * currentBrightness / 255);
  }
}

void setup() {
 
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  // Pozostała część funkcji setup() bez zmian...

  

  ledcSetup(LEDC_CHANNEL_0, 5000, 8);
  ledcAttachPin(PIN_RED, LEDC_CHANNEL_0);
  ledcSetup(LEDC_CHANNEL_1, 5000, 8);
  ledcAttachPin(PIN_GREEN, LEDC_CHANNEL_1);
  ledcSetup(LEDC_CHANNEL_2, 5000, 8);
  ledcAttachPin(PIN_BLUE, LEDC_CHANNEL_2);
  ledcSetup(LEDC_CHANNEL_3, 5000, 8);
  ledcAttachPin(PIN_WARM_WHITE, LEDC_CHANNEL_3);
  ledcSetup(LEDC_CHANNEL_4, 5000, 8);
  ledcAttachPin(PIN_COLD_WHITE, LEDC_CHANNEL_4);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<html><body>";
    html += "<head><meta charset='UTF-8'></head>";
    html += "<style>";
html += "body {";
html += "  background-image: url('URL_DO_TWOJEGO_ZDJĘCIA');";
html += "  background-size: cover;";
html += "  background-repeat: no-repeat;";
html += "  background-position: center;";
html += "}";
html += "</style>";
    html += "<style>";
    html += "body {display: flex; flex-direction: column; align-items: center;}";
    html += "button {width: 180px; height: 80px; margin: 5px; font-size: 16px; border-radius: 5px; color: white;}";
    html += "input[type='range'] {width: 200px;}";
    html += "</style>";
    html += "<p>LED CONTROL PANEL:</p>";
    html += "<input type='color' name='color' value='#000000' id='colorPicker'>";
    html += "<button style='background: linear-gradient(45deg,red,white); color: black;' onclick=\"setColor('#FF0000')\">Czerwony</button>";
    html += "<button style='background: linear-gradient(45deg,green,white); color: black;' onclick=\"setColor('#008000')\">Zielony</button>";
    html += "<button style='background: linear-gradient(45deg,blue,white);color: black;' onclick=\"setColor('#0000FF')\">Niebieski</button>";
    html += "<button style='background: white; color: black;'onclick=\"setColor('#FFFFFF')\">Zimny biały</button>";
    html += "<button style='background: linear-gradient(45deg,yellow,white);color: black;' onclick=\"setColor('#F5DEB3')\">Ciepły biały</button>";
     html += "<style>";
html += ".slider {width: 300px; height: 25px;}";
html += "p {font-size: 1.5em; font-weight: bold; color: #007BFF;}";
html += "</style>";

html += "<p>Intensywność:</p>";
html += "<input type='range' min='0' max='255' value='255' class='slider' id='brightnessSlider'>";

html += "<button style='background: linear-gradient(45deg, gray, white); color: black; font-size: 1.2em; padding: 10px;' onclick=\"turnOff()\">Wyłącz</button>";
  
    
html += "<div style='display: flex; justify-content: space-around;'>";
html += "<div style='display: flex; flex-direction: column;'>";

html += "<button style='background: linear-gradient(45deg, blue, indigo); color: black;' onclick=\"ocean()\">Morska bryza</button>"; 
html += "<button style='background: linear-gradient(45deg, grey,white); color: black;' onclick=\"max()\">MAX</button>";
html += "</div>";

html += "<div style='display: flex; flex-direction: column;'>";
html += "<button style='background: linear-gradient(45deg, pink, violet,white); color: black;'  onclick=\"kwiat()\">Wiosenny kwiat</button>";
html += "<button style='background: linear-gradient(90deg, red, orange, yellow, green, blue, indigo, violet); color: black;' onclick=\"startDisco()\">Dyskoteka</button>";
html += "</div>";
html += "</div>";

html += "<div style='display: flex; justify-content: space-around;'>";
html += "<div style='display: flex; flex-direction: column;'>";
html += "<button style='background: linear-gradient(45deg, green,yellow,orange); color: black;' onclick=\"tropikalny()\">Las tropikalny</button>";
html += "<button style='background: linear-gradient(45deg, yellow,orange); color:black;' onclick=\"lampka()\">Lampka nocna</button>"; 
html += "</div>";

html += "<div style='display: flex; flex-direction: column;'>";
html += "<button style='background: linear-gradient(45deg, green, blue, violet); color: black;'onclick=\"zorza()\">Zorza</button>"; 
html += "<button style='background: linear-gradient(45deg, blue, yellow); color: black;'onclick=\"koncentracja()\">Koncentracja</button>"; 

html += "</div>";
html += "</div>";

html += "<script>";


 

   
html += "var discoInterval = null;";
html += "var lastSetColor = null;";
html += "function setColor(color) {";
html += "  stopDisco();";
html += "  lastSetColor = color;";
html += "  sendColor(color);";
html += "}";
html += "function stopDisco() {";
html += "  if (discoInterval) {";
html += "    clearInterval(discoInterval);";
html += "    discoInterval = null;";
html += "  }";
html += "}";
html += "function startDisco() {";
html += "  stopDisco();";
html += "  var colors = ['#0000FF', '#008000', '#FF0000', '#FFFF00', '#FFA500', '#FFFFFF', '#008000', '#FFC0CB'];";
html += "  var index = 0;";
html += "  discoInterval = setInterval(function() {";
html += "    sendColorWithoutStoppingDisco(colors[index]);";
html += "    index = (index + 1) % colors.length;";
html += "  }, 100);";
html += "}";
html += "function sendColor(color) {";
html += "  var xhr = new XMLHttpRequest();";
html += "  xhr.open('POST', '/color', true);";
html += "  xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');";
html += "  xhr.send('color=' + encodeURIComponent(color));";
html += "}";
html += "function sendColorWithoutStoppingDisco(color) {";
html += "  lastSetColor = color;";
html += "  sendColor(color);";
html += "}";
    html += "document.getElementById('brightnessSlider').addEventListener('input', function() {";
    html += "  updateBrightness();";
    html += "});";
    html += "document.getElementById('colorPicker').addEventListener('change', function() {";
    html += "  setColor(this.value);"; // ustaw kolor z palety kolorów
    html += "});";
html += "function updateBrightness() {";
html += "  var color = lastSetColor || '#000000';"; // użyj ostatnio wybranego koloru lub domyślnego czarnego
html += "  var brightness = document.getElementById('brightnessSlider').value;";
html += "  sendColor(color);"; // wyślij aktualny kolor
html += "  sendBrightness(brightness);"; // wyślij aktualną jasność
html += "}";

html += "function sendBrightness(brightness) {";
html += "  var xhr = new XMLHttpRequest();";
html += "  xhr.open('POST', '/brightness', true);";
html += "  xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');";
html += "  xhr.send('brightness=' + encodeURIComponent(brightness));";
html += "}";

html += "function max() {";
html += "  var red = Math.round(255 * 1);"; 
html += "  var green = Math.round(255 *1);"; 
html += "  var blue = Math.round(255 * 1);"; 
html += "  var warmWhite = Math.round(255 * 1);"; 
html += "  var color = '#' + red.toString(16) + green.toString(16) + blue.toString(16);";
html += "  setColor(color);";
html += "  sendWarmWhite(warmWhite);"; // ta funkcja musi zostać zaimplementowana
html += "}";

html += "function kwiat() {";
html += "  stopDisco();";
html += "  var red = 240;"; // 63% czerwony
html += "  var green = 51;"; // 23% zielony
html += "  var blue = 252;"; // 92% niebieski
html += "  var warmWhite = 51;"; // 40% ciepły biały

html += "  var color = '#' + red.toString(16) + green.toString(16) + blue.toString(16);";
html += "  setColor(color);";
html += "  sendWarmWhite(warmWhite);"; // ta funkcja musi zostać zaimplementowana
html += "}";


html += "function ocean() {";
html += "  stopDisco();";
html += "  var red =  5;"; // 63% czerwony
html += "  var green = 255;"; // 80% zielony
html += "  var blue =200;"; // 40% niebieski
html += "  var warmWhite = 0;"; // 40% ciepły biały


html += "  var color = '#' + red.toString(16) + green.toString(16) + blue.toString(16);";
html += "  setColor(color);";
html += "  sendWarmWhite(warmWhite);"; // ta funkcja musi zostać zaimplementowana
html += "}";

html += "function tropikalny() {";
html += "  stopDisco();";
html += "  var red = 160;"; // 63% czerwony
html += "  var green = 189;"; // 23% zielony
html += "  var blue = 16;"; // 92% niebieski

html += "  var color = '#' + red.toString(16) + green.toString(16) + blue.toString(16);";
html += "  setColor(color);";
html += "  sendWarmWhite(warmWhite);"; // ta funkcja musi zostać zaimplementowana
html += "}";

html += "function lampka() {";
html += "  stopDisco();";
html += "  var red = 240;"; // 63% czerwony
html += "  var green = 240;"; // 23% zielony
html += "  var blue = 0;"; // 92% niebieski
html += "  var warmWhite = 30;"; // 40% ciepły biały

html += "  var warmWhite = Math.round(255 * 0.0);"; // 40% ciepły biały

html += "  var color = '#' + red.toString(16) + green.toString(16) + blue.toString(16);";
html += "  setColor(color);";
html += "  sendWarmWhite(warmWhite);"; // ta funkcja musi zostać zaimplementowana
html += "}";


html += "function zorza() {";
html += "  stopDisco();";
html += "  var red = Math.round(255 * 0.63);"; // 63% czerwony
html += "  var green = Math.round(255 * 0.23);"; // 23% zielony
html += "  var blue = Math.round(255 * 0.92);"; // 92% niebieski
html += "  var warmWhite = Math.round(255 * 0.40);"; // 40% ciepły biały

html += "  var color = '#' + red.toString(16) + green.toString(16) + blue.toString(16);";
html += "  setColor(color);";
html += "  sendWarmWhite(warmWhite);"; // ta funkcja musi zostać zaimplementowana
html += "}";

html += "function koncentracja() {";
html += "  stopDisco();";
html += "  var red = Math.round(255 * 0.5);"; 
html += "  var green = Math.round(255 *0.05);"; 
html += "  var blue = Math.round(255 * 0.35);"; 
html += "  var warmWhite = Math.round(255 * 1);"; 
html += "  var color = '#' + red.toString(16) + green.toString(16) + blue.toString(16);";
html += "  setColor(color);";
html += "  sendWarmWhite(warmWhite);"; // ta funkcja musi zostać zaimplementowana
html += "}";

html += "function turnOff() {";
html += "  if (discoInterval) {";
html += "    clearInterval(discoInterval);";
html += "    discoInterval = null;";
html += "  }";
html += "  var xhr = new XMLHttpRequest();";
html += "  xhr.open('POST', '/color', true);";
html += "  xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');";
html += "  xhr.send('color=' + encodeURIComponent('#000000'));";
html += "}";

    html += "</script>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  server.on("/color", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("color", true)) {
      String color = request->getParam("color", true)->value();
      changeColor(color);
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "400: nieprawidłowe żądanie");
    }
  });

  server.on("/brightness", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("brightness", true)) {
      String brightnessString = request->getParam("brightness", true)->value();
      currentBrightness = brightnessString.toInt();
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "400: nieprawidłowe żądanie");
    }
  });

  server.begin();
}

void loop() {
  delay(50);
}
