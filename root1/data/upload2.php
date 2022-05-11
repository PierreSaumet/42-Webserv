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

      <h1>Uploading images using PHP-CGI</h1>


      <div style="display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100%;">

         <form action="upload2.php" method="POST" enctype="multipart/form-data">
            <p>Choose a file   <input type="file" name="image" /></p>
            
            <p><input type="submit"/></p>
            </form>


         <?php
         if ($_SERVER["REQUEST_METHOD"] == "POST")
         {
            if(isset($_FILES['image']))
            {
               $errors= array();
               $file_name = $_FILES['image']['name'];
               $file_size =$_FILES['image']['size'];
               $file_tmp = $_FILES['image']['tmp_name'];
               $file_type= $_FILES['image']['type'];
               $tmp = $_FILES['image']['name'];
               $tmp = explode('.',$tmp);
               $file_ext = strtolower(end($tmp));
               
               $extensions= array("jpeg","jpg","png");
               
               if(in_array($file_ext,$extensions)=== false)
               {
                  $errors[]="extension not allowed, please choose a JPEG or PNG file.";
               }
               
               if(empty($errors)==true)
               {
                  $path = "./upload/".$file_name ;

                  // echo "Path is ".$path ;
                  if (move_uploaded_file($file_tmp,$path) == true) 
                  {
                     // header("Location: " . $path);
                     // exit();
                     // header("Cache-Control: no-cache");
                     // header("Status: 201 Created");
                     echo "Bingo <br/>";
                     echo "<img src=".$path." width=50% />";
                  }
                  else
                  {

                  }
                  // echo "<img src=".$path." width=50% />";
               }
               else
               {
                  print_r($errors);
                  // echo "<img src=".$path." width=50% />";
               }
            }
            else
            {
               // echo "No file has been uploaded";
            }
         } 
         else {
            // echo "Error in the method";
         }
      ?>



      </div>
   </body>
</html>