<html>
<body>

<?php

$dbname = 'example';
$dbuser = 'root';  
$dbpass = ''; 
$dbhost = 'localhost'; 

$connect = @mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);

if(!$connect){
	echo "Error: " . mysqli_connect_error();
	exit();
}

echo "Connection Success!<br><br>";

$ph = $_GET["ph"];
$nrPomiaru = $_GET["nrPomiaru"]; 


$query = "INSERT INTO iot_project (ph, nrPomiaru) VALUES ('$ph', '$nrPomiaru')";
$result = mysqli_query($connect,$query);

echo "Insertion Success!<br>";

?>
</body>
</html>