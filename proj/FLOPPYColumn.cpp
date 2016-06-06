//
//  FLOPPYColumn.cpp
//

#include "FLOPPYColumn.h"

FLOPPYColumn::FLOPPYColumn() {}

FLOPPYColumn::FLOPPYColumn(char *name, ColumnType type, int size, int offset) {
	this->name = name;
	this->type = type;
	this->size = size;
	this->offset = offset;
}

FLOPPYColumn::~FLOPPYColumn() {
    delete this->name;
}
