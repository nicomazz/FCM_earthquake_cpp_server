//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_USERPROVIDER_H
#define SIMPLE_WEB_SERVER_USERPROVIDER_H

#include "Models/User/User-odb.hpp"
#include "Models/Event/Event.hpp"
#include "Utility/TimeUtility.hpp"
#include "Models/User/User.hpp"
#include <vector>
#include <iostream>
#include <sqlite3.h>

/**
 * da qui si effettueranno query al db sqlite per prendere gli utenti
 * a cui inviare la notifica (Basandosi sulle preferenze di ogniuno)
 */
class UserPreferenceProvider {
public:

    static std::vector<User> requestUsersFromDB();

    static std::vector<User> requestActiveUsers();

        /**
         * salva lo stato dell'utente sul db  e aggiorna l'id di user
         * mettendo quello corrispondente nel db
         * @param user
         * @param checkAlreadyPresent
         */
    static long persistUser(User &user, bool checkAlreadyPresent = false);

    /**
     *
     * @param user
     */
    static void updateUser(User &user);

    static User getUser(long id);

    static void removeUser(User & user);
    static bool isUserPresent(long id);

    /**
     * gestisce cosa fare dopo una richiesta web di aggiungere un utente
     * o aggiornare il suo firebase id
     * @return id dell'utente aggiunto/aggiornato
     */
    static long updateOrInsertUser(User &user);

    /**
     * check that a user is in the database and has the right secret key.
     * if there isn't a match throw std::invalid_argument exception
     * @param user
     */
    static void checkValidUserInDB(User & user);

};


#endif //SIMPLE_WEB_SERVER_USERPROVIDER_H
