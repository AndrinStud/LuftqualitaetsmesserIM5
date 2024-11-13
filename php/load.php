<?php
/***************************************************************************************************************************
 * load.php 
 * http://www.waschbaeraugen.ch/php/load.php
 * Daten als JSON-String vom MC empfangen und Daten in die Datenbank einfügen
***************************************************************************************************************************/

require_once("server_config.php");
echo "This script receives HTTP POST messages and pushes their content into the database.";

###################################### connect to db
try{
    $pdo = new PDO($dsn, $db_user, $db_pass, $options); 
    echo "</br> DB Verbindung ist erfolgreich";
}
catch(PDOException $e){
    error_log("DB Error: " . $e->getMessage());
    echo json_encode(["status" => "error", "message" => "DB connection failed"]);
}

###################################### Empfangen der JSON-Daten

$inputJSON = file_get_contents('php://input'); // JSON-Daten aus dem Body der Anfrage
$input = json_decode($inputJSON, true); // Dekodieren der JSON-Daten in ein Array


###################################### receiving a post request from a HTML form, later from ESP32 C6

$received_api_key = $sensor = $room = $airQuality = "";
$api_key_value = getenv('API_KEY');

if (isset($input["api_key"])) {
    $received_api_key = test_input($input["api_key"]);
    if($received_api_key == $api_key_value) {
        $room = test_input($input["room"]);
        $airQuality = test_input($input["air_quality"]);

        # insert new user into db
        $sql = "INSERT INTO air_quality_measure (room, air_quality) VALUES (?,?)";
        $stmt = $pdo->prepare($sql);
        $stmt->execute([$room, $airQuality]);
    }
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>