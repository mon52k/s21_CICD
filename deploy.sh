#!/bin/bash

scp cat/s21_cat ws2@192.168.10.2:/usr/local/bin/ 
scp grep/s21_grep ws2@192.168.10.2:/usr/local/bin/

if [ $? -ne 0 ]; then
    echo "Ошибка копирования"
    exit 1
fi

ssh ws2@192.168.10.2 ls -lah /usr/local/bin/

if [ $? -ne 0 ]; then
    echo "Ошибка перемещения"
    exit 1
fi
