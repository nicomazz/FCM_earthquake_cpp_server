//
// Created by nicomazz97 on 04/02/17.
//

#include <Utility/TimeUtility.hpp>
#include <Models/Event/Event.hpp>
#include <DataSources/EventProvider.hpp>
#include <DataSources/ReportProvider.hpp>
#include "ReportChecker.hpp"

void ReportChecker::checkEventRelatedReport(int minutes) {
    long minutes_in_millis = minutes * TimeUtility::MILLIS_IN_MINUTE;
    long from_millis = TimeUtility::getCurrentMillis() - minutes_in_millis * 2;
    long to_millis = TimeUtility::getCurrentMillis() - minutes_in_millis;
    std::vector<Event> events = EventProvider::requestEventsInInterval(from_millis, to_millis);
    for (Event e : events){
        if (e.numberOfReports != 0) continue; // it already has right number
        std::vector<DBReport> reports = ReportProvider::getReportsRelatedToEvents(e);
        e.numberOfReports = (int) reports.size();
        EventProvider::updateEvent(e);
    }
}
