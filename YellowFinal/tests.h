#pragma once

#include <vector>
#include "database.h"
#include "date.h"
#include "node.h"
#include "condition_parser.h"
#include "sstream"
#include "parse_event.h"
#include "test_runner.h"
void TestAdd();
void TestParseEvent();
void TestParseCondition();
void TestParseDate();
void TestDel();
void TestFind();
void TestLast();