#!/bin/csh
set login = whoami
echo $login

set pid = `ps aux | grep -v $login | cut -c10-14`
echo $pid