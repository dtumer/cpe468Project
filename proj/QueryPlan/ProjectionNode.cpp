#include "ProjectionNode.h"

ProjectionNode::ProjectionNode() {
    this->op = PROJECTION;
}

void ProjectionNode::addToProjection(std::string tableName, std::string colName) {
    Column col = {tableName, colName};
    columns.push_back(col);
}

void ProjectionNode::printColumns() {
    std::cout << "in PrintColumns()\n";
    for (std::vector<Column>::const_iterator i = columns.begin(); i != columns.end(); ++i)
        std::cout << (*i).tableName << ' ' << (*i).colName << ", ";
    std::cout << '\n';
}

void ProjectionNode::setItems(std::vector<FLOPPYSelectItem *> *items) {
    this->items = items;
}

std::vector<FLOPPYSelectItem *> * ProjectionNode::getItems() {
    return this->items;
}

ProjectionNode::~ProjectionNode() {
}
