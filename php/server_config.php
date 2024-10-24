<?php
/*************************************************************
 * server_config.php
 * Datenbank-Verbindung
 * http://www.waschbaeraugen.ch/php/server_config.php
 *************************************************************/

error_reporting(E_ALL & ~E_DEPRECATED);

require '../dependencies/phpdotenv-2.5.2/src/Dotenv.php';
require '../dependencies/phpdotenv-2.5.2/src/Loader.php';
require '../dependencies/phpdotenv-2.5.2/src/Exception/ExceptionInterface.php';
require '../dependencies/phpdotenv-2.5.2/src/Exception/InvalidPathException.php';
use Dotenv\Dotenv;

$dotenv = new Dotenv(dirname(__DIR__));
$dotenv->load();

$db_host = getenv('DB_HOST');
$db_name = getenv('DB_NAME');
$db_user = getenv('DB_USER');
$db_pass = getenv('DB_PASS');

$db_charset = "utf8";

$dsn = "mysql:host=$db_host;dbname=$db_name;charset=$db_charset";
$options = [
    PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    PDO::ATTR_EMULATE_PREPARES   => false,
];
?>