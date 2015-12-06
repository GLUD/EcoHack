<html>
<body>
<?php
include 'connect.php';

$temperatura = htmlspecialchars($_GET["temp"]);
$humedad = htmlspecialchars($_GET["hum"]);
$ozono = htmlspecialchars($_GET["ozon"]);
$uv  = htmlspecialchars($_GET["uv"]);
$co = htmlspecialchars($_GET["co"]);
$cov = htmlspecialchars($_GET["cov"]);
$hs = htmlspecialchars($_GET["hs"]);
//$gps = htmlspecialchars($_GET["caffe"]);
//$fecha= date("Y/m/d");
//$hora=date("h:i:sa");
$sql = "INSERT INTO datos (temperatura,humedad,ozono,uv,co,cov,hs) VALUES (". $temperatura.",".$humedad." ,'".$ozono."',".$uv." ,".$co." ,".$cov." ,".$hs." )";
if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();

?>
<br>La temperatura esta en: <?php echo $_GET["temp"]; ?> <br>
El porcentaje de  humedad  esta en  <?php echo $_GET["hum"]; ?><br>
El nivel de ozono esta en: <?php echo $_GET["ozon"]; ?> <br>
La radiacion UV  esta en: <?php echo $_GET["uv"]; ?> <br>
La cantidad de dioxido de carbono esta en: <?php echo $_GET["co"]; ?> <br>
La calidad de aire esta  en: <?php echo $_GET["cov"]; ?> <br>
La humedad del suelo esta en: <?php echo $_GET["hs"]; ?> <br>
</body>
</html>
