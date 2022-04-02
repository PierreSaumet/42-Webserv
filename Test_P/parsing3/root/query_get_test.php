<html>
 <head>
  <title>GET Query String</title>
  <style>body { background-color: rgb(105, 140, 238) }
			h1 { font-size:2cm; text-align: center; color: rgb(121, 219, 56);
			text-shadow: 0 0 2mm red}
		</style>
 </head>


 <body>
    <h1> Im'gonne say hello to you using the query string in the URL ! </h1>
   <h2>
 <?php
    echo 'Hello ' . htmlspecialchars($_GET["user_name"]) . '!';
?>
    </h2>
 </body>
</html>