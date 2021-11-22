<?php
// we save uploaded PDF, splited images and generated zip file in
// the folder ./uploads

$Debug = false;
if( $Debug )
{
    ini_set('display_errors',1);
    error_reporting(E_ALL);
}

$target_dir = "uploads/";
$target_basename = basename($_FILES["fileToUpload_PDF"]["name"]);
$target_basename = str_replace( " ", "_", $target_basename ); //remove blank
$target_file = $target_dir . $target_basename;
$uploadOk = 1;
$fileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));

function CheckFolder( $fileName )
{
    $cmdStr = __DIR__ . '/pre_process.py' . ' ' . $fileName;
    $handle = popen( $cmdStr, 'r' );
    if( false == $handle )
    {
        echo "[error] failed: " . $cmdStr . "<br>";
    }
    else
    {
        pclose( $handle );
    }
}

function HandleImage( $fileName, $zipPath )
{
    $cmdStr = __DIR__ . '/pdfToPNGs.py' . ' ' . $fileName . ' ' . $zipPath;
    $handle = popen( $cmdStr, 'r' );
    if( false == $handle )
    {
        echo "[error] failed: " . $cmdStr . "<br>";
    }
    else
    {
        pclose( $handle );
    }
}

function GetZipName( $imagePath )
{
    $path_parts = pathinfo( $imagePath );
    $zipPath = $path_parts['dirname'] . "/" . $path_parts['filename'];
    $zipPath = $zipPath . ".zip";
    return $zipPath;
}

CheckFolder( $target_file );

// check uploads folder
if( !file_exists( $target_dir ) )
{
    if( mkdir( $target_dir ) )
    {
        if( $Debug )
        {
            echo "Create uploads folder successfully.<br>";
        }
    }
    else
    {
        echo "[error] failed to create folder uploads.<br>";
    }
}

// Check if image file is a actual image or fake image
if(isset($_POST["submit"]))
{
    if( $Debug )
    {
        echo "tmp file path: " . $_FILES["fileToUpload_PDF"]["tmp_name"] . "<br>";
    }

    $check = filesize($_FILES["fileToUpload_PDF"]["tmp_name"]);
    if($check != false && $check != 0)
    {
        if( $Debug ) echo "File is an PDF.<br>";
        $uploadOk = 1;
    }
    else
    {
        echo "[error] File is not an valid PDF.<br>";
        $uploadOk = 0;
        exit( -1 );
    }
}

// Check if file already exists
if (file_exists($target_file)) {
    // try to delete the same image
    if( false == unlink( $target_file ) )
    {
        echo "[error] Sorry, file already exists." . $target_file . "<br>";
        $uploadOk = 0;
        exit( -1 );
    }
}


// Check file size
if ($_FILES["fileToUpload_PDF"]["size"] > 1024 * 1024 * 80)
{
    echo "[error] Sorry, your file is too large, bigger than 80M<br>";
    $uploadOk = 0;
    exit( -1 );
}


// Allow certain file formats
if($fileType != "pdf")
{
    echo "[error] Sorry, only PDF files are allowed.<br>";
    $uploadOk = 0;
    exit( -1 );
}


// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0)
{
    echo "[error] Sorry, your file was not uploaded.<br>";
    exit( -1 );
// if everything is ok, try to upload file
}
else
{
    if (move_uploaded_file($_FILES["fileToUpload_PDF"]["tmp_name"], $target_file))
    {
        if( $Debug ) echo "The file ". $target_basename . " has been uploaded.<br>";

        $zipPath = GetZipName( $target_file );
        HandleImage( $target_file, $zipPath );
        if( file_exists( $zipPath ) )
        {
            $date = new DateTime();
            $timestamp = $date->getTimestamp();
            $result = $filePath. "?" . $timestamp;
            echo $result;
        }
        else
        {
            echo "[error] Sorry, here is not image zip generated:" . $zipPath . "<br>";
        }
    }
    else
    {
        echo "[error] Sorry, there was an error uploading your file: <br>";
        echo "  ".$_FILES["fileToUpload_PDF"]["tmp_name"]."  =>  ".$target_file . "<br>";
    }
}

?>
