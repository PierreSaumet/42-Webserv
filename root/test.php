<html>
	<head>
		<title>upload</title>
		<style>
			body { background-color: rgb(105, 140, 238) }
			h1 { font-size:2cm; text-align: center; color: rgb(121, 219, 56);
			text-shadow: 0 0 2mm red}
		</style>
		<link rel="icon" type="image/x-con" href="/flavicon.ico"/><link rel="shortcut icon" type="image/x-con" href="/flavicon.ico" />
</head>

<body>
	<h1> Test afficher du texte e </h1>

    <?php
    	if($_SERVER["REQUEST_METHOD"] == "POST"){

            $entity = file_get_contents('php://input');
            echo "TEST = " . $entity;
        } else{
            echo "Error";
        }
        ?>
		
</body>
</html>

<!-- curl -H "Transfer-Encoding: chunked " -d "bonjour je suis du texte " 127.0.0.1:4343/test.php -->

<!-- curl -d "bonjour je suis du texte " 127.0.0.1:4343/test.php -->
