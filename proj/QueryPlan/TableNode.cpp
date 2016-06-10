#include "TableNode.h"

TableNode::TableNode() {
    this->tableName = "";
    this->alias = "";
}

void TableNode::setTableName(char* tableName) {
    this->tableName = tableName == NULL ? std::string() : std::string(tableName);
}
const char* TableNode::getTableName() {
    return this->tableName.empty() ? "" : this->tableName.c_str();
}

void TableNode::setAlias(char* tableAlias) {
    this->alias = tableAlias == NULL ? std::string() : std::string(tableAlias);
}

const char* TableNode::getAlias() {
    return this->alias.empty() ? "" : this->alias.c_str();
}

void TableNode::printTableName() {
    printf((this->tableName).c_str());
    printf("\n");
}

TableNode::~TableNode() {
}
