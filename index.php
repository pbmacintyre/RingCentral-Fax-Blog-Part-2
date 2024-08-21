<?php
/**
 * Copyright (C) 2019-2024 Paladin Business Solutions
 */
ob_start() ;
session_start();

require_once('includes/ringcentral-functions.inc');
require_once('includes/ringcentral-php-functions.inc');

show_errors();

page_header(0);

function show_form ($message, $label = "", $print_again = false) {

    ?>
    <form action="" method="post" enctype="multipart/form-data">
        <table class="EditTable" >
            <tr class="CustomTable">
                <td colspan="2" class="CustomTableFullCol">
                    <img src="images/rc-logo.png"/>
                    <h2><?php echo app_name(); ?></h2>
                    <?php
                    if ($print_again == true) {
                        echo "<p class='msg_bad'>" . $message . "</strong></font>";
                    } else {
                        echo "<p class='msg_good'>" . $message . "</p>";
                    } ?>
                    <hr>
                </td>
            </tr>
            <tr class="CustomTable">
                <td class="left_col">
                    <p style='display: inline; <?php if ($label == "to_fax_number") echo "color:red"; ?>'>Receiving Fax #:</p>
                </td>
                <td class="right_col">
                    <input type="text" name="to_fax_number" >
                </td>
            </tr>
            <tr class="CustomTable">
                <td class="left_col">
                    <p style='display: inline; <?php if ($label == "cover_note") echo "color:red"; ?>'>Fax Cover Note:</p>
                </td>
                <td class="right_col">
                    <input type="text" name="cover_note" >
                </td>
            </tr>
            <tr class="CustomTable">
                <td class="left_col">
                    <p style='display: inline; <?php if ($label == "file_to_fax") echo "color:red"; ?>'>Upload file to Fax:</p>
                </td>
                <td class="right_col">
                    <input type="file" name="file_to_fax" id="file_to_fax">
                </td>
            </tr>
            <tr class="CustomTable">
                <td class="CustomTableFullCol">
                    <br/>
                    <input type="submit" class="submit_button" value="   List Faxes   " name="list_faxes">
                </td>
                <td class="CustomTableFullCol">
                    <br/>
                    <input type="submit" class="submit_button" value="   Send Fax   " name="send_fax">
                </td>
            </tr>
            <tr class="CustomTable">
                <td colspan="2" class="CustomTableFullCol"><hr></td>
            </tr>
        </table>
    </form>
    <?php
}

function check_form () {
    show_errors();

    $print_again = false;
    $label = "";
    $message = "";

    /* ============================================ */
    /* ====== START data integrity checks ========= */
    /* ============================================ */

    $to_fax_number = strip_tags($_POST['to_fax_number']);
    $cover_note = strip_tags($_POST['cover_note']);
    $target_file = basename($_FILES["file_to_fax"]["name"]);

    if ($target_file == "") {
        $print_again = true;
        $label = "";
        $message = "No file selected to be uploaded";
    }
    if ($cover_note == "") {
        $print_again = true;
        $label = "cover_note";
        $message = "No cover note has been provided";
    }
    if ($to_fax_number == "") {
        $print_again = true;
        $label = "to_fax_number";
        $message = "No receiving Fax Number has been provided";
    }

    /* ========================================== */
    /* ====== END data integrity checks ========= */
    /* ========================================== */

    $file_with_path = upload_file();

    $fax_sent_id = send_fax($to_fax_number, $file_with_path, $cover_note) ;
    if ($fax_sent_id > 0) {
        $print_again = true;
        $label = "";
        $message = "Fax sent successfully (Sent id): " . $fax_sent_id ;
        // clean out the file
        unlink($file_with_path) ;
    }
    show_form($message, $label, $print_again);
}

/* ============= */
/*  --- MAIN --- */
/* ============= */
if (isset($_POST['send_fax'])) {
    check_form();
} elseif (isset($_POST['list_faxes'])) {
    header("Location: list_faxes.php");
} else {
    $message = "Please provide information to be faxed. <br/><br/>";
    show_form($message);
}

ob_end_flush();
page_footer();
