#include "GroupingNode.h"

GroupingNode::GroupingNode() {
}

void GroupingNode::setStatement(FLOPPYSelectStatement *statement) {
    this->statement = statement;
}

void GroupingNode::getAggregates() {
    for (unsigned i=0; i<this->statement->selectItems->size(); i++) {
        FLOPPYSelectItem *spec = this->statement->selectItems->at(i);
        if(spec->_type == FLOPPYSelectItemType::AggregateType) {
            aggregates.push_back(spec);
            //printFLOPPYAggregate(spec->aggregate.op, spec->aggregate.value);
        }
    }
}

void GroupingNode::printAggregates() {
    std::cout << "in printAggregates()\n";
    FLOPPYValue *val;
    for (std::vector<FLOPPYSelectItem *>::const_iterator i = aggregates.begin(); i != aggregates.end(); ++i) {
        val = (*i)->aggregate.value;
        std::cout << (*i)->aggregate.op << ' ' << val->sVal << ", ";
    }
    std::cout << '\n';
}

GroupingNode::~GroupingNode() {
}
