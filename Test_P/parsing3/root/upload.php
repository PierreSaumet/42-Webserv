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
				// path info retourne info sur le path
				$ext = pathinfo($filename, PATHINFO_EXTENSION);
				if(!array_key_exists($ext, $allowed)) die("Error: format not good");	// verifique dans le ext il y a allowed


				// taille du fichier donc useless // a suppriner et a verifier soit meme
				// $maxsize = 2 * 1024 * 1024; // 5mo
				// if($filesize > $maxsize) exit("Error: fichir trop grad: ($filesize)");

				
				if (is_dir("upload/")) {
					// le type mime 
					if (in_array($filetype, $allowed)){	//verifie que filetype est dans allowed
						// verifie sil existe
						if(file_exists("upload/" . $_FILES["uploadfile"]["name"])){	// verifie que upload/ + le nom du fichier existe
							echo $_FILES["uploadfile"]["name"] . " existe deja ";	// il existe deja
							header("Status: 200 OK\r\n");
						} else{
							move_uploaded_file($_FILES["uploadfile"]["tmp_name"], "upload/" . $_FILES["uploadfile"]["name"]); // verifiw aue le tmp file est un fichier telecharge et le deplace
							echo "yes ca marche\n";
							header("Status: 201 Created\r\n");
						}
					} else{
						echo " marche pas";
					}
				} else{
					echo "dossier existe pas";
				}

			} else{
				echo "Error " . $_FILES["uploadfile"]["error"] . " trop grand ?: " . $_FILES["uploadfile"]["size"];
			}
		} else{
			echo "Method not good kek bur";
		}
	?>

	<p-> Maintenant cliquer ici pour le voir :   <a href="download/">ICI</a></p->
</body>
</html>