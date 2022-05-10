<!DOCTYPE html>

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


	<form enctype="multipart/form-data" action="upload.php" method="POST">
			<p>Il faut choisir un fichier a uploader: <input name="uploadfile" type="file" /><br />
			<input type="submit" value="upload_file" />
			</p>
	</form>


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
				
				if (is_dir("upload/")) {
					// le type mime 
					if (in_array($filetype, $allowed)){	//verifie que filetype est dans allowed
						
						if(file_exists("upload/" . $_FILES["uploadfile"]["name"])){	// verifie que upload/ + le nom du fichier existe
							
							header("Status: 200 OK\r\n");
							echo $_FILES["uploadfile"]["name"] . " existe deja ";	// il existe deja
							
					
						} else{
							move_uploaded_file($_FILES["uploadfile"]["tmp_name"], "upload/" . $_FILES["uploadfile"]["name"]); // verifiw aue le tmp file est un fichier telecharge et le deplace
							
							header("Status: 201 Created\r\n");
							echo "yes ca marche\n";
							// echo "Content-Length: " . $filesize;
							
						}
					} else{
						echo " marche pas";
					}
				} else{
					echo "dossier existe pas";
				}

			} else{
				echo "Error method ";
			}
		} else{
			// echo "Method not good kek bur";
		}
	?>

	
</body>
</html>