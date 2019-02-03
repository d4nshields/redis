#!/bin/sh
ITERATIONS=2
src/redis-cli flushdb >/dev/null
while [ $ITERATIONS -ge 1 ]; do
  echo $ITERATIONS,`src/redis-benchmark -r 1000000000 -n 1 eval "local r=__rand_int__; \
  redis.call(\"set\",r,\"0123\");\
  redis.call(\"expire\",r,\"1\")" 0 | grep 'requests per second'`
  sleep 0.001
  ITERATIONS=$((ITERATIONS-1))
done

