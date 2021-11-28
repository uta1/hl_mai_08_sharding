#!/bin/bash

docker exec -it db-node-1 bash /etc/mysql/init_db.sh
docker exec -it db-node-2 bash /etc/mysql/init_db.sh
