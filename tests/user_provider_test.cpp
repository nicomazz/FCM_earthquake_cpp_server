
//
// Created by nicomazz97 on 21/11/16.
//

#include "ServerUtility/server_http.hpp"
#include "ServerUtility/client_http.hpp"
#include "ServerUtility/FirecloudServerInitializer.hpp"
#include "../DataSources/EventProvider.hpp"

#include <cassert>

using namespace std;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

int main() {

    UserPreferenceProvider up;

    // test event add/delete

    User u;
    u.firebaseID = "testUser";
    int pre_size = (int) up.requestUsersFromDB().size();
    long inserted_id = up.persistUser(u);
    assert(inserted_id > 0);
    assert(u.id == inserted_id);
    assert((int) up.requestUsersFromDB().size() == pre_size + 1);

    //ispresent
    assert(up.isUserPresent(u.id));

    //delete
    up.removeUser(u);
    User deleted = up.getUser(inserted_id);
    assert(deleted.id<=0);


    std::cerr<<"All is working for User!";

    return 0;
}
