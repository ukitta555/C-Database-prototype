#pragma once

#include "LogicalOperation.h"
#include "comparison.h"
#include "date.h"
#include <string>
#include <memory>

enum class NodeType
{
	Empty,
	DateComparison,
	EventComparison,
	LogicalOperation
};

class Node
{
public:
	Node();
	virtual bool Evaluate(const Date& date, const std::string& event) const = 0;
	const NodeType type;
};

class EmptyNode : public Node
{
};

class EventComparisonNode : public Node
{
public:
	const Comparison cmp;
	const Date date;
};

class LogicalOperationNode : public Node
{
public:
	const LogicalOperation logicalOperation;
	const std::shared_ptr<Node> left;
	const std::shared_ptr<Node> right;
};