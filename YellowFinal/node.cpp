#include "node.h"

Node :: Node (const NodeType type) : type(type) {};


EmptyNode::EmptyNode() : Node(NodeType::Empty) {};

bool EmptyNode::Evaluate(const Date& date, const std::string& event) const{
	return true;
}

DateComparisonNode::DateComparisonNode(const Comparison cmp, const Date& date)
	: Node(NodeType::DateComparison), cmp(cmp), date(date) {};

bool DateComparisonNode::Evaluate(const Date& date, const std::string& event) const 
{
	if (cmp == Comparison::Less)
	{
		return (date < this->date);
	}
	else if (cmp == Comparison::LessOrEqual)
	{
		return (date < this->date) || (date == this->date);
	}
	else if (cmp == Comparison::Greater)
	{
		return (this->date < date);
	}
	else if (cmp == Comparison::GreaterOrEqual)
	{
		return ((this->date) < date || (this->date == date))
	}
	else if (cmp == Comparison::Equal)
	{
		return (this->date == date);
	}
	else if (cmp == Comparison::NotEqual)
	{
		return (!(this->date == date));
	}
	return true;
}

EventComparisonNode::EventComparisonNode(const Comparison cmp, const std::string& event)
	: Node(NodeType::EventComparison), cmp(cmp), event(event) 
{
};

LogicalOperationNode::LogicalOperationNode (const LogicalOperation logicalOperation
	, const std::shared_ptr<Node> left
	, const std::shared_ptr<Node> right)
	: Node(NodeType::LogicalOperation), logicalOperation(logicalOperation), left(left), right(right) {};