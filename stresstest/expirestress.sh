#!/bin/sh
ITERATIONS=819200
#ITERATIONS=655360
  src/redis-cli flushdb >/dev/null
  echo $ITERATIONS,`src/redis-benchmark -R 39 -n $ITERATIONS -t psetex -P 200 | grep 'requests per second'`
