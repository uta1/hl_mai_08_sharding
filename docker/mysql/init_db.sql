CREATE DATABASE IF NOT EXISTS sql_test;
USE sql_test;
create table if not exists users (
    login char(50) not null primary key,
    first_name char(50) not null,
    last_name char(50) not null,
    age int not null
);
select * from users;
