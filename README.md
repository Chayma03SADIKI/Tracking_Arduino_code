GPS-GSM Geofence Alert System
Overview
This project implements a geofencing system using GPS and GSM modules. It alerts via SMS when a device (e.g., a car) moves outside a predefined geographical boundary.

Components Used
Hardware:
GPS Module (AltSoftSerial)
GSM Module (SoftwareSerial)

Software Libraries:
TinyGPS++ for GPS data parsing
Functionality

The system continuously monitors the current GPS coordinates of the device and calculates the distance from a predefined center of the geofence. If the device moves outside the specified boundary (diameter of 15 km in this case), it triggers an SMS alert to a predefined phone number.

Setup Instructions

Connections:
GPS RX to Arduino pin 8
GPS TX to Arduino pin 9
GSM RX to Arduino pin 2
GSM TX to Arduino pin 3

Software Configuration:
Upload the provided Arduino sketch (geofence_alert.ino) to your Arduino board.
Ensure the baud rates for GPS and GSM modules are set correctly (9600 in this example).

SIM Card:
Insert a compatible SIM card into the GSM module with sufficient SMS capabilities.

Testing:
Power on the Arduino board and monitor the Serial Monitor for GPS coordinates and distance calculations.
Verify SMS alerts are sent when the device moves outside the geofence.

Notes
Adjust the PHONE constant to your desired recipient phone number for SMS alerts.
Modify initialLatitude, initialLongitude, and Diametre variables to set your geofence center and diameter.

Resources

TinyGPS++ Library

Arduino SoftwareSerial Library
