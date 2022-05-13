<?php
    if($_SERVER["REQUEST_METHOD"] == "POST")
    {
        $entity = file_get_contents('php://input');
        echo "DATA = " . $entity;

    }
    else
    {
        echo "Error file";
    }
?>