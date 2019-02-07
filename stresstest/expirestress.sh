#!/bin/sh
#ITERATIONS=2097152
ITERATIONS=655360
  src/redis-cli flushdb >/dev/null
  echo $ITERATIONS,`src/redis-benchmark -r 8000000 -n $ITERATIONS -t psetex | grep 'requests per second'`
