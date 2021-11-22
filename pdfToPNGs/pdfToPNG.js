<script type="text/javascript">
    function onConvertClick_PDF()
    {
        var files = document.getElementById("fileToUpload_PDF").files;
        console.log( files );
        if( files == undefined || files == "" || files.length <= 0 )
        {
            alert("please choose file");
            return false;
        };
        console.log( files[0].name );


        var form = new FormData();
        form.append("fileToUpload_PDF", files[0]);

        var xmlRequset = new XMLHttpRequest();
        var url = "/functions/pdf-to-pngs/pdfToPNGs.php";
        xmlRequset.open("post", url, true); // true - asynchronous processing

        xmlRequset.upload.onprogress = progressFunction_PDF;
        xmlRequset.onload = uploadComplete_PDF;
        xmlRequset.onerror = uploadFailed_PDF;
        xmlRequset.send(form);
    }

    function ShowWaiting_PDF() {
        var waitItem = document.getElementById('waiting_PDF');
        waitItem.style.display = "block";
    }

    function HidWaiting_PDF() {
        var waitItem = document.getElementById('waiting_PDF');
        waitItem.style.display = "none";
    }

    function ShowResult_PDF() {
        var resultItem = document.getElementById('result_PDF');
        resultItem.style.display = "block";
    }

    function HidResult_PDF() {
        var resultItem = document.getElementById('result_PDF');
        resultItem.style.display = "none";
    }

    function ShowResultLog_PDF( text ){
        if( text.substr( 0, 7 ) == "[error]" )
        {
            alert( text );
            return ;
        }
        var fileName = text.substr( text.lastIndexOf( '/' )+1, text.length );
        fileName = fileName.substr( 0, fileName.lastIndexOf( '?' ) );
        var url = "/functions/pdf-to-pngs/" + text;
        var resultA = document.getElementById('resultA_PDF');
        resultA.href = url;
        resultA.download = fileName;
        resultA.innerHTML = "Click here to download: " + fileName;
        ShowResult_PDF();
    }

    function uploadFailed_PDF(evt) {
        alert("failed to upload file!");
        HidWaiting_PDF();
        HidResult_PDF();
    }

    function uploadComplete_PDF(evt) {
        var text = evt.target.responseText;
        console.log( text );
        if( text )
        {
            //alert("success: " + text);
        }
        else
        {
            alert("uploadComplete_PDF - failed to upload file: "+text);
        }
        HidWaiting_PDF();
        ShowResultLog_PDF( text );
    }

    function progressFunction_PDF(event)
    {
        // event.total是需要传输的总字节，event.loaded是已经传输的字节。如果event.lengthComputable不为真，则event.total等于0
        var value = Math.round(event.loaded / event.total * 100);
        console.log( value );
        document.getElementById("progress_PDF").value = value;
        if( value > 99 )
        {
            ShowWaiting_PDF();
        }
    }
</script>