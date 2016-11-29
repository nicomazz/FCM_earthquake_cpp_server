//
// Created by nicomazz97 on 29/11/16.
//

#ifndef SIMPLE_WEB_SERVER_COMMONPROVIDER_H
#define SIMPLE_WEB_SERVER_COMMONPROVIDER_H


class CommonProvider {
    public:
        EventProvider() {}

        std::vector<Event> requestEventWebUpdate();

        std::vector<Event> requestNewEventNotInDB();

        std::vector<Event> requestEventFromDB();

        long persistEvent(Event &e, bool checkAlreadyPresent = false);

        Event getEvent(long id);

        void deleteEvent(Event e);

        bool isEventPresent(long id);

};


#endif //SIMPLE_WEB_SERVER_COMMONPROVIDER_H
