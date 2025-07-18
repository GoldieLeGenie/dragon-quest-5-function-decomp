#include "status/StageAttribute.h"



status::StageAttribute g_StageAttribute;

bool status::StageAttribute::isCarriageEnable(const StageAttribute* self){
    return (self->flag_.get() >> 8) & 1;
}

bool status::StageAttribute::isCarriageEnter(const StageAttribute* self){
    return (self->flag_.get() >> 7) & 1;
}


const char* status::StageAttribute::getMapName(const StageAttribute* self){
    return self->map_;
}