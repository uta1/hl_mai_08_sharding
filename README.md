## Начало работы

1. Для сборки кода выполните код:
```
mkdir build
cd build
cmake ..
make
```

2. Далее для развёртки контейнеров выполните следующие команды: 
```
cd docker
docker-compose up -d --build
```

3. Далее (при первом запуске контейнеров) дождитесь прогрузки контейнеров и выполните из корня репозитория код, создающий БД:
```
bash create_db.sh
```

При сборке образа на каждом из хостов *db-node-i* раскладываются специальный скрипт *init_db.sh*. Скрипт *create_db.sh* запускает его на каждом хосте.

4. Итак, БД созданы. Запускаем клиент *proxysql* командой:
```
bash start.sh
```

5. Флоу настроено. Можно приступать к тестированию.


## Тестирование

В файле *test_requests.sh* можно найти примеры запросов всех 3х типов из задания. При первом запуске скрипта вывод будет таким:
```
{"created":true}
{"created":true}
{"created":true}
{"age":23,"first_name":"ura","last_name":"skakovskiy","login":"uta7"}
{"age":23,"first_name":"ura","last_name":"skakovskiy","login":"uta8"}
[{"age":23,"first_name":"ura","last_name":"skakovskiy","login":"uta6"},{"age":23,"first_name":"ura","last_name":"skakovskiy","login":"uta7"},{"age":23,"first_name":"ura","last_name":"skakovskiy","login":"uta8"}]
```
При последущих запусках скрипта юзеры заново создаваться не будут. ```curl``` получит ответ ```409 Conflict```.

Проверить логику шардирования можно следующей командой:
```
bash print_users_on_hosts.sh
```
После выполнения скрипта *test_requests.sh* вывод будет следующим:
```
mysql: [Warning] Using a password on the command line interface can be insecure.
login	first_name	last_name	age
uta6	ura	skakovskiy	23
uta7	ura	skakovskiy	23

mysql: [Warning] Using a password on the command line interface can be insecure.
login	first_name	last_name	age
uta8	ura	skakovskiy	23

```
Т.о. эти 2 скрипта показывают, что запросы всех 3х типов работают корректно.
