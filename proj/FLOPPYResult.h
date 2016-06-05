//
//  FLOPPYResult.h
//

#ifndef FLOPPYResult_h
#define FLOPPYResult_h

#include <string>

typedef enum {
    ErrorType,
    SelectType,
    UpdateType,
    InsertType
} FLOPPYResultType;

class FLOPPYResult {
public:
    FLOPPYResult(FLOPPYResultType type);
    virtual ~FLOPPYResult();
    
    virtual FLOPPYResultType type() {
        return _type;
    }
    
    union {
        const char *errorMsg;
    };
    
    FLOPPYResultType _type;
};

#endif /* FLOPPYResult_h */
