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
	Node(const NodeType type);
	virtual bool Evaluate(const Date& date, const std::string& event) const = 0;
private:
	const NodeType type;
};

class EmptyNode : public Node
{
public:
	EmptyNode();
	bool Evaluate(const Date& date, const std::string& event) const override;
};

class DateComparisonNode : public Node
{
public:
	DateComparisonNode(const Comparison cmp, const Date& date);
	bool Evaluate(const Date& date, const std::string& event) const override;
private:
	const Comparison cmp;
	const Date date;
};


class EventComparisonNode : public Node
{
public:
	EventComparisonNode(const Comparison cmp, const std::string& event);
private:
	const Comparison cmp;
	const std::string event;
};


class LogicalOperationNode : public Node
{
public:
	LogicalOperationNode(const LogicalOperation logicalOperation
						,const std::shared_ptr<Node> left
						,const std::shared_ptr<Node> right);
private:
	const LogicalOperation logicalOperation;
	const std::shared_ptr<Node> left;
	const std::shared_ptr<Node> right;
};