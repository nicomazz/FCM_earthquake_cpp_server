//
// Created by nicomazz97 on 29/11/16.
//

#ifndef SIMPLE_WEB_SERVER_COMMONPROVIDER_H
#define SIMPLE_WEB_SERVER_COMMONPROVIDER_H

template<class T>
class CommonProvider {
public:
    CommonProvider() {}

    std::vector <T> get();

    long persist(T &e, bool checkAlreadyPresent = false);

    T get(long id);

    void erase(long id);

    bool isPresent(long id);

};


#endif //SIMPLE_WEB_SERVER_COMMONPROVIDER_H
