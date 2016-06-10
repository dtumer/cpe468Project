//
//  FLOPPYResult.cpp
//

#include "FLOPPYResult.h"

using namespace std;

FLOPPYResult::FLOPPYResult(FLOPPYResultType type) : _type(type) {
    msg = NULL;
}
FLOPPYResult::~FLOPPYResult() {
    if(_type == FLOPPYResultType::SelectType)
        delete recordSet;
    else
    	free(msg);
}

void FLOPPYResult::print() {
    
    
    switch (type()) {
        case FLOPPYResultType::ErrorType:
            //printf("ErrorType\n");
            printf("ERROR - %s\n", msg);
            break;
        case FLOPPYResultType::SelectType:
            //printf("SelectType\n");
            recordSet->print();
            break;
        case FLOPPYResultType::MessageType:
            //printf("MessageType\n");
            printf("%s\n", msg);
            break;
    }
}
