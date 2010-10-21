<?php

function test_yajl_null($ctx) {
    print "test_yajl_null called with context $ctx\n";
}

$hYajl = yajl_new(0, 0, "SMACKDOWN", test_yajl_null);
print "hYajl = ".$hYajl."\n";

$T = "{ \"foo\": null }";
$r = 0;
$r = yajl_parse($hYajl, $T);
print "r = ".$r."\n";

if($hYajl) {
    yajl_dispose($hYajl);
}
?>

