//
// Created by nicomazz97 on 20/11/16.
//

#include <Models/User/User.hpp>
#include <DataSources/Database.hpp>
#include <Utility/UserMatching.hpp>
#include "UserPreferenceProvider.hpp"

std::vector<User> UserPreferenceProvider::requestUsersFromDB() {
    using namespace odb::core;
    std::vector<User> results;

    database *db = Database::getInstance().getUserDatabase();

    {
        typedef odb::query<User> query;
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

void UserPreferenceProvider::persistUser(User& user, bool checkAlreadyPresent) {
    using namespace odb::core;

    if (checkAlreadyPresent && isUserPresent(user.id)) // esiste già
        return;

    try {
        database *db = Database::getInstance().getUserDatabase();
        {
            transaction t(db->begin());
            user.id = db->persist(user);
            std::cout<<"id nuovo: "<<user.id<<"\n";
            t.commit();
        }
    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

User UserPreferenceProvider::getUser(long  id) {
    using namespace odb::core;
    typedef odb::query<User> query;
    typedef odb::result<User> result;

    try {
        database *db = Database::getInstance().getUserDatabase();
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

std::vector<User> UserPreferenceProvider::requestUsersToNotify(Event event) {
    std::vector<User> allUsers = requestUsersFromDB();
    std::vector<User> toNotify;

    for (User &user: toNotify)
        if (isUserToBeNotified(user, event)) {
            toNotify.push_back(user);
        }


    return std::vector<User>();
}

bool UserPreferenceProvider::isUserToBeNotified(User &u, Event &e) {
    return UserMatching(u, e).toNotify();
}

void UserPreferenceProvider::updateUser(User &user) {
    using namespace odb::core;
    typedef odb::query<User> query;
    typedef odb::result<User> result;
    try {
        database *db = Database::getInstance().getUserDatabase();
        transaction t(db->begin());
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
