// SOURCES
// https://forum.arduino.cc/t/api-rte-ecowatt/1017281/37
// https://randomnerdtutorials.com/esp32-http-get-post-arduino/
// https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/
// https://data.rte-france.com/catalog/-/api/doc/user-guide/Tempo+Like+Supply+Contract/1.1#TLCRG02
// MAKERTRONIC - https://www.youtube.com/watch?v=mw6xLQAqiKs&lc=UgwY5Fh_WTROy7Fow6h4AaABAg.9z1ZN67ggNI9z23zYdJtDm


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// *****************************************
//     ! ATTENTION ELEMENTS SECRETS !
// *****************************************

const char* ssid =          "xxx";
const char* password =      "xxx";


// le certificat racine (format PEM) de https://particulier.edf.fr/
const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIKETCCB/mgAwIBAgIURVq8Yhrnh9iN9Xt9VJX+q8eS71swDQYJKoZIhvcNAQEL\n" \
"BQAwVjELMAkGA1UEBhMCTkwxIDAeBgNVBAoMF1F1b1ZhZGlzIFRydXN0bGluayBC\n" \
"LlYuMSUwIwYDVQQDDBxRdW9WYWRpcyBFdXJvcGUgRVYgU1NMIENBIEcxMB4XDTIz\n" \
"MDYxMzE0MTM0OFoXDTI0MDYxMzE0MDgwMFowgfcxEzARBgsrBgEEAYI3PAIBAxMC\n" \
"RlIxHjAcBgsrBgEEAYI3PAIBAgwNSWxlLWRlLUZyYW5jZTEWMBQGCysGAQQBgjc8\n" \
"AgEBDAVQYXJpczEdMBsGA1UEDwwUUHJpdmF0ZSBPcmdhbml6YXRpb24xFDASBgNV\n" \
"BAUTCzU1MiAwODEgMzE3MQswCQYDVQQGEwJGUjEWMBQGA1UECAwNSWxlLWRlLUZy\n" \
"YW5jZTEOMAwGA1UEBwwFUGFyaXMxITAfBgNVBAoMGEVMRUNUUklDSVRFIERFIEZS\n" \
"QU5DRSBTQTEbMBkGA1UEAwwScGFydGljdWxpZXIuZWRmLmZyMIIBIjANBgkqhkiG\n" \
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwfm+mBpovkFjpZqJbBvo3NwNNhvwP56zFUQU\n" \
"ChGfGOmzaP+x1kvppq8JaDsLxRVrf6wQZ2f1BFHIG6i63mNrM8Qh7p1GqV/GuUFF\n" \
"ZH1RLSsIYGHIPADxqCcyDH1Wui4P7gQiyGikfwfLoy4aBCXdmf/1rKB72z7Xn6DI\n" \
"MppfcJ/R0FWdTNYkV0v/AYa+tUZKGF9/ieUwGm/tCNxEaMoB75ElXE/MCZwyMhhu\n" \
"O9mPuweuTsgfhi37lV44/p29PKLxPkYa4Dyu11UTal+5eZ2iGConKcukr0PzrOHO\n" \
"VFVqA/pI1EeH4J2Hh22e9xIP4zuVjR4TWRAbhfMHJ+v1v2aJ5QIDAQABo4IFMzCC\n" \
"BS8wDAYDVR0TAQH/BAIwADAfBgNVHSMEGDAWgBRJPQ7cecXL2xKj1Q2r8s+pr+Nm\n" \
"qTCBgwYIKwYBBQUHAQEEdzB1MEcGCCsGAQUFBzAChjtodHRwOi8vdHJ1c3QucXVv\n" \
"dmFkaXNnbG9iYWwuY29tL3F1b3ZhZGlzZXVyb3BlZXZzc2xjYWcxLmNydDAqBggr\n" \
"BgEFBQcwAYYeaHR0cDovL29jc3AucXVvdmFkaXNnbG9iYWwuY29tMIIBgwYDVR0R\n" \
"BIIBejCCAXaCEnBhcnRpY3VsaWVyLmVkZi5mcoIeY21zbW9iaWxlLXBhcnRpY3Vs\n" \
"aWVycy5lZGYuY29tghZlc3BhY2Utc2VjdXJpc2UuZWRmLmZygg9lZGZldG1vaS5l\n" \
"ZGYuZnKCHndzLW1vYmlsZS1wYXJ0aWN1bGllcnMuZWRmLmNvbYIQc291c2NyaXJl\n" \
"LmVkZi5mcoIUZXNwYWNlLWNsaWVudC5lZGYuZnKCGnBhc3MtY29sbGVjdGl2aXRl\n" \
"cy5lZGYuY29tggtzc28uZWRmLmNvbYIYYXBpLXBhcnRpY3VsaWVycy5lZGYuY29t\n" \
"ghdtb25jb21wdGUub2RpY2VlLmVkZi5mcoIKc3NvLmVkZi5mcoIQYXV0aC1hb2Rl\n" \
"LmVkZi5mcoIZaWQtZXNwYWNlLXNlY3VyaXNlLmVkZi5mcoIlYXV0aC5lbnRyZXBy\n" \
"aXNlcy1jb2xsZWN0aXZpdGVzLmVkZi5mcoITYXBpLWNvbW1lcmNlLmVkZi5mcjBa\n" \
"BgNVHSAEUzBRMEYGDCsGAQQBvlgAAmQBAjA2MDQGCCsGAQUFBwIBFihodHRwOi8v\n" \
"d3d3LnF1b3ZhZGlzZ2xvYmFsLmNvbS9yZXBvc2l0b3J5MAcGBWeBDAEBMB0GA1Ud\n" \
"JQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATBKBgNVHR8EQzBBMD+gPaA7hjlodHRw\n" \
"Oi8vY3JsLnF1b3ZhZGlzZ2xvYmFsLmNvbS9xdW92YWRpc2V1cm9wZWV2c3NsY2Fn\n" \
"MS5jcmwwHQYDVR0OBBYEFKW9DWK/lGs4caC+Ox6Jf+rvGppfMA4GA1UdDwEB/wQE\n" \
"AwIFoDCCAfkGCisGAQQB1nkCBAIEggHpBIIB5QHjAHcA2ra/az+1tiKfm8K7XGvo\n" \
"cJFxbLtRhIU0vaQ9MEjX+6sAAAGItSQ28AAABAMASDBGAiEA4CQkIMXpoiN57qjQ\n" \
"0gEFEKO0/j+QlkDclAlBcK8SDOACIQDBYTD1BUQqWG27HYG8vPI10GWk83d6u/Fb\n" \
"jRX0UCOPCwB3AO7N0GTV2xrOxVy3nbTNE6Iyh0Z8vOzew1FIWUZxH7WbAAABiLUk\n" \
"NqEAAAQDAEgwRgIhAMEX/Ra4uXHx4Ukp7hwaAVTw1nhc/W5yvQu98/sEYXXfAiEA\n" \
"mNCJcSqNZm7X4tmiCIw9qGkuxD9O184Jm5cJHYgfd7UAdgA7U3d1Pi25gE6LMFsG\n" \
"/kA7Z9hPw/THvQANLXJv4frUFwAAAYi1JDdsAAAEAwBHMEUCIDJMyDXSRGNR+g8w\n" \
"lHWz9BTTEXnu8wtH7DFEXqtcELJ4AiEA85Z/LIKmYf4oZnQLIobTalG6g0koRLLc\n" \
"8uY3cxeV96gAdwB2/4g/Crb7lVHCYcz1h7o0tKTNuyncaEIKn+ZnTFo6dAAAAYi1\n" \
"JDZwAAAEAwBIMEYCIQCU2uA4zo79pCxqXHYUmq4DEJYb49lZdQFbtrcv+sfuxgIh\n" \
"AOgrgyISgfS9iZcHdy13K6bkVuJos09LrijSvj7oduqXMA0GCSqGSIb3DQEBCwUA\n" \
"A4ICAQCfcN40U5iyLVmjy48GALwzsZC9VffA1c4nCPjBH7SwjuJCPo3eCJuDwujs\n" \
"qFdsR1ZUk/GaLp9s1I6ogCJ4Xg3zUVvjwNnEYFiuudeyySnLJhgeVbSZdREZZnuW\n" \
"wFE0Us6EOtzM+/o1HJNgPs/9ahQ4wNCqj326VAAWcx030Z41Yr9DE2nkCraaAEOM\n" \
"ouO4wIeUA1+u8Pfe9qpLDG4XmyfaFqJAdLH5t/W3Szu2vVbhg/5EJN+bm1+DBboM\n" \
"A9CfRteoBw/ig50vgBdTu2N6sVCojpPrUtfwVxNfEyeFvvOB0dycTkk6g8SKVMuV\n" \
"0pCsKOII/WS9N4Hv5TFmf0gs/8UpocPSIYZPTAZAc08rSvUVVkzXbjQdWzz8Amq0\n" \
"8+0QpmPym7LFo2gr/trZhcwqBoo9rISey86bnaKgetAniQ1FI/JUufBplreQeF+q\n" \
"3jyD8C16LIbQGD9OBfBFb6nxp/eSu+SGKF3v11fCP13KVMb9Pp5QwzHME1SWyBqO\n" \
"QwJQyfXtd/JlkGdrnYWIhO7tgXxfwv0VN2532l3EEaHM+7yJJ5nOD9hUJkjHjg0s\n" \
"ODotkGauNqGT/qdHRmjNpv3gixNYIicgeRVPlSZvsd9nw2smkiIc7OnrYatVKaMY\n" \
"ekMnQ9bu2N4TSqb8DwsM0PtJqfGQaTwdRu5AD8SL47a+1dopBw==\n" \
"-----END CERTIFICATE-----\n";


// *****************************************
// serveur NTP
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 1;
const int   daylightOffset_sec = 3600 * 0;
 
// leds
const int ledJourBleu = 0;
const int ledJourBlanc = 12;
const int ledJourRouge = 2;
const int ledDemainBleu = 13;
const int ledDemainBlanc = 4;
const int ledDemainRouge = 5;


  void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.write('.');
  }
  Serial.print("\nConnected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
    // On configure le seveur NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

        // leds en sortie
    pinMode(ledJourBleu, OUTPUT);
    pinMode(ledJourBlanc, OUTPUT);
    pinMode(ledJourRouge, OUTPUT);
    pinMode(ledDemainBleu, OUTPUT);
    pinMode(ledDemainBlanc, OUTPUT);
    pinMode(ledDemainRouge, OUTPUT);
    digitalWrite(ledJourBleu, HIGH);

}


void loop() {

// recup du jour de l'année format AAAA/MM/JJ
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Erreur de recup de l'heure");
  }
 
  char strftime_buf[20];
  strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d", &timeinfo);
  
  //Serial.println(strftime_buf); // debug date du jour

// Construire l'URL avec la date formatée
  String url = "https://particulier.edf.fr/services/rest/referentiel/searchTempoStore?dateRelevant=" + String(strftime_buf);
  Serial.println(url); // debug url
 
  // Effectuer la requête HTTP
  HTTPClient http;
  WiFiClientSecure client;
  X509List cert(root_ca);
  client.setTrustAnchors(&cert);
  http.begin(client,url);


  int httpCode = http.GET();
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
 
      // Parse JSON avec ArduinoJSON
      const size_t capacity = JSON_OBJECT_SIZE(2) + 100;
      DynamicJsonDocument doc(capacity);
      deserializeJson(doc, payload);
 
      // Extraire les valeurs
      const char* couleurJourJ = doc["couleurJourJ"];
      const char* couleurJourJ1 = doc["couleurJourJ1"];

      digitalWrite(ledJourBleu, LOW);
      digitalWrite(ledJourBlanc, LOW);
      digitalWrite(ledJourRouge, LOW);
      digitalWrite(ledDemainBleu, LOW);
      digitalWrite(ledDemainBlanc, LOW);
      digitalWrite(ledDemainRouge, LOW);

      // DEBUG
      Serial.print("CouleurJourJ: ");
      Serial.println(couleurJourJ);
      Serial.print("CouleurJourJ1: ");
      Serial.println(couleurJourJ1);
 
      // Affichage des LEDs du jour
      Serial.print("CouleurJourJ: ");
      if (strcmp(couleurJourJ, "TEMPO_BLEU") == 0) {
        Serial.println("bleu");
        digitalWrite(ledJourBleu, HIGH);
      }
      if (strcmp(couleurJourJ, "TEMPO_BLANC") == 0) {
        Serial.println("blanc");
        digitalWrite(ledJourBlanc, HIGH);
      }
      if (strcmp(couleurJourJ, "TEMPO_ROUGE") == 0) {
        Serial.println("rouge");
        digitalWrite(ledJourRouge, HIGH);
      }
 
      // Affichage des LEDs du lendemain
      Serial.print("CouleurJourJ1: ");
      if (strcmp(couleurJourJ1, "TEMPO_BLEU") == 0) {
        Serial.println("bleu");
        digitalWrite(ledDemainBleu, HIGH);
      }
      if (strcmp(couleurJourJ1, "TEMPO_BLANC") == 0) {
        Serial.println("blanc");
        digitalWrite(ledDemainBlanc, HIGH);
      }
      if (strcmp(couleurJourJ1, "TEMPO_ROUGE") == 0) {
        Serial.println("rouge");
        digitalWrite(ledDemainRouge, HIGH);
      }
     
    } // fin if (httpCode == HTTP_CODE_OK) {
 
  } else {
    Serial.println("Erreur de connexion au serveur EDF");
  } //if (httpCode > 0) {
 
  http.end();
 
  //delay(300000); // Attendre 5 minutes avant de recommencer
  delay(10000); // Attendre 10 secondes avant de recommencer (pour debug)





}