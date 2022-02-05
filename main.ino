#include<SPI.h>
#include<Ethernet.h>
#include <LiquidCrystal.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xEF, 0xED};
EthernetServer server(80);
String readString;

const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);



void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();

  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);

        if (readString.length() < 100) {
          readString += c;
        }

        if (c == '\n') {
          Serial.print(readString);
          client.println("<HTTP/1.1 200 0K>");
          client.print("<Connection-Type: text/html>");
          client.println("<Connection: close>");
          client.println("");

          client.println("<!DOCTYPE html>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Webserver</title>");
          client.println("</head>");
          client.println("<body>");
          client.println("<label for=\"name\">Inserire testo:</label><input type=\"text\" id=\"name\"  name=\"name\" required >");
          client.println("<button onclick=\"scriviLcd()\">Scrivi su LCD</button>");
          client.println("<script>");
          client.println("function scriviLcd(){ var url = \"/?textLcd=\" + document.getElementById(\"name\").value;window.location.href = url;}");
          client.println("</script>");
          client.println("<body style=background-color:powderblue>");

          delay(1);
          client.stop();
         
          if (readString.indexOf("/?textLcd=") > 0) {
              lcd.setCursor(0, 1);
              String res = readString.substring(readString.indexOf("=")+1,readString.indexOf("HTTP/1.1"));
              Serial.print(res);
              lcd.print(res);
          }
          readString = "";
        }

      }

    }


  }
}
