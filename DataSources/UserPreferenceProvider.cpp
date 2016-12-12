//
// Created by nicomazz97 on 20/11/16.
//

#include <Models/User/User.hpp>
#include <DataSources/Database.hpp>
#include <Utility/UserMatching.hpp>
#include <syslog.h>
#include "UserPreferenceProvider.hpp"

std::vector<User> UserPreferenceProvider::requestUsersFromDB() {
    using namespace odb::core;
    std::vector<User> results;
    std::shared_ptr<database> db = Database::getInstance().getDatabase();
    {
        typedef odb::result<User> result;

        // session s;
        transaction t(db->begin());

        result r(db->query<User>());

        for (const User &e: r)
            results.push_back(e);

        t.commit();
    }

    return results;
}
std::vector<User> UserPreferenceProvider::requestActiveUsers() {
    using namespace odb::core;
    typedef odb::query<User> query;
    typedef odb::result<User> result;

    std::vector<User> results;
    std::shared_ptr<database> db = Database::getInstance().getDatabase();
    {

        // session s;
        transaction t(db->begin());

        long fromTime = TimeUtils::getCurrentMillis() - (1000*60*60); // millis of one hour ago
        result r(db->query<User>(query::lastActivity > fromTime));

        for (const User &e: r)
            results.push_back(e);

        t.commit();
    }

    return results;
}
long UserPreferenceProvider::persistUser(User &user, bool checkAlreadyPresent) {
    using namespace odb::core;

    if (checkAlreadyPresent && isUserPresent(user.id)) // esiste già
        return user.id;

    try {
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        {
            transaction t(db->begin());
            user.lastModify = TimeUtils::getCurrentMillis();
            user.id = db->persist(user);
            //std::cout<<"id nuovo: "<<user.id<<"\n";
            t.commit();
            return user.id;
        }
    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return -1;
}

User UserPreferenceProvider::getUser(long id) {
    using namespace odb::core;
    typedef odb::query<User> query;
    typedef odb::result<User> result;

    try {
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        transaction t(db->begin());

        result r(db->query<User>(query::id == id));
        // result<User> r(db->query<User>());


        for (const User e: r) {
            t.commit();
            return e;
        }
        t.commit();

    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return User();
}

bool UserPreferenceProvider::isUserPresent(long id) {
    return getUser(id).id == id;
}


void UserPreferenceProvider::updateUser(User &user) {
    using namespace odb::core;
    try {
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        transaction t(db->begin());
        user.lastModify = TimeUtils::getCurrentMillis();
        db->update(user);
        t.commit();
    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }

}

long UserPreferenceProvider::updateOrInsertUser(User &user) {
    UserPreferenceProvider userPP;

    if (user.hasId()){
        checkValidUserInDB(user); // throw exception if is not valid
        userPP.updateUser(user);
    }
    else // new user
        userPP.persistUser(user);

    return user.id;
}

void UserPreferenceProvider::removeUser(User &user) {
    using namespace odb::core;

    try {
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        transaction t(db->begin());
        db->erase(user);
        t.commit();

    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}



void UserPreferenceProvider::checkValidUserInDB(User &user) {
    User inDB = getUser(user.id);
    if (inDB.id < 0)
        throw std::invalid_argument("Error: User not in database!");
    if (inDB.secretKey != user.secretKey)
        throw std::invalid_argument("Error: secretKey mismatch!");
}
