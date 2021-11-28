#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>

namespace database{
    class Database{
        private:
            std::string _connection_string;
            Database();
        public:
            static Database& get();
            Poco::Data::Session create_session();
            static size_t shards_count();
            static std::string sharding_hint(const std::string& login);
            static std::vector<std::string> get_all_hints();
    };
}
#endif