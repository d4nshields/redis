#!/bin/sh
ITERATIONS=50000
src/redis-cli flushdb >/dev/null
echo $ITERATIONS,`src/redis-benchmark -r 1000000000 -n $ITERATIONS eval "local r=__rand_int__; \
redis.call(\"set\",r,\"0123\");\
redis.call(\"expire\",r,\"1\")" 0 | grep 'requests per second'`
sleep 1
