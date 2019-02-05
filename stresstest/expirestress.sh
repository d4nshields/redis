#!/bin/sh
#ITERATIONS=2097152
ITERATIONS=1776
while [ $ITERATIONS -ge 1024 ]; do
  src/redis-cli flushdb >/dev/null
  echo $ITERATIONS,`src/redis-benchmark -r 1000000000 -n $ITERATIONS eval "local r=__rand_int__; \
  redis.call(\"set\",r,\"0123\");\
  redis.call(\"expire\",r,\"1\")" 0 | grep 'requests per second'`
  sleep 1
  ITERATIONS=$((ITERATIONS/2))
done

