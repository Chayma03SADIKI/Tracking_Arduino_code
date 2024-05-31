#include <SoftwareSerial.h>
#include <AltSoftSerial.h>
#include <TinyGPS++.h>
 
// Définition des broches pour les modules GPS et GSM
#define GSM_RX 2
#define GSM_TX 3
#define GPS_RX 8
#define GPS_TX 9

// Définition du numéro de téléphone pour les alertes SMS
const String PHONE = "+xxxxxxxxxxxx";//Mettre le num de téléphone ici
// Initialisation des ports séries pour les modules GPS et GSM
SoftwareSerial GSM(GSM_RX, GSM_TX);
AltSoftSerial GPS;

// Initialisation de l'objet TinyGPS++ pour le traitement des données GPS
TinyGPSPlus gps;

// Coordonnées du centre de la géofence et son diametre, de centre INPT
float initialLatitude = 33.979832;
float initialLongitude = -6.868155;
float Diametre = 15000; // 15 km

// Variables pour la latitude et la longitude actuelles
float latitude, longitude;

// Fonction pour obtenir les données GPS
void getGps(float& latitude, float& longitude);

// Fonction pour calculer la distance entre deux points GPS
float getDistance(float flat1, float flon1, float flat2, float flon2);

// Fonction pour envoyer un message d'alerte SMS
void sendAlert();

void setup() {
  Serial.begin(9600);
  GSM.begin(9600);
  GPS.begin(9600);
  delay(1000);

  // Configuration du module GSM
  GSM.println("AT");
  delay(1000);
  GSM.println("ATE1"); // Activer l'écho
  delay(1000);
  GSM.println("AT+CPIN?"); // Vérifier si la carte SIM est prête
  delay(1000);
  GSM.println("AT+CMGF=1"); // Mode texte pour les SMS
  delay(1000);
  GSM.println("AT+CNMI=1,1,0,0,0"); // Gérer les nouveaux SMS entrants
  delay(1000);
}

void loop() {
  getGps(latitude, longitude);
  float distance = getDistance(latitude, longitude, initialLatitude, initialLongitude);

  Serial.print("Latitude: "); Serial.println(latitude, 6);
  Serial.print("Longitude: "); Serial.println(longitude, 6);
  Serial.print("Current Distance: "); Serial.println(distance);

  if (distance > Diametre) {
    sendAlert();
    delay(300000); // Attendre 5 minutes avant d'envoyer un autre message d'alerte
  }

  while (GPS.available()) {
    if (gps.encode(GPS.read())) {
      // Mise à jour des coordonnées GPS actuelles
      latitude = gps.location.lat();
      longitude = gps.location.lng();
    }
  }

  while (GSM.available()) {
    Serial.println(GSM.readString());
  }
  
  while (Serial.available()) {
    GSM.println(Serial.readString());
  }
}

void getGps(float& latitude, float& longitude) {
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 2000;) {
    while (GPS.available()) {
      if (gps.encode(GPS.read())) {
        newData = true;
        break;
      }
    }
  }
  
  if (newData) {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
  } else {
    Serial.println("No GPS data available");
    latitude = 0;
    longitude = 0;
  }
}

float getDistance(float flat1, float flon1, float flat2, float flon2) {
  float diflat = radians(flat2 - flat1);
  flat1 = radians(flat1);
  flat2 = radians(flat2);
  float diflon = radians(flon2 - flon1);
  float dist = 2 * asin(sqrt(sq(sin(diflat / 2)) + cos(flat1) * cos(flat2) * sq(sin(diflon / 2))));
  dist *= 6371000.0; // Conversion en mètres
  return dist;
}

void sendAlert() {
  String sms_data = "Alert! The Car is outside the fence.\r";
  sms_data += "http://maps.google.com/maps?q=loc:";
  sms_data += String(latitude) + "," + String(longitude);
  
  GSM.print("AT+CMGS=\""); // Commande pour envoyer un SMS
  GSM.print(PHONE);
  GSM.println("\"");
  delay(1000);
  GSM.print(sms_data);
  delay(100);
  GSM.write(0x1A); // ASCII code for Ctrl+Z (indique la fin du message SMS)
  delay(1000);
}
