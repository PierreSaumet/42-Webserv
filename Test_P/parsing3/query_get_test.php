<html>
 <head>
  <title>GET Query String</title>
  <link rel="stylesheet" type="text/css" href="/static/style1.css">
 </head>
 <body>
 <body>
    <h1> Im'gonne say hello to you using the query string in the URL ! </h1>
   <h2>
 <?php
    echo 'Hello ' . htmlspecialchars($_GET["name"]) . '!';
?>
    </h2>
 </body>
</html>