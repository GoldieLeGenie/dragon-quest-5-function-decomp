#include "status/StageAttribute.h"



status::StageAttribute g_StageAttribute;
char status::StageAttribute::toramana_ = 0;

bool status::StageAttribute::isCarriageEnable(const StageAttribute* self){
    return self->flag_.test(8);
}

bool status::StageAttribute::isCarriageEnter(const StageAttribute* self){
    return self->flag_.test(7);
}


const char* status::StageAttribute::getMapName(const StageAttribute* self){
    return self->map_;
}

int status::StageAttribute::isToramana()
{
    return static_cast<unsigned char>(status::StageAttribute::toramana_);
}


bool status::StageAttribute::isEncount(const StageAttribute* self)
{
    return self->flag_.test(9);
}
