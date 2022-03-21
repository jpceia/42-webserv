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

echo "<br />";
echo $_GET;
echo "<br />";
foreach ($_GET as $key=>$value)

{
    echo "$key => $value <br />";
}
echo "<br />";
echo $_POST;
echo "<br />";
foreach ($_POST as $key=>$value)

{
    echo "$key => $value <br />";
}
echo "<br />";


?>

</body>
</html>
