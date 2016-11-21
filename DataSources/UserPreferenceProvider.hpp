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
    UserPreferenceProvider() {}

    std::vector<User> requestUsersFromDB();


    /**
     * salva lo stato dell'utente sul db  e aggiorna l'id di user
     * mettendo quello corrispondente nel db
     * @param user
     * @param checkAlreadyPresent
     */
    void persistUser(User &user, bool checkAlreadyPresent = false);

    /**
     *
     * @param user
     */
    void updateUser(User &user);

    User getUser(long id);

    bool isUserPresent(long id);

    /**
     * gestisce cosa fare dopo una richiesta web di aggiungere un utente
     * o aggiornare il suo firebase id
     * @return id dell'utente aggiunto/aggiornato
     */
    long handleNewUserRequest(User & user);

private:
    bool isUserToBeNotified(User &u, Event &e);
};


#endif //SIMPLE_WEB_SERVER_USERPROVIDER_H
