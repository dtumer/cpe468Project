//
//  FLOPPYResult.h
//

#ifndef FLOPPYResult_h
#define FLOPPYResult_h

#include <string>
#include <stdio.h>
#include <iostream>

typedef enum {
    ErrorType,
    SelectType,
    UpdateType,
    InsertType,
    MessageType
} FLOPPYResultType;

class FLOPPYResult {
public:
    FLOPPYResult(FLOPPYResultType type);
    ~FLOPPYResult();
    
    virtual FLOPPYResultType type() {
        return _type;
    }
    
    union {
        char *msg;
    };
    
    FLOPPYResultType _type;
};

#endif /* FLOPPYResult_h */
