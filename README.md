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

## Schritt 3: Mikroprozessor richtig stecken:
![](img/Steckplan.jpg?raw=true)

## Schritt 4: Mikroprozessorentwicklung einrichten
1. Python installieren falls noch nicht geschehen
2. Die PlatformIO IDE Extension von PlatformIO installieren
3. Darin den microprocessor Ordner öffnen
4. Die notwendigen Dateien zur Entwicklung werden automatisch erstellt
5. Eigentlich sollte gemäss platformIO.ini Datei im microprocessor Ordner die generate_env_config.py (im selben Ordner) bei der Arduino Kompilierung automatisch ausgeführt werden. Da dies aber noch nicht funktioniert, muss das manuell gemacht werden. Über die Kommandozeile kann das Skript ausgeführt werden.