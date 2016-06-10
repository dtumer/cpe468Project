#include "GroupingNode.h"

GroupingNode::GroupingNode() {
}

GroupingNode::GroupingNode(FLOPPYSelectStatement *statement) {
	this->statement = statement;
}

void GroupingNode::setStatement(FLOPPYSelectStatement *statement) {
    this->statement = statement;
}

std::vector<FLOPPYSelectItem *> GroupingNode::getAggregates() {
    return this->aggregates;
}

void GroupingNode::loadAggregates() {
    //get aggregates in Select Statement
    for (unsigned int i=0; i<this->statement->selectItems->size(); i++) {
        if(this->statement->selectItems->at(i)->_type == FLOPPYSelectItemType::AggregateType) {
            aggregates.push_back(spec);
            //printFLOPPYAggregate(spec->aggregate.op, spec->aggregate.value);
        }
    }

    //get aggregates in Having section
    if(this->statement->groupBy->havingCondition) {
        //loadHavingAggregates(this->statement->groupBy->havingCondition);
    }
}

FLOPPYSelectItem* GroupingNode::loadHavingAggregates(FLOPPYNode *node) {
	if (node->_type == FLOPPYNodeType::AggregateNode) {
		printf("AGGREGATION\n");
	}
	else if (node->_type == FLOPPYNodeType::ConditionNode) {
		printf("CONDITION\n");
	}
	else if (node->_type == FLOPPYNodeType::ExpressionNode) {
		printf("EXPRESSION\n");
	}
	else {
		printf("VALUE NODE\n");
	}
	
	return NULL;	
}
	
    // if(node->_type == FLOPPYNodeType::ConditionNode || node->_type == FLOPPYNodeType::ExpressionNode) {
//         if(node->node.op == FLOPPYNodeOperator::AndOperator) {
//             loadHavingAggregates(node->node.left);
//             loadHavingAggregates(node->node.right);
//         }
//         else if(node->node.op == FLOPPYNodeOperator::NotOperator) {
//             loadHavingAggregates(node->node.left);
//         }
//         else if(node->node.op == FLOPPYNodeOperator::GreaterThanOperator) {
//             loadHavingAggregates(node->node.left);
//             loadHavingAggregates(node->node.right);
//         }
//         else if(node->node.op == FLOPPYNodeOperator::GreaterThanEqualOperator) {
//             loadHavingAggregates(node->node.left);
//             loadHavingAggregates(node->node.right);
//         }
//         else if(node->node.op == FLOPPYNodeOperator::LessThanOperator) {
//             loadHavingAggregates(node->node.left);
//             loadHavingAggregates(node->node.right);
//         }
//         else if(node->node.op == FLOPPYNodeOperator::LessThanEqualOperator) {
//             loadHavingAggregates(node->node.left);
//             loadHavingAggregates(node->node.right);
//         }
//         else if(node->node.op == FLOPPYNodeOperator::EqualOperator) {
//             loadHavingAggregates(node->node.left);
//             loadHavingAggregates(node->node.right);
//         }
//         else if(node->node.op == FLOPPYNodeOperator::NotEqualOperator) {
//             loadHavingAggregates(node->node.left);
//             loadHavingAggregates(node->node.right);
//         }
//         else if(node->node.op == FLOPPYNodeOperator::ParenthesisOperator) {
//             loadHavingAggregates(node->node.left);
//         }
//     }
//     else if(node->_type == FLOPPYNodeType::AggregateNode) {
//         FLOPPYSelectItem item(AggregateType);
//         //item.aggregate(node->aggregate.op, node->aggregate.value);
//         //aggregates.push_back(item);
//     }
//     else if(node->_type == FLOPPYNodeType::ValueNode) {
//     }
// }

void GroupingNode::printAggregates() {
    std::cout << "in printAggregates()\n";
    FLOPPYValue *val;
    for (std::vector<FLOPPYSelectItem *>::const_iterator i = aggregates.begin(); i != aggregates.end(); ++i) {
        val = (*i)->aggregate.value;
        if(!val) {
            std::cout << (*i)->aggregate.op << ", ";
        } else {
            std::cout << (*i)->aggregate.op << ' ' << val->sVal << ", ";
        }
    }
    std::cout << '\n';
}

void GroupingNode::printGroupByAttributes() {
    std::cout << "in printGroupByAttrbs()\n";
    std::vector<FLOPPYTableAttribute *> attrbs = *(getGroupByAttributes());
    char *tbl, *attr;
    for (std::vector<FLOPPYTableAttribute *>::const_iterator i = attrbs.begin(); i != attrbs.end(); ++i) {
        tbl = (*i)->tableName;
        attr = (*i)->attribute;
        if(tbl) {
            std::cout << tbl << ' ' << attr << ", ";
        } else {
            std::cout << attr << ", ";
        }
    }
    std::cout << '\n';
}

std::vector<FLOPPYTableAttribute *> * GroupingNode::getGroupByAttributes() {
    return this->statement->groupBy->groupByAttributes;
}

GroupingNode::~GroupingNode() {
}
