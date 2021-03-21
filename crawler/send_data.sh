#!/bin/sh

while true; do
    MACHINE_ID=[machine ID]
    DASHBOARD=[dashboard url http://domain:port/path/to/index.php]
    THREADS=$(ps -e -T | grep "python3" | wc -l)
    LOGS=$(python3 src/log_analyzer.py)
    STORAGE=$(du -h data/)

    echo $THREADS
    echo $LOGS
    echo $STORAGE

    curl -X POST --data "ID=$MACHINE_ID&threads=$THREADS&logs=$LOGS&storage=$STORAGE" $DASHBOARD

    sleep 60
done
