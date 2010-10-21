<?php

function test_yajl_null($ctx) {
    print "test_yajl_null called with context $ctx\n";
}

$callbacks = array(
    test_yajl_null,
    /* test_yajl_boolean */     null,
    /* test_yajl_integer */     null,
    /* test_yajl_double */      null,
                                null,
    /* test_yajl_string */      null,
    /* test_yajl_start_map */   null,
    /* test_yajl_map_key */     null,
    /* test_yajl_end_map */     null,
    /* test_yajl_start_array */ null,
    /* test_yajl_end_array */   null
);

/* function yajl_alloc($a, $b, $c, $d) {
    return 1;
}
*/

$hYajl = yajl_alloc($callbacks, array(0,0), null, null);
print "hYajl = ".$hYajl;

if($hYajl) {
    yajl_free($hYajl);
}
?>

