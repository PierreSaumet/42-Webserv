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
			//  && $_FILES["uploadfile"]["error"] == 0
			if(isset($_FILES["uploadfile"]))
			{
				$allowed = array("jpg" => "image/jpg", "jpeg" => "image/jpeg", "gif" => "image/gif", "png" => "image/png");
				$filename = $_FILES["uploadfile"]["name"];
				$filetype = $_FILES["uploadfile"]["type"];
				$filesize = $_FILES["uploadfile"]["size"];

				$ext = pathinfo($filename, PATHINFO_EXTENSION);
				if(!array_key_exists($ext, $allowed)) die("Error: format not good");
				
				if (is_dir("upload/"))
				{
					if (in_array($filetype, $allowed))
					{
						if(file_exists("upload/" . $_FILES["uploadfile"]["name"]))
						{	
							http_response_code();
							echo $_FILES["uploadfile"]["name"] . "Existe deja ";
						}
						else
						{
							if (move_uploaded_file($_FILES["uploadfile"]["tmp_name"], "upload/" . $_FILES["uploadfile"]["name"]) == true)
							{
								$path = "./upload/".$filename ;
								echo "Bingo <br/>";
                     			echo "<img src=".$path." width=50% />";
								
							}							
						}
					} 
				}
				else
				{
					echo "Folder doesn't exist";
				}
			}
			else
			{
			
				echo "Error file";
			}
		}
	?>
	<p><a href="/" index>retour index</a></p>
	
</body>
</html>