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

void UserPreferenceProvider::persistUser(User &user, bool checkAlreadyPresent) {
    using namespace odb::core;

    if (checkAlreadyPresent && isUserPresent(user.id)) // esiste già
        return;

    try {
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        {
            transaction t(db->begin());
            user.lastModify = TimeUtils::getCurrentMillis();
            user.id = db->persist(user);
            //std::cout<<"id nuovo: "<<user.id<<"\n";
            t.commit();
        }
    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
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

long UserPreferenceProvider::handleNewUserRequest(User &user) {
    UserPreferenceProvider userPP;

    if (user.id > 0 && userPP.isUserPresent(user.id))
        userPP.updateUser(user);
    else
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

UserPreferenceProvider::UserPreferenceProvider() {

}
