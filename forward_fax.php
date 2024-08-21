<?php
/**
 * Copyright (C) 2019-2024 Paladin Business Solutions
 */

ob_start();
session_start();

require_once('includes/ringcentral-functions.inc');
require_once('includes/ringcentral-php-functions.inc');

show_errors();

page_header(0);

function show_form ($message, $label = "", $print_again = false) { ?>
    <table class="CustomTable">
        <tr class="CustomTable">
            <td colspan="2" class="CustomTableFullCol">
                <img src="images/rc-logo.png"/>
                <h2><?php echo_plain_text("Forward a Fax", "red", "large"); ?></h2>
                <?php
                if ($print_again == true) {
                    echo "<p class='msg_bad'>" . $message . "</strong></font>";
                } else {
                    echo "<p class='msg_good'>" . $message . "</p>";
                } ?>
            </td>
        </tr>
        <form action="" method="post" enctype="multipart/form-data">
            <tr class="CustomTable">
                <td class="center_col">
                    <p style='display: inline;'>Receiving Fax #:</p>
                    &nbsp;&nbsp;&nbsp;
                    <input type="text" name="to_fax_number">
                    &nbsp;&nbsp;&nbsp;
                    <input type="submit" class="submit_button" value="   Send forward   " name="forward_fax">
                    <br/>
                    <hr>
                </td>
            </tr>
            <?php $filename = view_fax($_GET['id']); ?>
            <tr class="CustomTable">
                <td class="CustomTableFullCol">
                    <div id="pdf-container">
                        <!-- Embed the PDF using iframe -->
                        <iframe src="<?php echo $filename; ?>" width="100%" height="100%" style="border: none;"></iframe>
                    </div>
                </td>
            </tr>
            <tr class="CustomTable">
                <td colspan="2" class="CustomTableFullCol">
                    <br/>
                    <input type="submit" class="submit_button" value="   Return to List   " name="list_faxes">
                    <input type="submit" class="submit_button" value="   Home   " name="home_page">
                </td>
            </tr>
            <tr class="CustomTable">
                <td colspan="2" class="CustomTableFullCol">
                    <hr>
                </td>
            </tr>
        </form>
    </table>
    <?php
}

/* ============= */
/*  --- MAIN --- */
/* ============= */
if (isset($_POST['home_page'])) {
    header("Location: index.php");
} elseif (isset($_POST['forward_fax'])) {
    if (send_forward_fax ($_POST['to_fax_number'], $_GET['id']) == false) {
        $message = "Fax forward failed";
    }  else {
        $message = "The selected Fax was forwarded successfully.";
    }
    show_form($message);
} elseif (isset($_POST['list_faxes'])) {
    header("Location: list_faxes.php");
} else {
    $message = "Showing your selected fax. Please provide forwarding fax number<br/><br/>";
    show_form($message);
}

ob_end_flush();
page_footer();
