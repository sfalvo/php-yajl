<?php

function test_yajl_cb($ctx, $type, $arg) {
    print "CTX: $ctx    TYPE: $type    ARG: $arg\n";
}

$hYajl = yajl_new(0, 0, "SMACKDOWN", test_yajl_cb);
print "hYajl = ".$hYajl."\n";

$T = "{ \"nl\": null, \"bl\": false,                \"db\": 123.45, \"str\": \"DEADBEEF\", \"ar\": [1, 2, \"three\"] }";
$r = 0;
$r = yajl_parse($hYajl, $T);
print "r = ".$r."\n";

if($hYajl) {
    yajl_dispose($hYajl);
}
?>

