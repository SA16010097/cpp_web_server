#!/bin/bash

server_name="my_http_server"

pid=$(pidof $server_name)

if [[ $? -eq 0 ]]
then
    echo "Already exists. pid:$pid"
else
    /data/server/build/$server_name >>/data/server/log/3344_log.txt 2>&1 &   
    echo "Run $server_name."
fi
