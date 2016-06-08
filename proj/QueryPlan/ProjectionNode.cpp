#include "ProjectionNode.h"

ProjectionNode::ProjectionNode() {
    op = PROJECTION;
}

void ProjectionNode::addToProjection(std::string tableName, std::string colName) {
    Column col = {tableName, colName};
    columns.push_back(col);
}

ProjectionNode::~ProjectionNode() {
}
