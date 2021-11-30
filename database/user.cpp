#include "user.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/LogStream.h>
#include <Poco/Logger.h>

#include <sstream>
#include <exception>
#include <future>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Logger;
using Poco::LogStream;

namespace database
{

    Poco::JSON::Object::Ptr User::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("login", login);
        root->set("first_name", first_name);
        root->set("last_name", last_name);
        root->set("age", age);

        return root;
    }

    User User::fromJSON(const std::string &str)
    {
        User user;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        user.login = object->getValue<std::string>("login");
        user.first_name = object->getValue<std::string>("first_name");
        user.last_name = object->getValue<std::string>("last_name");
        user.age = object->getValue<int>("age");

        return user;
    }

    void User::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            //*
            Statement drop_stmt(session);
            drop_stmt << "DROP TABLE IF EXISTS users", now;
            //*/

            // (re)create table
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS users ("
                           "   login CHAR(50) NOT NULL PRIMARY KEY,"
                           "   first_name CHAR(50) NOT NULL,"
                           "   last_name CHAR(50) NOT NULL,"
                           "   age INT NOT NULL"
                           " );",
                           now;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::optional<User> User::read_by_login(std::string login)
    {
        try
        {
            auto db = database::Database::get();
            Poco::Data::Session session = db.create_session();
            Poco::Data::Statement select(session);

            const std::string sharding_hint = db.sharding_hint(login);
            std::string select_str = "SELECT login, first_name, last_name, age FROM users WHERE login=? " + sharding_hint;

            User a;
            select << select_str,
                    into(a.login),
                    into(a.first_name),
                    into(a.last_name),
                    into(a.age),
                    use(login),
                    range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                if (!select.execute()) return std::nullopt;
            }

            return a;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::vector<User> User::search_with_hint(std::optional<std::string> first_name, std::optional<std::string> last_name, const std::string& hint)
    {
        std::vector<User> result;

        try
        {
            auto db = database::Database::get();
            Poco::Data::Session session = db.create_session();
            Statement select(session);
            User a;
            std::string first_name_pattern = "%" + first_name.value_or("") + "%";
            std::string last_name_pattern = "%" + last_name.value_or("") + "%";
            std::string select_str = "SELECT login, first_name, last_name, age FROM users WHERE first_name LIKE ? AND last_name LIKE ? " + hint;
            select << select_str,
                    into(a.login),
                    into(a.first_name),
                    into(a.last_name),
                    into(a.age),
                    use(first_name_pattern),
                    use(last_name_pattern),
                    range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute()) {
                    result.push_back(a);
                }
            }
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }

        return result;
    }

    std::vector<User> User::search(std::optional<std::string> first_name, std::optional<std::string> last_name)
    {
        // get all hints for shards
        std::vector<std::string> hints = database::Database::get_all_hints();

        std::vector<std::future<std::vector<User>>> futures;

        // map phase in parallel
        for (const std::string &hint : hints)
        {
            auto handle = std::async(std::launch::async, [&first_name, &last_name, &hint]() -> std::vector<User>
            {
                return search_with_hint(first_name, last_name, hint);
            });

            futures.emplace_back(std::move(handle));
        }

        std::vector<User> result;
        // reduce phase
        // get values
        for(auto& res : futures){
            std::vector<User> v = res.get();
            std::copy(std::begin(v),
                      std::end(v),
                      std::back_inserter(result));
        }

        return result;
    }

   
    void User::save_to_mysql()
    {

        try
        {
            auto db = database::Database::get();
            Poco::Data::Session session = db.create_session();
            Poco::Data::Statement insert(session);

            const std::string sharding_hint = db.sharding_hint(login);
            std::string select_str = "INSERT INTO users (login, first_name, last_name, age) VALUES(?, ?, ?, ?) " + sharding_hint;
            insert << select_str,
                use(login),
                use(first_name),
                use(last_name),
                use(age);

            insert.execute();
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw std::string("connection error");
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw std::string("statement error");
        }
    }
}
