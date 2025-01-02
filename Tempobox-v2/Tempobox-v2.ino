// SOURCES
// https://forum.arduino.cc/t/api-rte-ecowatt/1017281/37
// https://randomnerdtutorials.com/esp32-http-get-post-arduino/
// https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/
// https://data.rte-france.com/catalog/-/api/doc/user-guide/Tempo+Like+Supply+Contract/1.1#TLCRG02
// MAKERTRONIC - https://www.youtube.com/watch?v=mw6xLQAqiKs&lc=UgwY5Fh_WTROy7Fow6h4AaABAg.9z1ZN67ggNI9z23zYdJtDm


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
//#include <HTTPClient.h>
//#include <WiFi.h>

#include <ArduinoJson.h>
#include <WiFiUdp.h>

// *****************************************
//     ! ATTENTION ELEMENTS SECRETS !
// *****************************************

const char* ssid =          "Maiz";
const char* password =      "rastafouille";

 
// leds
const int ledJourBleu = 5;
const int ledJourBlanc = 4;
const int ledJourRouge = 0;
const int ledDemainBleu = 2;
const int ledDemainBlanc = 12;
const int ledDemainRouge = 13;


// le certificat racine (format PEM) de https://particulier.edf.fr/
const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIBzzCCAXWgAwIBAgIEYfJ84jAKBggqhkjOPQQDAjBNMQswCQYDVQQGEwJMVDEV\n" \
"MBMGA1UECgwMTm9yZFZQTiBTLkEuMScwJQYDVQQDDB5Ob3JkVlBOIFMuQS4gLVRo\n" \
"cmVhdFByb3RlY3Rpb24wHhcNMjQxMDA3MjE0ODE1WhcNMjUxMDA3MjE1MzE1WjAf\n" \
"MR0wGwYDVQQDDBRhcGktY291bGV1ci10ZW1wby5mcjBZMBMGByqGSM49AgEGCCqG\n" \
"SM49AwEHA0IABDviEFdv5CttGgf+5Y7wP1NVq9J3CIM9uh3L8vuL7k241xr97NXW\n" \
"AJrghNCKLWmAUi6MclBcPHLmBsOBTavNS6SjcTBvMB0GA1UdDgQWBBR16FW9Elxn\n" \
"BUUbOfLcKNsTWO//AzATBgNVHSUEDDAKBggrBgEFBQcDATA5BgNVHREEMjAwghRh\n" \
"cGktY291bGV1ci10ZW1wby5mcoIYd3d3LmFwaS1jb3VsZXVyLXRlbXBvLmZyMAoG\n" \
"CCqGSM49BAMCA0gAMEUCIAWHUgaVgrFurAJ1D4oxvPH3j96i/1LrCvD9BINNMYq0\n" \
"AiEAi14x+q1rS79hWybRMQVPIx+DEapPPUAQ4WdM5AfX2VY=\n" \
"-----END CERTIFICATE-----\n";

void setup(){


    // leds en sortie
    pinMode(ledJourBleu, OUTPUT);
    pinMode(ledJourBlanc, OUTPUT);
    pinMode(ledJourRouge, OUTPUT);
    pinMode(ledDemainBleu, OUTPUT);
    pinMode(ledDemainBlanc, OUTPUT);
    pinMode(ledDemainRouge, OUTPUT);

    digitalWrite(ledJourBleu, LOW);digitalWrite(ledJourBlanc, LOW);digitalWrite(ledJourRouge, LOW);
    digitalWrite(ledDemainBleu, LOW);digitalWrite(ledDemainBlanc, LOW);digitalWrite(ledDemainRouge, LOW);

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledJourBleu, HIGH);digitalWrite(ledJourBlanc, LOW);digitalWrite(ledJourRouge, LOW);
    digitalWrite(ledDemainBleu, LOW);digitalWrite(ledDemainBlanc, LOW);digitalWrite(ledDemainRouge, LOW);
    delay(100);
    digitalWrite(ledJourBleu, LOW);digitalWrite(ledJourBlanc, HIGH);digitalWrite(ledJourRouge, LOW);
    digitalWrite(ledDemainBleu, LOW);digitalWrite(ledDemainBlanc, LOW);digitalWrite(ledDemainRouge, LOW);
    delay(100);
    digitalWrite(ledJourBleu, LOW);digitalWrite(ledJourBlanc, LOW);digitalWrite(ledJourRouge, HIGH);
    digitalWrite(ledDemainBleu, LOW);digitalWrite(ledDemainBlanc, LOW);digitalWrite(ledDemainRouge, LOW);
    delay(100);
    digitalWrite(ledJourBleu, LOW);digitalWrite(ledJourBlanc, LOW);digitalWrite(ledJourRouge, LOW);
    digitalWrite(ledDemainBleu, LOW);digitalWrite(ledDemainBlanc, LOW);digitalWrite(ledDemainRouge, HIGH);
    delay(100);
    digitalWrite(ledJourBleu, LOW);digitalWrite(ledJourBlanc, LOW);digitalWrite(ledJourRouge, LOW);
    digitalWrite(ledDemainBleu, LOW);digitalWrite(ledDemainBlanc, HIGH);digitalWrite(ledDemainRouge, HIGH);
    delay(100);
    digitalWrite(ledJourBleu, LOW);digitalWrite(ledJourBlanc, LOW);digitalWrite(ledJourRouge, LOW);
    digitalWrite(ledDemainBleu, HIGH);digitalWrite(ledDemainBlanc, LOW);digitalWrite(ledDemainRouge, LOW);
    delay(100);
    Serial.write('.');
  }
    digitalWrite(ledJourBleu, LOW);digitalWrite(ledJourBlanc, LOW);digitalWrite(ledJourRouge, LOW);
    digitalWrite(ledDemainBleu, LOW);digitalWrite(ledDemainBlanc, LOW);digitalWrite(ledDemainRouge, LOW);

  Serial.print("\nConnected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());


}


void loop() {
 
  // On coupe les leds avant de les allumer
  digitalWrite(ledJourBleu,     LOW);
  digitalWrite(ledJourBlanc,    LOW);
  digitalWrite(ledJourRouge,    LOW);
  digitalWrite(ledDemainBleu,   LOW);
  digitalWrite(ledDemainBlanc,  LOW);
  digitalWrite(ledDemainRouge,  LOW);

  // URL JSON
  String urlToDay = "https://www.api-couleur-tempo.fr/api/jourTempo/today";
  //Serial.println(urlToDay); // debug url
  String urlTomorrow = "https://www.api-couleur-tempo.fr/api/jourTempo/tomorrow";
  //Serial.println(urlTomorrow); // debug url

  // reponse : 
  //{
  //  "dateJour": "2024-09-03",
  //  "codeJour": 1,
  //  "periode": "2024-2025"
  //}

  // Effectuer la requête HTTP
  HTTPClient http;
  WiFiClientSecure client;
  X509List cert(root_ca);
  client.setTrustAnchors(&cert);
  http.begin(client,urlToDay);

  int httpCode = http.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();

        // Parse JSON avec ArduinoJSON
        const size_t capacity = JSON_OBJECT_SIZE(3) + 100;
        DynamicJsonDocument doc(capacity);
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
          return;
        }

        // Extraire les valeurs
        int codeJour = doc["codeJour"];
        Serial.print("CouleurJour: ");Serial.println(codeJour);

        // Afficher la couleur en fonction du code
        //Code couleur du tarif Tempo applicable: 
        // - 0: tarif inconnu (pas encore communiqué par RTE) 
        // - 1: tarif bleu 
        // - 2: tarif blanc 
        // - 3: tarif rouge
        String couleur;
        switch(codeJour) {
          case 0:
            couleur = "INCONNU";
            break;
          case 1:
            couleur = "BLEU";
            digitalWrite(ledJourBleu, HIGH);
            digitalWrite(ledJourBlanc, LOW);
            digitalWrite(ledJourRouge, LOW);
            break;
          case 2:
            couleur = "BLANC";
            digitalWrite(ledJourBleu, LOW);
            digitalWrite(ledJourBlanc, HIGH);
            digitalWrite(ledJourRouge, LOW);
            break;
          case 3:
            couleur = "ROUGE";
            digitalWrite(ledJourBleu, LOW);
            digitalWrite(ledJourBlanc, LOW);
            digitalWrite(ledJourRouge, HIGH);
            break;
          default:
            couleur = "INCONNU";
        }

        // Afficher le résultat debug
        Serial.print("Couleur du jour : ");
        Serial.println(couleur);
      }
    } else {
      Serial.println("Erreur lors de la requête HTTP");
    }

    http.end(); // Libérer les ressources


 http.begin(client,urlTomorrow);

  int httpCodeDemain = http.GET();
    if (httpCodeDemain > 0) {
      if (httpCodeDemain == HTTP_CODE_OK) {
        String payload = http.getString();

        // Parse JSON avec ArduinoJSON
        const size_t capacity = JSON_OBJECT_SIZE(3) + 100;
        DynamicJsonDocument doc(capacity);
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
          return;
        }

        // Extraire les valeurs
        int codeJour = doc["codeJour"];

        // Afficher la couleur en fonction du code
        //Code couleur du tarif Tempo applicable: 
        // - 0: tarif inconnu (pas encore communiqué par RTE) 
        // - 1: tarif bleu 
        // - 2: tarif blanc 
        // - 3: tarif rouge
        String couleur;
        switch(codeJour) {
          case 0:
            couleur = "INCONNU";
            break;
          case 1:
            couleur = "BLEU";
            digitalWrite(ledDemainBleu, HIGH);
            digitalWrite(ledDemainBlanc, LOW);
            digitalWrite(ledDemainRouge, LOW);
            break;
          case 2:
            couleur = "BLANC";
            digitalWrite(ledDemainBleu, LOW);
            digitalWrite(ledDemainBlanc, HIGH);
            digitalWrite(ledDemainRouge, LOW);
            break;
          case 3:
            couleur = "ROUGE";
            digitalWrite(ledDemainBleu, LOW);
            digitalWrite(ledDemainBlanc, LOW);
            digitalWrite(ledDemainRouge, HIGH);
            break;
          default:
            couleur = "INCONNU";
        }

        // Afficher le résultat debug
        Serial.print("Couleur de demain : ");
        Serial.println(couleur);
      }
    } else {
      Serial.println("Erreur lors de la requête HTTP");
    }

    http.end(); // Libérer les ressources


  delay(600000); // Attendre 5 minutes avant de recommencer
  //delay(10000); // Attendre 10 secondes avant de recommencer (pour debug)

}
