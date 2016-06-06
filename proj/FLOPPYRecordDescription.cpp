//
//  FLOPPYRecordDescription.cpp
//

#include "FLOPPYRecordDescription.h"

FLOPPYRecordDescription::FLOPPYRecordDescription() {
    columns = new std::vector<FLOPPYColumn *>();
}

FLOPPYRecordDescription::~FLOPPYRecordDescription() {
    for (auto itr = this->columns->begin() ; itr != this->columns->end(); itr++) {
        delete (*itr);
    }
    delete columns;
}

void FLOPPYRecordDescription::addColumn(char *name, ColumnType type, int size, int offset) {
    FLOPPYColumn *column = new FLOPPYColumn(name, type, size, offset);
    columns->push_back(column);
}