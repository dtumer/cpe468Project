//
//  FLOPPYResult.cpp
//

#include "FLOPPYResult.h"

FLOPPYResult::FLOPPYResult(FLOPPYResultType type) : _type(type) {
    msg = NULL;
}
FLOPPYResult::~FLOPPYResult() {
    if(msg)
    	free(msg);
}

void FLOPPYResult::print() {
    
    
    switch (type()) {
        case FLOPPYResultType::ErrorType:
            printf("ErrorType\n");
            printf("ERROR - %s\n", msg);
            break;
        case FLOPPYResultType::SelectType:
            printf("SelectType\n");
            recordSet->print();
            break;
        case FLOPPYResultType::UpdateType:
            printf("UpdateType\n");
            break;
        case FLOPPYResultType::InsertType:
            printf("InsertType\n");
            printf("%s\n", msg);
            break;
        case FLOPPYResultType::MessageType:
            printf("MessageType\n");
            printf("%s\n", msg);
            break;
    }
}
