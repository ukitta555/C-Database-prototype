#include "node.h"

Node :: Node (const NodeType type) : type(type) {};

EmptyNode::EmptyNode() : Node(NodeType::Empty) {};

DateComparisonNode::DateComparisonNode(const Comparison cmp, const Date& date)
	: Node(NodeType::DateComparison), cmp(cmp), date(date) {};

EventComparisonNode::EventComparisonNode(const Comparison cmp, const std::string& event)
	: Node(NodeType::EventComparison), cmp(cmp), event(event) {};

LogicalOperationNode::LogicalOperationNode (const LogicalOperation logicalOperation
	, const std::shared_ptr<Node> left
	, const std::shared_ptr<Node> right)
	: Node(NodeType::LogicalOperation), logicalOperation(logicalOperation), left(left), right(right) {};