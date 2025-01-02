#include <WiFi.h>
#include "time.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char* ssid = "Maiz";  //--> Your wifi name
const char* password = "rastafouille"; //--> Your wifi password
// leds
const int ledJourBleu = 15;
const int ledJourBlanc = 2;
const int ledJourRouge = 4;
const int ledDemainBleu = 16;
const int ledDemainBlanc = 17;
const int ledDemainRouge = 5;

String couleurJourJ;
String couleurJourJ1;


void setup() {
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

  // Construire l'URL avec la date formatée
  String urlToDay = "https://www.api-couleur-tempo.fr/api/jourTempo/today";
  //Serial.println(urlToDay); // debug url
  String urlTomorrow = "https://www.api-couleur-tempo.fr/api/jourTempo/tomorrow";
  //Serial.println(urlTomorrow); // debug url
  // Effectuer la requête HTTP

  HTTPClient http;
  http.begin(urlToDay);
  int httpCodeTempo = http.GET();
  if (httpCodeTempo > 0) {
    if (httpCodeTempo == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.print("payloadDe = ");Serial.println(payload);

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
        Serial.print("codeJour = ");Serial.println(codeJour);
 
        // Afficher la couleur en fonction du code
        //Code couleur du tarif Tempo applicable: 
        // - 0: tarif inconnu (pas encore communiqué par RTE) 
        // - 1: tarif bleu 
        // - 2: tarif blanc 
        // - 3: tarif rouge
        switch(codeJour) {
          case 0:
            couleurJourJ = "INCONNU";
            break;
          case 1:
            couleurJourJ = "BLEU";
            digitalWrite(ledJourBleu, HIGH);
            digitalWrite(ledJourBlanc, LOW);
            digitalWrite(ledJourRouge, LOW);
            break;
          case 2:
            couleurJourJ = "BLANC";
            digitalWrite(ledJourBleu, LOW);
            digitalWrite(ledJourBlanc, HIGH);
            digitalWrite(ledJourRouge, LOW);
            break;
          case 3:
            couleurJourJ = "ROUGE";
            digitalWrite(ledJourBleu, LOW);
            digitalWrite(ledJourBlanc, LOW);
            digitalWrite(ledJourRouge, HIGH);
            break;
          default:
            couleurJourJ = "INCONNU";
        }
    } // fin if (httpCode == HTTP_CODE_OK) {
  } else {
    Serial.println("TEMPO - Erreur de connexion au serveur EDF");
  } //if (httpCode > 0) {
  http.end();

  http.begin(urlTomorrow);
  int httpCodeDemain = http.GET();
    if (httpCodeDemain > 0) {
      if (httpCodeDemain == HTTP_CODE_OK) {
        String payload = http.getString();
                Serial.print("payloadDemain = ");Serial.println(payload);
 
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
        Serial.print("codeDemain = ");Serial.println(codeJour);

        switch(codeJour) {
          case 0:
            couleurJourJ1 = "INCONNU";
            break;
          case 1:
            couleurJourJ1 = "BLEU";
            digitalWrite(ledDemainBleu, HIGH);
            digitalWrite(ledDemainBlanc, LOW);
            digitalWrite(ledDemainRouge, LOW);
            break;
          case 2:
            couleurJourJ1 = "BLANC";
            digitalWrite(ledDemainBleu, LOW);
            digitalWrite(ledDemainBlanc, HIGH);
            digitalWrite(ledDemainRouge, LOW);
            break;
          case 3:
            couleurJourJ1 = "ROUGE";
            digitalWrite(ledDemainBleu, LOW);
            digitalWrite(ledDemainBlanc, LOW);
            digitalWrite(ledDemainRouge, HIGH);
            break;
          default:
            couleurJourJ1 = "INCONNU";
        }
      }
    } else {
      Serial.println("TEMPO - Erreur de connexion au serveur EDF");
     }
    http.end(); // Libérer les ressources

      // DEBUG
      Serial.print("CouleurJourJ: ");
      Serial.println(couleurJourJ);
      Serial.print("CouleurJourJ1: ");
      Serial.println(couleurJourJ1);
  delay(600000); // Attendre 5 minutes avant de recommencer
  //delay(10000); // Attendre 10 secondes avant de recommencer (pour debug)


}
