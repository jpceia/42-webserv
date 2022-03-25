<!DOCTYPE html>
<html>
<body>

<h1>My first PHP page</h1>

<?php

echo "Hello World!\n";
echo 'Hello ' . htmlspecialchars($_GET["hello"]) . '!';


//Call getenv() function without argument

$env_array =getenv();

echo "<h3>The list of environment variables with values are :</h3>";

//Print all environment variable names with values

foreach ($env_array as $key=>$value)

{
    echo "$key => $value <br />";
}

// Get variables
echo "<h3>GET variables:</h3>";
echo "<pre>";
print_r($_GET);
echo "</pre>";
echo "<br />";

// Post variables
echo "<h3>POST variables:</h3>";
echo "<pre>";
print_r($_POST);
echo "</pre>";
echo "<br />";

// Cookies
echo "<h3>Cookies:</h3>";
echo "<pre>";
print_r($_COOKIE);
echo "</pre>";
echo "<br />";

// Session
echo "<h3>Session:</h3>";
echo "<pre>";
print_r($_SESSION);
echo "</pre>";




?>

</body>
</html>
