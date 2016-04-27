#include "read_storage.h"

void reset(Triplet& t){

    t.is_filled.reset();
    t.original.reset();
    t.corrected.reset();
    t.reference.reset();
}
