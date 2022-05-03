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
	<h1> Test upload de fichier image </h1>

	<?php
    	if($_SERVER["REQUEST_METHOD"] == "POST"){
			
			if(isset($_FILES["uploadfile"]) && $_FILES["uploadfile"]["error"] == 0){
				$allowed = array("jpg" => "image/jpg", "jpeg" => "image/jpeg", "gif" => "image/gif", "png" => "image/png");
				$filename = $_FILES["uploadfile"]["name"];
				$filetype = $_FILES["uploadfile"]["type"];
				$filesize = $_FILES["uploadfile"]["size"];
				// $errors = array()

				// check extansin
				$ext = pathinfo($filename, PATHINFO_EXTENSION);
				if(!array_key_exists($ext, $allowed)) die("Error: format not good");


				// taille du fichier donc useless
				$maxsize = 5 * 1024 * 1024; // 5mo
				if($filesize > $maxsize) die("Error: fichir trop grad");

				// le type mime 
				if (in_array($filetype, $allowed)){
					// verifie sil existe
					if(file_exists("upload/" . $_FILES["uploadfile"]["name"])){
						echo $_FILES["uploadfile"]["name"] . " existe deja ";
					} else{
						move_uploaded_file($_FILES["uploadfile"]["tmp_name"], "upload/" . $_FILES["uploadfile"]["name"]);
						echo "yes ca marche";
					}
				} else{
					echo " marche pas";
				}

			} else{
				echo "Error " . $_FILES["uploadfile"]["error"];
			}
		}
	?>

</body>
</html>