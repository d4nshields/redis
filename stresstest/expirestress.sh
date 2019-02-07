#!/bin/sh
ITERATIONS=2097152
#ITERATIONS=655360
  src/redis-cli flushdb >/dev/null
  echo $ITERATIONS,`src/redis-benchmark -r 8000000 -n $ITERATIONS eval "local r=KEYS[1];\
    redis.call(\"set\",r,\"0123\");\
    redis.call(\"expire\",r,\"1\");" 1 __rand_int__ | grep 'requests per second'`
