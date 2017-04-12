 <?php

//Acquire get parameters

if ($_SERVER["REQUEST_METHOD"] == "GET") {
  $device = $_GET["device"];
  $open = $_GET["open"];
}

if ($_SERVER["REQUEST_METHOD"] == "POST") {
  $device = $_POST["device"];
  $open = $_POST["open"];
}

//Establish database connection
$servername = "localhost";
$username = "root";
$password = "nut8A5+-";
$dbname = "testdb";
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
$sql = "INSERT INTO `OpenDetector` (`ID`, `DEVICE`, `OPEN`, `TIMESTAMP`) VALUES (NULL, '". $device ."', '" . $open ."', CURRENT_TIMESTAMP)";
if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}
$conn->close();
?> 
