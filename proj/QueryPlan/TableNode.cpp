#include "TableNode.h"

TableNode::TableNode() {
}

void TableNode::setTableName(std::string tableName) {
    this->tableName = tableName;
}

void TableNode::printTableName() {
    printf((this->tableName).c_str());
    printf("\n");
}

TableNode::~TableNode() {
}
