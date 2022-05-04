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
	<h1> Test download de fichier </h1>

    <?php
        if(isset($_GET['path']))
        {
            //Read the filename
            $filename = $_GET['path'];
            //Check the file exists or not
            if(file_exists($filename)) {

                //Define header information
                header('Content-Description: File Transfer');
                header('Content-Type: application/octet-stream');
                header("Cache-Control: no-cache, must-revalidate");
                header("Expires: 0");
                header('Content-Disposition: attachment; filename="'.basename($filename).'"');
                header('Content-Length: ' . filesize($filename));
                header('Pragma: public');

                //Clear system output buffer
                flush();

                //Read the size of the file
                readfile($filename);

                //Terminate from the script
                die();
            }
            else{
                echo "File does not exist.";
            }
        }
        else{
            echo "Filename is not defined.";
        }

    ?>
</body>
</html>