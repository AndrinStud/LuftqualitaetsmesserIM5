<?php
/***************************************************************************************************************************
 * unload.php 
 * http://www.waschbaeraugen.ch/php/unload.php
 * Daten aus der Datenbank holen und als JSON-String zurückgeben, sobald main.js dieses Script per HTTP aufruft (fetch())
***************************************************************************************************************************/


########################################################## Wenn main.js dieses Script per HTTP aufruft (fetch()), bekommt es einen JSON-String im Body der Return-HTTP-Nachricht zurück.

require_once("server_config.php");
// echo "This script fetches database entries and makes them available to use for visualizing.";

function roundToQuarterHour($datetime) {
    $timestamp = strtotime($datetime);
    $minutes = date('i', $timestamp);
    $roundedMinutes = round($minutes / 15) * 15;
    return date('Y-m-d H:i:s', mktime(date('H', $timestamp), $roundedMinutes, 0, date('m', $timestamp), date('d', $timestamp), date('Y', $timestamp)));
}

try {
    $data = [];

    $pdo = new PDO($dsn, $db_user, $db_pass);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    $rooms = $pdo->query("SELECT id, name FROM room");

    while ($room = $rooms->fetch(PDO::FETCH_ASSOC)) {
        $roomId = $room['id'];
        $roomName = $room['name'];

        $sql = "SELECT * FROM air_quality_measure WHERE room = :room ORDER BY creation_date DESC LIMIT 200";
        $stmt = $pdo->prepare($sql);
        $stmt->execute(['room' => $roomId]);

        $airQuality = [];
        $creationDate = [];
        while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
            $airQuality[] = $row['air_quality'];
            $creationDate[] = roundToQuarterHour($row['creation_date']);
        }

        $data[$roomName] = [
            'airQuality' => array_map('floatval', $airQuality),
            'creationDate' => array_values($creationDate)
        ];
    }

    $json = json_encode(['data' => $data]);
    echo $json;

} catch (PDOException $e) {
    echo "Error: " . $e->getMessage();
}
