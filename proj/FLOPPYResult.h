//
//  FLOPPYResult.h
//

#ifndef FLOPPYResult_h
#define FLOPPYResult_h

#include "FLOPPYRecordSet.h"
#include <string>
#include <stdio.h>
#include <iostream>

typedef enum {
    ErrorType,
    SelectType,
    MessageType
} FLOPPYResultType;

class FLOPPYResult {
public:
    FLOPPYResult(FLOPPYResultType type);
    ~FLOPPYResult();
    
    virtual FLOPPYResultType type() {
        return _type;
    }
    
    void print();
    
    union {
        char *msg;
        FLOPPYRecordSet *recordSet;
    };
    
    FLOPPYResultType _type;
};

#endif /* FLOPPYResult_h */
