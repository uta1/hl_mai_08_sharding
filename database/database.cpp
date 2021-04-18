#include "database.h"
#include "../config/config.h"
#include <functional>

namespace database{
    Database::Database(){
        _connection_string+="host=";
        _connection_string+=Config::get().get_host();
        _connection_string+=";user=";
        _connection_string+=Config::get().get_login();
        _connection_string+=";db=";
        _connection_string+=Config::get().get_database();
        _connection_string+=";password=";
        _connection_string+=Config::get().get_password();

        Poco::Data::MySQL::Connector::registerConnector();
    }

    std::string Database::sharding_hint(long from, long to){
        static size_t max_shards = 2;
        std::string key;

        key += std::to_string(from);
        key += ";";
        key += std::to_string(to);

        size_t shard_number = std::hash<std::string>{}(key)%max_shards;

        std::string result = "-- sharding:";
        result += shard_number;
        return result;
    }

    Database& Database::get(){
        static Database _instance;
        return _instance;
    }

    Poco::Data::Session Database::create_session(){
        return Poco::Data::Session(Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, _connection_string));
    }

}