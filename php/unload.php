<?php
/***************************************************************************************************************************
 * unload.php 
 * http://www.waschbaeraugen.ch/php/unload.php
 * Daten aus der Datenbank holen und als JSON-String zurückgeben, sobald main.js dieses Script per HTTP aufruft (fetch())
***************************************************************************************************************************/


########################################################## Wenn main.js dieses Script per HTTP aufruft (fetch()), bekommt es einen JSON-String im Body der Return-HTTP-Nachricht zurück.

require_once("server_config.php");
// echo "This script fetches database entries and makes them available to use for visualizing.";

try {
    $creationDate = [];
    $airQuality = [];

    $pdo = new PDO($dsn, $db_user, $db_pass);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    $sql = "SELECT * FROM air_quality_measure WHERE room = '4184bb7c-91fb-11ef-be33-c4cbe1d62274' ORDER BY creation_date DESC LIMIT 100"; # nur letzten 200 Daten holen

    $stmt = $pdo->prepare($sql);
    $stmt->execute();
    while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
        $airQuality[] = $row['air_quality'];
        $creationDate[] = $row['creation_date'];
    }

    // chart.js will zwei Arrays haben, damit eines für die Werte der Kurve und eines für die Beschriftungen auf der x-Achse verwendet werden kann
    $data = [
        'airQuality' => array_map('floatval', $airQuality),
        'creationDate' => array_values($creationDate)
    ];
    $json = json_encode(['data' => $data]);

    echo $json;


} catch (PDOException $e) {
    echo "Error: " . $e->getMessage();
}
