<?php
/** Copyright (C) 2019-2024 Paladin Business Solutions */

/* ================= */
/* Generic functions */
/* ================= */

function app_name () {
    return "FAX API Sample App - Part 2";
}

/* ================== */
/* Get RingCental SDK */
/* ================== */
function ringcentral_sdk () {
    // Include Libraries
    require('includes/vendor/autoload.php');

    $dotenv = Dotenv\Dotenv::createImmutable(__DIR__);
    $dotenv->load();

    $jwt_key = $_ENV['RC_USER_JWT'] ;

    $sdk = new RingCentral\SDK\SDK(
        $_ENV['RC_APP_CLIENT_ID'],
        $_ENV['RC_APP_CLIENT_SECRET'],
        $_ENV['RC_SERVER_URL']);

    $platform = $sdk->platform();

    // Login via API
    if (!$sdk->platform()->loggedIn()) {
        try {
            $platform->login(["jwt" => $jwt_key]);
        }
        catch (\RingCentral\SDK\Http\ApiException $e) {
            $sdk = 0;
            // exit("<br/><br/>Unable to authenticate to platform. Check your RingCentral credentials. <br/><br/>") ;
        }
    }
    $controller = array('SDK' => $sdk, 'platform' => $platform);
    return $controller;
}

function send_fax ($to_fax_number, $file_with_path, $cover_note) {
    $controller = ringcentral_sdk();
    $platform = $controller['platform'];
    $sdk = $controller['SDK'];

    try {
        $bodyParams = $sdk->createMultipartBuilder()
            ->setBody(array(
                'to' => array(array('phoneNumber' => $to_fax_number)),
                'faxResolution' => "High",
                'coverPageText' => $cover_note,
            ))
            ->add(fopen($file_with_path, 'r'))
            ->request('/restapi/v1.0/account/~/extension/~/fax');

        $resp = $platform->sendRequest($bodyParams);
        $message_id = $resp->json()->id;

    }
    catch (\RingCentral\SDK\Http\ApiException $e) {
        // Getting error messages using PHP native interface
        print 'Expected HTTP Error: ' . $e;
        print '  Message: ' . $e->apiResponse->response()->error() . PHP_EOL;
    }
    return $message_id;
}

function get_faxes () {
    $controller = ringcentral_sdk();
    $platform = $controller['platform'];

    $dateTime = new DateTime('now', new DateTimeZone('AST'));
    $fromDate = $startDateTime = $dateTime->modify('-4 days')->format('Y-m-d\TH:i:s.v\Z');

    $queryParams = array(
        'availability' => array('Alive',),
        //'conversationId' => '<ENTER VALUE>',
        'dateFrom' => $fromDate,
        // format: 2016-03-10T18:07:52.534Z
        //'dateTo' => '<ENTER VALUE>',
        'direction' => array('Inbound'),
        //'distinctConversations' => true,
        'messageType' => array('Fax',),
        //'readStatus' => array(  ),
        //'page' => 000,
        //'perPage' => 000,
        //'phoneNumber' => '<ENTER VALUE>'
    );
    $fax_list = array();
    $i = 1;
    try {
        $endpoint = "/restapi/v1.0/account/~/extension/~/message-store";
        $resp = $platform->get($endpoint, $queryParams);
//        echo_spaces("fax object", $resp->json());
        foreach ($resp->json()->records as $value) {
//            echo_spaces("Fax Id", $value->id);
            $fax_list[$i]["Id"] = $value->id;
//            echo_spaces("Fax Created", date('F j, Y, g:i a', strtotime($value->creationTime)));
            $fax_list[$i]["Created"] = date('F j, Y, g:i a', strtotime($value->creationTime));
//            echo_spaces("Fax Page Count", $value->faxPageCount);
            $fax_list[$i]["Pages"] = $value->faxPageCount;
//            echo_spaces("From #", $value->from->phoneNumber);
            $fax_list[$i]["From"] = $value->from->phoneNumber;
//            $fax_list[$i]["Sent to"] = $value->subject;
//            echo_spaces("From Whom", $value->from->name);
            $fax_list[$i]["Sender"] = $value->from->name;
            $i++;
        }
//        echo_spaces("Object", $resp->json());

    }
    catch (\RingCentral\SDK\Http\ApiException $e) {
        // Getting error messages using PHP native interface
        $fax_list[$i]["error"] = $e->getMessage();
    }
    return $fax_list;
}

function view_fax ($faxId) {
    $controller = ringcentral_sdk();
    $platform = $controller['platform'];

    $resp = $platform->get("/account/~/extension/~/message-store/$faxId");
    $object = $resp->json();

    if (isset($object->attachments)) {
        $fileName = $object->id . "_fax.";
        $fileName .= substr($object->attachments[0]->contentType, -3);

        try {
            $res = $platform->get($object->attachments[0]->uri);
            file_put_contents($fileName, $res->raw());
        }
        catch (\RingCentral\SDK\Http\ApiException $e) {
            echo_spaces("Object error", $e->getMessage());
        }
    }
    return $fileName;
}

function send_forward_fax ($to_fax_number, $faxid) {
    $controller = ringcentral_sdk();
    $platform = $controller['platform'];
    $sdk = $controller['SDK'];

    $filename = view_fax($faxid) ;

    try {
        $bodyParams = $sdk->createMultipartBuilder()
            ->setBody(array(
                'to' => array(array('phoneNumber' => $to_fax_number)),
                'faxResolution' => "High",
//                'coverIndex' => 0,
                'coverPageText' => "This is a forwarded fax",
            ))
            ->add(fopen($filename, 'r'))
            ->request('/restapi/v1.0/account/~/extension/~/fax');

        $response = $platform->sendRequest($bodyParams);

    } catch (\RingCentral\SDK\Http\ApiException $e) {
        $response = false ;
    }
    return $response;
}