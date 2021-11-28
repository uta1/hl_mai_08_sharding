curl -X POST 'localhost:80/person' --data-raw '{"login":"uta1","first_name":"ura","last_name":"skakovskiy","age":23}' -v
curl -X GET 'localhost:80/person?login=uta1' -v
