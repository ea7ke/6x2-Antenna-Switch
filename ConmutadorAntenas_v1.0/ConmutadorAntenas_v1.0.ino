#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SSD1306.h"

// OLED
SSD1306 display(0x3c, 4, 15);

// WiFi
const char* ssid = "7KE";
const char* password = "322Koala#";

// Pines de antenas
const int radio1Pins[6] = {2, 5, 12, 13, 14, 27};
const int radio2Pins[6] = {16, 17, 18, 19, 21, 22};

// Estado de selección
int selectedRadio1 = -1;
int selectedRadio2 = -1;

AsyncWebServer server(80);

// Función para actualizar OLED
void updateOLED() {
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "IP: " + WiFi.localIP().toString());
  display.drawString(0, 12, "Radio1: " + String(selectedRadio1 >= 0 ? selectedRadio1 + 1 : 0));
  display.drawString(0, 24, "Radio2: " + String(selectedRadio2 >= 0 ? selectedRadio2 + 1 : 0));
  display.display();
}

// Función para generar HTML dinámico
String generatePage(int radio) {
  String html = "<html><head><meta charset='UTF-8'><title>Radio " + String(radio) + "</title>";
  html += "<script>";
  html += "const RADIO=" + String(radio) + ";";
  html += "function updateButtons(){";
  html += "fetch('/estado').then(r => r.json()).then(data => {";
  html += "let r1 = data.radio1; let r2 = data.radio2;";
  html += "for(let i=0;i<6;i++){";
  html += "let btn = document.getElementById('btn'+i);";
  html += "if(r1==i && RADIO==1) btn.style.backgroundColor='blue';";
  html += "else if(r2==i && RADIO==2) btn.style.backgroundColor='blue';";
  html += "else if((r1==i && RADIO==2) || (r2==i && RADIO==1)) btn.style.backgroundColor='red';";
  html += "else btn.style.backgroundColor='green';";
  html += "}});}";
  html += "setInterval(updateButtons, 2000); window.onload = updateButtons;";
  html += "</script></head><body>";
  html += "<h2>Radio " + String(radio) + "</h2>";
  for (int i = 0; i < 6; i++) {
    html += "<form action='/select" + String(radio) + "' method='GET'>";
    html += "<input type='hidden' name='antena' value='" + String(i) + "'>";
    html += "<input type='submit' id='btn" + String(i) + "' value='Antena " + String(i + 1) + "' style='width:100px;height:40px;margin:5px;'>";
    html += "</form>";
  }
  html += "</body></html>";
  return html;
}

// Función para seleccionar antena
void selectAntena(int radio, int antena) {
  if (radio == 1) {
    if (selectedRadio2 != antena) {
      selectedRadio1 = antena;
    }
  } else {
    if (selectedRadio1 != antena) {
      selectedRadio2 = antena;
    }
  }

  for (int i = 0; i < 6; i++) {
    digitalWrite(radio1Pins[i], i == selectedRadio1 ? HIGH : LOW);
    digitalWrite(radio2Pins[i], i == selectedRadio2 ? HIGH : LOW);
  }

  updateOLED();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(4, 15);
  display.init();
  display.flipScreenVertically();

  for (int i = 0; i < 6; i++) {
    pinMode(radio1Pins[i], OUTPUT);
    pinMode(radio2Pins[i], OUTPUT);
    digitalWrite(radio1Pins[i], LOW);
    digitalWrite(radio2Pins[i], LOW);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  updateOLED();

  server.on("/radio1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", generatePage(1));
  });

  server.on("/radio2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", generatePage(2));
  });

  server.on("/select1", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("antena")) {
      int antena = request->getParam("antena")->value().toInt();
      selectAntena(1, antena);
    }
    request->redirect("/radio1");
  });

  server.on("/select2", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("antena")) {
      int antena = request->getParam("antena")->value().toInt();
      selectAntena(2, antena);
    }
    request->redirect("/radio2");
  });

  server.on("/estado", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"radio1\":" + String(selectedRadio1) + ",";
    json += "\"radio2\":" + String(selectedRadio2);
    json += "}";
    request->send(200, "application/json", json);
  });

  server.begin();
}

void loop() {}
