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
        Date date1 = ParseDate(ss);
        const string event = ParseEvent(ss);
        db.Add(date1, event);
        ss.clear();
        ss << "2001-12-01 event123";
        Date date2 = ParseDate(ss);
        const string event1 = ParseEvent(ss);
        db.Add(date2, event1);

        AssertEqual(db.DebugPrint(), "0001-12-01 event1232001-12-01 event123", "001 + 2001 year");
    }
    // multiple additions with repeated dates
    {
        Database db;
        stringstream ss;

        ss << "001-12-01 event123";
        Date date1 = ParseDate(ss);
        const string event = ParseEvent(ss);
        db.Add(date1, event);
        ss.clear();
        ss << "001-12-01 event1234";
        Date date2 = ParseDate(ss);
        const string event1 = ParseEvent(ss);
        db.Add(date2, event1);

        AssertEqual(db.DebugPrint(), "0001-12-01 event1230001-12-01 event1234", "001 + 001 year");
    }
    // multiple additions with repeated dates + events
    {
        // multiple additions with repeated dates
        {
            Database db;
            stringstream ss;

            ss << "001-12-01 event123";
            Date date1 = ParseDate(ss);
            const string event = ParseEvent(ss);
            db.Add(date1, event);
            ss.clear();
            ss << "001-12-01 event123";
            Date date2 = ParseDate(ss);
            const string event1 = ParseEvent(ss);
            db.Add(date2, event1);

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


void TestParseDate()
{
    // 2000-12-12 & 2012-12-12 test cases
    {
        istringstream is1 ("2000-12-12");
        
        Date date(2000, 12, 12);
        AssertEqual(ParseDate(is1), date, "2000-12-12");

        istringstream is2 ("2012-12-12");
        Assert( (ParseDate(is2) != date), "2012-12-12");
    }
}



void TestParseCondition() {
    {
        istringstream is("date != 2017-11-18");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({ 2017, 1, 1 }, ""), "Parse condition 1");
        Assert(!root->Evaluate({ 2017, 11, 18 }, ""), "Parse condition 2");
    }
    {
        istringstream is(R"(event == "sport event")");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({ 2017, 1, 1 }, "sport event"), "Parse condition 3");
        Assert(!root->Evaluate({ 2017, 1, 1 }, "holiday"), "Parse condition 4");
    }
    {
        istringstream is("date >= 2017-01-01 AND date < 2017-07-01");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({ 2017, 1, 1 }, ""), "Parse condition 5");
        Assert(root->Evaluate({ 2017, 3, 1 }, ""), "Parse condition 6");
        Assert(root->Evaluate({ 2017, 6, 30 }, ""), "Parse condition 7");
        Assert(!root->Evaluate({ 2017, 7, 1 }, ""), "Parse condition 8");
        Assert(!root->Evaluate({ 2016, 12, 31 }, ""), "Parse condition 9");
    }
    {
        istringstream is(R"(event != "sport event" AND event != "Wednesday")");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({ 2017, 1, 1 }, "holiday"), "Parse condition 10");
        Assert(!root->Evaluate({ 2017, 1, 1 }, "sport event"), "Parse condition 11");
        Assert(!root->Evaluate({ 2017, 1, 1 }, "Wednesday"), "Parse condition 12");
    }
    {
        istringstream is(R"(event == "holiday AND date == 2017-11-18")");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(!root->Evaluate({ 2017, 11, 18 }, "holiday"), "Parse condition 13");
        Assert(!root->Evaluate({ 2017, 11, 18 }, "work day"), "Parse condition 14");
        Assert(root->Evaluate({ 1, 1, 1 }, "holiday AND date == 2017-11-18"), "Parse condition 15");
    }
    {
        istringstream is(R"(((event == "holiday" AND date == 2017-01-01)))");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({ 2017, 1, 1 }, "holiday"), "Parse condition 16");
        Assert(!root->Evaluate({ 2017, 1, 2 }, "holiday"), "Parse condition 17");
    }
    {
        istringstream is(R"(date > 2017-01-01 AND (event == "holiday" OR date < 2017-07-01))");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(!root->Evaluate({ 2016, 1, 1 }, "holiday"), "Parse condition 18");
        Assert(root->Evaluate({ 2017, 1, 2 }, "holiday"), "Parse condition 19");
        Assert(root->Evaluate({ 2017, 1, 2 }, "workday"), "Parse condition 20");
        Assert(!root->Evaluate({ 2018, 1, 2 }, "workday"), "Parse condition 21");
    }
    {
        istringstream is(R"(date > 2017-01-01 AND event == "holiday" OR date < 2017-07-01)");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({ 2016, 1, 1 }, "event"), "Parse condition 22");
        Assert(root->Evaluate({ 2017, 1, 2 }, "holiday"), "Parse condition 23");
        Assert(root->Evaluate({ 2017, 1, 2 }, "workday"), "Parse condition 24");
        Assert(!root->Evaluate({ 2018, 1, 2 }, "workday"), "Parse condition 25");
    }
    {
        istringstream is(R"(((date == 2017-01-01 AND event == "holiday")))");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({ 2017, 1, 1 }, "holiday"), "Parse condition 26");
        Assert(!root->Evaluate({ 2017, 1, 2 }, "holiday"), "Parse condition 27");
    }
    {
        istringstream is(R"(((event == "2017-01-01" OR date > 2016-01-01)))");
        shared_ptr<Node> root = ParseCondition(is);
        Assert(root->Evaluate({ 1, 1, 1 }, "2017-01-01"), "Parse condition 28");
        Assert(!root->Evaluate({ 2016, 1, 1 }, "event"), "Parse condition 29");
        Assert(root->Evaluate({ 2016, 1, 2 }, "event"), "Parse condition 30");
    }
}

void TestDel()
{
    // add-del-add-del combination
    {
        Database db;
        db.Add({2000,12,12}, "xmas");

        istringstream is(R"(((event == "xmas")))");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date& date, const string& event) {
            return condition->Evaluate(date, event);
        };
        int count = db.RemoveIf(predicate);

        AssertEqual(count, 1, "count, 1, first try to empty");


        db.Add({ 2000,12,12 }, "1xmas1");

        istringstream is1(R"(((event == "1xmas1")))");
        condition = ParseCondition(is1);
        auto predicate1 = [condition](const Date& date, const string& event) {
            return condition->Evaluate(date, event);
        };
        count = db.RemoveIf(predicate1);

        AssertEqual(count, 1, "count, 1, second try to empty");
    }
    // multiple events in 1 day deletion
    {
        Database db;
        db.Add({ 2012, 12, 12}, "xmas");
        db.Add({ 2000, 12, 12}, "summer day");
        db.Add({ 2000, 12, 12 }, "saw her face");
        db.Add({ 2000, 12, 12 }, "I thought that she could be the one");


        istringstream is(R"(((date == 2000-12-12)))");
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date& date, const string& event) {
            return condition->Evaluate(date, event);
        };
        int count = db.RemoveIf(predicate);
        AssertEqual(count, 3, "multiple deletions");
    }
    // del all xmas'es
    {
        Database db;
        db.Add({ 2012, 12, 12}, "xmas");
        db.Add({ 2011, 12, 12}, "xmas");
        db.Add({ 2010, 12, 12 }, "xmas");
        db.Add({ 2000, 12, 12 }, "I thought that she could be the one");

        istringstream is1(R"(((event == "xmas")))");

        auto condition = ParseCondition(is1);
        auto predicate = [condition](const Date& date, const string& event) {
            return condition->Evaluate(date, event);
        };
        int count = db.RemoveIf(predicate);
        AssertEqual(count, 3, "multiple deletions of xmas");
    }

    // clean up the vector for 1 day and test whether refilling it works
    {
        Database db;
        db.Add({ 2012, 12, 12 }, "xmas");
        db.Add({ 2000, 12, 12 }, "summer day");
        db.Add({ 2000, 12, 12 }, "saw her face");
        db.Add({ 2000, 12, 12 }, "I thought that she could be the one");


        istringstream is1(R"(((date == 2000-12-12)))");
        auto condition1 = ParseCondition(is1);
        auto predicate1 = [condition1](const Date& date, const string& event) {
            return condition1->Evaluate(date, event);
        };
        int count1 = db.RemoveIf(predicate1);
        AssertEqual(count1, 3, "multiple deletions (cleaning 1 vector)");

        db.Add({ 2000, 12, 12 }, "summer day1");
        db.Add({ 2000, 12, 12 }, "saw her face1");
        db.Add({ 2000, 12, 12 }, "I thought that she could be the one1");


        istringstream is2(R"(((date == 2000-12-12)))");
        auto condition2 = ParseCondition(is2);
        auto predicate2 = [condition2](const Date& date, const string& event) {
            return condition2->Evaluate(date, event);
        };
        int count2 = db.RemoveIf(predicate2);
        AssertEqual(count2, 3, "multiple deletions (refill + cleaning  vector again)");

    }
}

void TestFind()
{
    { // find multiple events on 1 day && 0 year
        Database db;
        Date d = {0, 12, 12};
        db.Add(d, "test1");
        db.Add(d, "test2");
        
        istringstream is1(R"(((date == 0-12-12)))");
        auto condition1 = ParseCondition(is1);
        auto predicate1 = [condition1](const Date& date, const string& event) {
            return condition1->Evaluate(date, event);
        };
        
        vector<pair<Date, string>> correct = { make_pair(d, "test1"), make_pair(d, "test2") };
        AssertEqual(correct, db.FindIf(predicate1), "0 year & multiple events on 1 day");
    }
    { // find 1 event on different days
        Database db;
        Date date1 = { 2000, 12, 12 };
        Date date2 = { 2000, 11, 11 };
        db.Add(date1, "tobefound");
        db.Add(date2, "tobefound");
        db.Add(date1, "nottobefound");

        istringstream is1(R"(((event == "tobefound")))");
        auto condition1 = ParseCondition(is1);
        auto predicate1 = [condition1](const Date& date, const string& event) {
            return condition1->Evaluate(date, event);
        };


        vector <pair<Date, string>> correct = { make_pair(date2, "tobefound"), make_pair(date1, "tobefound") };
        AssertEqual(correct, db.FindIf(predicate1), "different days & same event");
    }
    { // search in empty database
        Database db;
        istringstream is1(R"(((event == "tobefound")))");
        auto condition1 = ParseCondition(is1);
        auto predicate1 = [condition1](const Date& date, const string& event) {
            return condition1->Evaluate(date, event);
        };
        vector <pair<Date, string>> correct = {};
        AssertEqual(correct, db.FindIf(predicate1), "empty db");
    }
    { // check for correct format
        Database db;
        Date date = {1, 2, 3};
        db.Add(date, "tobefound");
        istringstream is1(R"(((event == "tobefound")))");
        auto condition1 = ParseCondition(is1);
        auto predicate1 = [condition1](const Date& date, const string& event) {
            return condition1->Evaluate(date, event);
        };
        vector <pair<Date, string>> correct = {make_pair(date, "tobefound")};
        AssertEqual(correct, db.FindIf(predicate1), "format test 1");
        for (const auto& event : correct)
        {
            cerr << event << endl;
        }
    }
}

void TestLast()
{
    {// test all possible queries for given db (before everything, in the middle, after everything)
        Database db;
        Date d = { 1899, 10, 10 };
        try
        {
            cerr << db.Last(d); // should output No events
        }
        catch (invalid_argument&) {
            cerr << "No entries" << endl;
        }
        Date d1 = {1900, 12, 12};
        db.Add(d1, "test1");
        Date d2 = { 2000,12,12 };
        db.Add(d2, "test2");
        db.Add(d2, "test3");
        pair<Date, string> correct = make_pair(d1, "test1");
        Date search1 = {1900, 12, 12};
        AssertEqual(correct, db.Last(search1), "1900-12-12");
        Date search2 = { 1950, 12, 12 };
        AssertEqual(correct, db.Last(search2), "1950-12-12");
        Date search3 = { 2000,12,12 };
        pair<Date, string> correct1 = make_pair(d2, "test3");
        AssertEqual(correct1, db.Last(search3), "2000-12-12 two events");
        Date search4 = { 2001,12,12 };
        AssertEqual(correct1, db.Last(search4), "2001-12-12 after everything");
    }
}