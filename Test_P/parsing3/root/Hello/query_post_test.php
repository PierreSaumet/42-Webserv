<html>
 <head>
  <title>Form with POST</title>
  <style>body { background-color: rgb(105, 140, 238) }
			h1 { font-size:2cm; text-align: center; color: rgb(121, 219, 56);
			text-shadow: 0 0 2mm red}
		</style>
		<link rel="icon" type="image/x-con" href="/flavicon.ico"/><link rel="shortcut icon" type="image/x-con" href="/flavicon.ico" />
</head>

 <body>
    <h1> On a bien recu les donnees =): </h1>
   <h2>
 <?php
	echo 'Tu t appelles : '.htmlspecialchars($_POST["name"]);
	echo ' et ton texte est -'.htmlspecialchars($_POST["message"]);
	echo '-';
?>
    </h2>
 </body>
</html>