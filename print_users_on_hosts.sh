#!/bin/bash

docker exec -it db-node-1 bash /etc/mysql/print_users.sh
echo ''
docker exec -it db-node-2 bash /etc/mysql/print_users.sh
echo ''
docker exec -it db-node-3 bash /etc/mysql/print_users.sh
echo ''
