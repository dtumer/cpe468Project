//
//  FLOPPYRecord.cpp
//

#include "FLOPPYRecord.h"


FLOPPYRecord::FLOPPYRecord() {
    columns = new std::vector<FLOPPYRecordAttribute *>();
}

FLOPPYRecord::~FLOPPYRecord() {
    for (auto itr = this->columns->begin() ; itr != this->columns->end(); itr++) {
        delete (*itr);
    }
    delete columns;
}