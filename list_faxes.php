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
                <h2><?php echo_spaces("list Faxes"); ?></h2>
                <?php
                if ($print_again == true) {
                    echo "<p class='msg_bad'>" . $message . "</strong></font>";
                } else {
                    echo "<p class='msg_good'>" . $message . "</p>";
                } ?>
                <hr>
            </td>
        </tr>
        <?php $faxes = get_faxes();
        foreach ($faxes as $fax) { ?>
        <tr class="CustomTable">
            <td class="left_col">
                <?php foreach ($fax as $key => $value) {
                    if ($key == "Id") { ?>
                        <!--                        echo_spaces("$key", $value, 1);-->
                        <a class="links" href="view_fax.php?id=<?= $value ?>">View</a>
                        &nbsp;&nbsp;
                        <a class="links" href="forward_fax.php?id=<?= $value ?>">Forward</a>
                        &nbsp;&nbsp;
                    <?php }
                    echo "<strong>" . $key . ": </strong>" ;
                    echo $value . " ";
                } ?>
            </td>
        </tr>
        <?php } ?>
        <tr class="CustomTable">
            <td class="CustomTableFullCol">
                <hr>
                <br/>
                <form action="" method="post" enctype="multipart/form-data">
                    <input type="submit" class="submit_button" value="   Home   " name="home_page">
                </form>
            </td>
        </tr>
    </table>
    <?php
}

/* ============= */
/*  --- MAIN --- */
/* ============= */
if (isset($_POST['home_page'])) {
    header("Location: index.php");
} else {
    $message = "Here are your current faxes. <br/><br/>";
    show_form($message);
}

ob_end_flush();
page_footer();
