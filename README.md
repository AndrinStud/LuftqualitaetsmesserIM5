# Inhaltsverzeichnis
- [Dokumentation](#dokumentation)
  - [Zweck des Projekts und Link](#zweck-des-projekts-und-link)
  - [Aufbau und Funktionsweise Code](#aufbau-und-funktionsweise-code)
    - [Mikroprozessorcode](#mikroprozessorcode)
    - [Datenbankaufbau](#datenbankaufbau)
    - [Datenverarbeitung (PHP)](#datenverarbeitung-php)
    - [Visualisierung (HTML und JavaScript)](#visualisierung-html-und-javascript)
  - [Known Bugs](#known-bugs)
  - [Reflexion und Umsetzungsprozess](#reflexion-und-umsetzungsprozess)
    - [Herausforderungen beim Verständnis der Schaltpläne](#herausforderungen-beim-verständnis-der-schaltpläne)
    - [Programmierung in VSCode](#programmierung-in-vscode)
    - [Probleme bei der Verteilung der Luftqualitätsmesser](#probleme-bei-der-verteilung-der-luftqualitätsmesser)
    - [Sicherheitsmassnahmen](#sicherheitsmassnahmen)
    - [Nutzung von Copilot](#nutzung-von-copilot)
- [Anleitung zum Start (Programmierung in VS Code)](#anleitung-zum-start-programmierung-in-vs-code)
  - [Schritt 1: Variablen für Visual Studio SFTP-Extension einrichten](#schritt-1-variablen-für-visual-studio-sftp-extension-einrichten)
  - [Schritt 2: Umgebungsvariablen einrichten](#schritt-2-umgebungsvariablen-einrichten)
  - [Schritt 3: Mikroprozessorsteckplan richtig stecken](#schritt-3-mikroprozessorsteckplan-richtig-stecken)
  - [Schritt 4: Mikroprozessorentwicklung einrichten](#schritt-4-mikroprozessorentwicklung-einrichten)

# Dokumentation

## Zweck des Projekts und Link
Der Zweck dieses Projekts ist es, die Luftqualität in verschiedenen Räumen des Medienhauses der FHGR zu überwachen. Durch die kontinuierliche Messung und Visualisierung der Luftqualität können potenzielle Gesundheitsrisiken frühzeitig erkannt und entsprechende Massnahmen ergriffen werden. Das System besteht aus mehreren Luftqualitätsmessern, die Sensordaten erfassen und diese an einen Server senden, wo sie verarbeitet und visualisiert werden.

Das Projekt kann hier getestet werden: [Luftqualitätsmonitor Medienhaus](http://www.waschbaeraugen.ch/)

## Aufbau und Funktionsweise Code
### Mikroprozessorcode
Der Mikroprozessorcode ist in C++ geschrieben und läuft auf einem ESP32-C6 Mikroprozessor. Die Hauptaufgaben des Codes sind:
- Initialisierung der Hardwarekomponenten (Gassensor, WLAN-Modul)
- Regelmässiges Auslesen der Sensordaten
- Senden der Sensordaten an den Server

Die Hauptdatei `src.ino` steuert den Ablauf und verwendet Bibliotheken wie `WiFi.h` für die Netzwerkverbindung und `Arduino_JSON.h` für die JSON-Verarbeitung. Die Datei `generate_env_config.py` wird verwendet, um Umgebungsvariablen in eine Header-Datei zu konvertieren.

#### Wichtige Funktionen:
- **`setup()`**: Initialisiert die WLAN-Verbindung und legt den SensorPin und die Raum-ID fest.
- **`loop()`**: Liest die Sensordaten in regelmässigen Abständen aus und sendet sie als JSON-String per HTTP POST Request an den Server.

### Datenbankaufbau
Die Daten werden in einer MySQL-Datenbank gespeichert. Der Aufbau der Datenbank ist wie folgt:
- **Tabelle `room`**: Speichert Informationen über die verschiedenen Räume mit den Spalten `id`, `name`, `creation_date`.
- **Tabelle `air_quality_measure`**: Speichert die Sensordaten mit den Spalten `id`, `room`, `air_quality`, `creation_date`.

Die Datei `createTables.sql` enthält die SQL-Befehle zum Erstellen der notwendigen Tabellen und zum Einfügen der Raumdaten.

### Datenverarbeitung (PHP)
Die Datenverarbeitung auf dem Server erfolgt mittels PHP-Skripten. Diese Skripte empfangen die Sensordaten vom Mikroprozessor und speichern sie in der Datenbank. Wichtige Dateien sind:
- **`load.php`**: Empfängt die Daten vom Mikroprozessor und fügt sie in die Datenbank ein.
  - Verbindet sich mit der Datenbank.
  - Empfängt JSON-Daten vom Mikroprozessor.
  - Fügt die empfangenen Daten in die Tabelle `air_quality_measure` ein.
- **`unload.php`**: Stellt eine API zur Verfügung, um die gespeicherten Daten abzurufen.
  - Verbindet sich mit der Datenbank.
  - Holt die letzten 200 Einträge der Luftqualitätsmessungen für jeden Raum.
  - Gibt die Daten als JSON-String zurück.
- **`server_config.php`**: Enthält die Konfigurationsdaten für die Datenbankverbindung.
  - Lädt die Umgebungsvariablen aus der `.env` Datei.
  - Stellt die Datenbankverbindungsparameter bereit.

### Visualisierung (HTML und JavaScript)
Die Visualisierung der Daten erfolgt über eine Weboberfläche, die mit HTML und JavaScript erstellt wurde. Die Hauptkomponenten sind:
- **`index.html`**: Die Hauptseite, die die Benutzeroberfläche darstellt.
  - Enthält ein Canvas-Element zur Darstellung der Diagramme.
  - Bindet die JavaScript-Datei `main.js` ein.
- **`main.js`**: Enthält die Logik zur Datenabfrage von der API und zur Darstellung der Daten in Diagrammen.
  - **`fetchData()`**: Holt die Daten von der API (`unload.php`).
  - **`main()`**: Verarbeitet die abgerufenen Daten und erstellt die Diagramme mit Chart.js.

Die Daten werden in Echtzeit aktualisiert und in Diagrammen dargestellt, um die Luftqualität visuell zu überwachen.

## Known Bugs
- Auf einigen Geräten und Browsern funktioniert die ChartJS Darstellung nicht

## Reflexion und Umsetzungsprozess

### Herausforderungen beim Verständnis der Schaltpläne
Zu Beginn des Projekts hatte ich Schwierigkeiten, die Schaltpläne zu verstehen, da ich mich nicht gut mit Elektrizität auskenne. Es hat einige Zeit gedauert, bis ich die verschiedenen Komponenten und deren Verbindungen richtig nachvollziehen konnte. Durch kontinuierliches Lernen und praktische Anwendung konnte ich jedoch mein Verständnis verbessern.

### Programmierung in VSCode
Ein weiterer wichtiger Schritt war die Einrichtung der Entwicklungsumgebung. Ich habe einige Zeit daran gearbeitet, sowohl PHP als auch Arduino im VSCode programmieren zu können. Dank der PlatformIO IDE Extension konnte ich dies erfolgreich umsetzen. Diese Erweiterung hat die Entwicklung und das Debugging erheblich erleichtert.

### Probleme bei der Verteilung der Luftqualitätsmesser
Beim Verteilen der Luftqualitätsmesser bin ich immer wieder auf Probleme mit den FHGR Services gestossen. Diese Probleme hätten durch frühzeitige Abklärung und Kommunikation vermieden werden können. Bei einem nächsten Mal würde ich diese Aspekte früher im Projektverlauf klären, um Verzögerungen zu vermeiden.

### Sicherheitsmassnahmen
Aus Sicherheitsgründen habe ich alle meine Passwörter und ähnliche sensible Informationen in eine `.env` Datei geschrieben. Dies stellt sicher, dass diese Daten nicht im GitHub-Repository landen und somit nicht öffentlich zugänglich sind. Diese Praxis hat die Sicherheit meiner Anwendung erheblich verbessert.

### Nutzung von Copilot
Um die Effizienz bei der Entwicklung zu steigern, habe ich zum Teil mit GitHub Copilot gearbeitet. Diese KI-gestützte Programmierhilfe hat mir geholfen, schneller Lösungen zu finden und den Code effizienter zu schreiben.

# Anleitung zum Start (Programmierung in VS Code)
## Schritt 1: Variablen für Visual Studio SFTP-Extension einrichten
1. Die SFTP Extension von Natizyskunk installieren
2. Auf oberster Ebene Ordner **.vscode** erstellen
3. Darin eine Datei mit dem Namen **sftp.json** erstellen
4. Folgendes muss dort rein:

    	{
          "name": "DEIN WUNSCHNAME",
          "host": "HOST",
          "protocol": "ftp",
          "port": 21,
          "username": "USERNAME",
          "password": "PASSWORD",
          "remotePath": "/",
          "uploadOnSave": true,
          "useTempFile": false,
          "openSsh": false,
          "ignore": [
              ".vscode",
              "README.md",
              ".git",
              ".DS_Store",
              ".gitignore",
              "createTables.sql",
              "microprocessor"
          ]
      }
   
    > **Wichtig:** *HOST*, *USERNAME* und *PASSWORD* müssen nun durch die FTP-Anmeldedaten des Entwicklungsserver ersetzt werden!

## Schritt 2: Umgebungsvariablen einrichten
1. Auf oberster Ebene Datei **.env** erstellen
2. Folgendes muss dort rein:

    	DB_NAME=DATENBANKNAME
    	DB_USER=BENUTZERNAME
    	DB_PASS=PASSWORT
        NETWORK_SSID=NETZWERKNAME
        NETWORK_PASS=NETZWERKPASSWORT
        API_KEY=APISCHLUESSEL

    > **Wichtig:** *DATENBANKHOST*, *DATENBANKNAME*, *BENUTZERNAME* und *PASSWORT* müssen durch die Anmeldedaten der Datenbank deines Entwicklungsserver ersetzt werden. *NETZWERKNAME* und *NETZWERKPASSWORT* müssen durch die Netzwerk Logindaten für den Mikroprozessor ersetzt werden. APISCHLUESSEL muss durch einen beliebigen API-Schlüssel ersetzt werden.

## Schritt 3: Mikroprozessorsteckplan richtig stecken:
Folgende Komponenten braucht es für einen (der fünf) Luftqualitätsmesser:
- 5 Kabel (2x positiv, 2x negativ, 1x Signal)
- Steckboard
- Mikroprozessor esp32-c6
- Gassensor
- Kompensator

Damit kann dann folgendermassen gesteckt werden:
![](img/Steckplan.jpg?raw=true)

## Schritt 4: Mikroprozessorentwicklung einrichten
1. Python installieren falls noch nicht geschehen
2. Die PlatformIO IDE Extension von PlatformIO installieren
3. Darin den microprocessor Ordner öffnen
4. Die notwendigen Dateien zur Entwicklung werden automatisch erstellt
5. Eigentlich sollte gemäss platformIO.ini Datei im microprocessor Ordner die generate_env_config.py (im selben Ordner) bei der Arduino Kompilierung automatisch ausgeführt werden. Da dies aber noch nicht funktioniert, muss das manuell gemacht werden. Über die Kommandozeile kann das Skript ausgeführt werden.