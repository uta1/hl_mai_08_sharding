curl -X POST 'localhost:80/person' --data-raw '{"login":"uta6","first_name":"ura","last_name":"skakovskiy","age":23}' -v
curl -X POST 'localhost:80/person' --data-raw '{"login":"uta7","first_name":"ura","last_name":"skakovskiy","age":23}' -v
curl -X POST 'localhost:80/person' --data-raw '{"login":"uta8","first_name":"ura","last_name":"skakovskiy","age":23}' -v
curl -X GET 'localhost:80/person?login=uta7' -v
curl -X GET 'localhost:80/person?login=uta8' -v
curl -X GET 'localhost:80/person?first_name=ur' -v
