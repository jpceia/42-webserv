<?php
    if ($_SERVER['REQUEST_METHOD'] == 'POST') {
        // set full name variable
        $flavour = htmlspecialchars($_POST["flavour"]);
        setcookie("flavour", $flavour, time() + (86400 * 30), "/");
    }
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=<, initial-scale=1.0">
    <title>Set Cookie</title>
</head>
<body>
    <!-- Create a form to get the first name and last name
        if it is a GET request,
        otherwise, if it is a POST request,
        display the first name and last name -->
    <?php
        // If it is a GET request,
        // display the first name and last name
        if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    ?>
    <form action="set_cookie.php" method="post">
        <p>Please enter your favorite cookie flavour:</p>
        <input type="text" name="flavour" />
        <br />
        <input type="submit" value="Submit">
    </form>
    <?php
        }
        else if ($_SERVER['REQUEST_METHOD'] == 'POST') {
            echo "Your favorite cookie flavour " . $flavour . " was set up!";
        }
    ?>
</body>
</html>
