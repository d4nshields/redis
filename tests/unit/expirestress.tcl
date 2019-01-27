package require sha1;

proc rand_range {min max} { 
    return [expr int(rand()*($max-$min+1)) + $min]
}

proc encrypt {msg} { 
    return [sha1::sha1 $msg];
}

proc make_key {index} {
    set a $index
    append a ".";
    append a [clock milliseconds];
    append a ".";
    append a [rand_range 0 65536];
    return $a;
}

start_server {tags {"expirestress"}} {
    set blocks 10240
    set maxtime 300
    set pause_ms 10
    set expire_s 2
    test {EXPIRESTRESS - stress-test expiration} {
        for {set t 0} { $t < $maxtime} {incr t} {
            puts "time $t; inserting $blocks"
            for {set i 0} { $i < $blocks} {incr i} {
                set key [encrypt [make_key $i]];
                r set $key {foo}
                r expire $key $expire_s
            }
            after $pause_ms;
        }
    } {}
}

