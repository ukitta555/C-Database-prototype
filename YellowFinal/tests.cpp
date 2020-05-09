#include "tests.h"
void TestAdd()
{
    // zero year
    {
        Database db;
        stringstream ss;
        ss << "0-1-1 event1";
        Date date = ParseDate(ss);
        const auto event = ParseEvent(ss);
        db.Add(date, event);
        AssertEqual(db.DebugPrint(), "0000-01-01 event1", "0 year");
    }
    // format
    {
        Database db;
        stringstream ss;
        ss << "1-1-1 event123";
        Date date = ParseDate(ss);
        const auto event = ParseEvent(ss);
        db.Add(date, event);
        AssertEqual(db.DebugPrint(), "0001-01-01 event123", "1 year");
    }

    {
        Database db;
        stringstream ss;
        ss << "001-12-01 event123";
        Date date = ParseDate(ss);
        const auto event = ParseEvent(ss);
        db.Add(date, event);
        AssertEqual(db.DebugPrint(), "0001-12-01 event123", "001 year");
    }
    // multiple additions
    {
        Database db;
        stringstream ss;

        ss << "001-12-01 event123";
        Date date = ParseDate(ss);
        const string event = ParseEvent(ss);
        db.Add(date, event);
        ss.clear();
        ss << "2001-12-01 event123";
        date = ParseDate(ss);
        const string event1 = ParseEvent(ss);
        db.Add(date, event1);

        AssertEqual(db.DebugPrint(), "0001-12-01 event1232001-12-01 event123", "001 + 2001 year");
    }
    // multiple additions with repeated dates
    {
        Database db;
        stringstream ss;

        ss << "001-12-01 event123";
        Date date = ParseDate(ss);
        const string event = ParseEvent(ss);
        db.Add(date, event);
        ss.clear();
        ss << "001-12-01 event1234";
        date = ParseDate(ss);
        const string event1 = ParseEvent(ss);
        db.Add(date, event1);

        AssertEqual(db.DebugPrint(), "0001-12-01 event1230001-12-01 event1234", "001 + 001 year");
    }
    // multiple additions with repeated dates + events
    {
        // multiple additions with repeated dates
        {
            Database db;
            stringstream ss;

            ss << "001-12-01 event123";
            Date date = ParseDate(ss);
            const string event = ParseEvent(ss);
            db.Add(date, event);
            ss.clear();
            ss << "001-12-01 event123";
            date = ParseDate(ss);
            const string event1 = ParseEvent(ss);
            db.Add(date, event1);

            AssertEqual(db.DebugPrint(), "0001-12-01 event123", "001 + 001 year");
        }
    }
}

void TestParseEvent() {
    {
        istringstream is("event");
        AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
    }
    {
        istringstream is("   sport event ");
        AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
    }
    {
        istringstream is("  first event  \n  second event");
        vector<string> events;
        events.push_back(ParseEvent(is));
        events.push_back(ParseEvent(is));
        AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
    }
}
