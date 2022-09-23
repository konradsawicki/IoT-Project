#include <WiFi.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(22,23,5,18,19,21);

int phPin = 34;
int buttonPin = 33;
int greenPin = 27;
int redPin = 26;
int yellowPin = 25;
int buttonCount = 0;
int buttonState = 0;
int lastButtonState = 0;
int nrPomiaru = 0;
const int httpPort = 80;

float ph = 0;

const char* ssid = "your_ssid";
const char* password = "your_password"; 
const char* host = "your_host_adress";

void setup() {   
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Connecting");
  lcd.setCursor(0,1);
  lcd.print("to WiFi...");
  pinMode(buttonPin, INPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  digitalWrite(greenPin, LOW);
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("");
  Serial.println("Connected to WiFi with IP adress: ");
  Serial.print(WiFi.localIP());
  lcd.clear();
  lcd.print("Connected");
  lcd.setCursor(0,1);
  lcd.print("with WiFi ");
  delay(1000);
}

void loop() {
  WiFiClient client;
  ph = ((3267.67 - analogRead(phPin))/212.67);  
  lcd.clear();
  lcd.print("pH value");
  lcd.setCursor(0,1);
  lcd.print("equals ");
  lcd.print(ph);
  delay(400);
  if (ph >= 10.00 && ph <= 13.00) {
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
  }
  else {
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
  }
  buttonState = digitalRead(33);
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      buttonCount++;
      if (buttonCount % 2 == 1) {
        nrPomiaru++;
        digitalWrite(yellowPin, HIGH); 
      }
      else {
        digitalWrite(yellowPin, LOW);
      }
    }
  }
  lastButtonState = buttonState;
  if (buttonCount % 2 == 1) {
    Serial.print("Connecting to Server...");
    Serial.println(host);
    // Use WiFiClient class to create TCP connections
    if (!client.connect(host, httpPort)) {
        Serial.println("Connection failed!");
        return;
    }
    // This will send the request to the server
    client.print(String("GET http://localhost/iot_project/connect.php?") + 
                          ("&ph=") + ph +
                          ("&nrPomiaru=") + nrPomiaru +
                          " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 1000) {
            Serial.println(">>> No answer from the Client!");
            client.stop();
            return;
        }
    }
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line); 
    }
    Serial.println();
    Serial.println("Connection ended");
    delay(100);
  }
}
