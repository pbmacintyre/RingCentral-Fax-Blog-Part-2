<?php
/**
 * Copyright (C) 2019-2024 Paladin Business Solutions
 *
 */

/* ========================== */
/* Call HTML wrappers section */
/* ========================== */
function page_header() {
    echo "<html class='SiteWide'><head >";
    echo "<title >" . app_name() . "</title></head>";
    echo "<link rel='stylesheet' href='css/ringcentral-custom.css'>";
}
function page_footer() {
    echo "</html>";
}
/* =================== */
/* Turn on Error codes */
/* =================== */
function show_errors() {
     error_reporting();
//     error_reporting(E_ALL & ~E_NOTICE & ~E_WARNING);
    ini_set('display_errors', 1);
}
function echo_spaces($text, $value="", $lines=0) {
    echo "<br /><strong><p style='color: red; display: inline'>$text:</p></strong> " ;
    if (is_string($value) || is_int($value))  { echo $value; }
    if (is_object($value) || is_array($value)) { ringcentral_dump_obj($value); }
    if (is_bool($value)) { if ($value) echo "TRUE / Success"; else echo 'FALSE / Failure'; }
    $i = 1 ;
    while ($i <= $lines) {
        echo "<br />" ;
        $i++;
    }
}
/* ======================== */
/* Dump the provided object */
/* ======================== */
function ringcentral_dump_obj($object) {
    echo "<pre>";
    var_dump($object);
    echo "</pre>";
}
function required_field() {
    echo "<p style='color: red; display: inline'>*</p>" ;
}

function upload_file () {

    if ($_SERVER['REQUEST_METHOD'] == 'POST') {
        // Directory where the file will be uploaded
        $target_dir = "uploads/";

        // Path to the target file
        $target_file = $target_dir . basename($_FILES["file_to_fax"]["name"]);

        // Variable to keep track of upload status
        $uploadOk = 1;

        // Get file extension
        $fileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));

        // Check file size (optional, e.g., limit to 5MB)
        if ($_FILES["file_to_fax"]["size"] > 5000000) {
            echo "Sorry, your file is too large.";
            $uploadOk = 0;
        }

        // Allow certain file formats (optional)
        // image files and PDFs
        $allowedTypes = array("jpg", "png", "jpeg", "pdf");
        if (!in_array($fileType, $allowedTypes)) {
            echo "Sorry, only JPG, PNG & PDF files are allowed.";
            $uploadOk = 0;
        }

        // Check if $uploadOk is set to 0 by an error
        if ($uploadOk == 0) {
            echo "Sorry, your file was not uploaded.";
            // If everything is ok, try to upload file
        } else {
            if (move_uploaded_file($_FILES["file_to_fax"]["tmp_name"], $target_file)) {
                $filename = htmlspecialchars(basename($_FILES["file_to_fax"]["name"])) ;
            } else {
                echo "Sorry, there was an error uploading your file.";
            }
        }
    }
    return $target_file ;

}
