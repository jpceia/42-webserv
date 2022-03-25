<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=<, initial-scale=1.0">
    <title>Set Session</title>
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
    <form action="set_session.php" method="post">
        <p>Please enter your name:</p>
        <input type="text" name="first_name" placeholder="First Name">
        <br />
        <input type="text" name="last_name" placeholder="Last Name">
        <br />
        <input type="submit" value="Submit">
    </form>
    <?php
        }
        else if ($_SERVER['REQUEST_METHOD'] == 'POST') {
            // set full name variable
            $full_name = htmlspecialchars($_POST["first_name"]) . ' ' . htmlspecialchars($_POST["last_name"]);
            echo "Hello " . $full_name . "!";
            $_SESSION["name"] = $full_name;
        }
    ?>
</body>
</html>
