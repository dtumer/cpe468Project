#include "FLOPPYTableAttribute.h"
#include <stdlib.h>

FLOPPYTableAttribute::FLOPPYTableAttribute() {

}

FLOPPYTableAttribute::~FLOPPYTableAttribute() {
    free(tableName);
    free(attribute);
}
