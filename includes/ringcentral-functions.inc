<?php
/** Copyright (C) 2019-2024 Paladin Business Solutions */

/* ================= */
/* Generic functions */
/* ================= */

function app_name () {
    return "FAX API Sampler App";
}

/* ================== */
/* Get RingCental SDK */
/* ================== */
function ringcentral_sdk () {
    // Include Libraries
    require('includes/vendor/autoload.php');

    // JWT Name: "JWT for Basic FAX API Tester"
    $jwt_key = "eyJraWQiOiI4NMiOg9JnmVlLwCT6ChLto5VE_MlR9Fh7tkXt4JXoDu8U5h8gIWr5AShDFw";

    // Use Production platform
    // App Name: "Basic FAX API Tester"
    $server = 'https://platform.ringcentral.com';
    $client_id = "ARH5Mjub";
    $client_secret = "5IcVTwfREy9fME31fw9U5T";
    $from_account_phone_number = "+15054" ;

    $sdk = new RingCentral\SDK\SDK($client_id, $client_secret, $server);

    // Login via API
    if (!$sdk->platform()->loggedIn()) {
        try {
            $sdk->platform()->login(["jwt" => $jwt_key]);
        }
        catch (\RingCentral\SDK\Http\ApiException $e) {
            $sdk = 0;
            // exit("<br/><br/>Unable to authenticate to platform. Check your RingCentral credentials. <br/><br/>") ;
        }
    }
    return $sdk;
}

function send_fax($to_fax_number, $file_with_path, $cover_note){
    $rcsdk = ringcentral_sdk ();
    $from_account_phone_number = "+1504" ;
    try {
        $bodyParams = $rcsdk->createMultipartBuilder()
            ->setBody(array(
                'to' => array(array('phoneNumber' => $to_fax_number)),
                'faxResolution' => "High",
                'coverPageText' => $cover_note,
            ))
            ->add(fopen($file_with_path, 'r'))
            ->request('/restapi/v1.0/account/~/extension/~/fax');

        $resp = $rcsdk->platform()->sendRequest($bodyParams);
        $message_id = $resp->json()->id;

    }catch (\RingCentral\SDK\Http\ApiException $e) {
        // Getting error messages using PHP native interface
        print 'Expected HTTP Error: ' . $e;
        print '  Message: ' . $e->apiResponse->response()->error() . PHP_EOL;
    }
    return $message_id ;
}
