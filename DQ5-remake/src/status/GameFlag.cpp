#include "status/GameFlag.h"

status::GameFlag status::g_GlobalFlag;


bool status::GameFlag::check(GameFlag* self, uint32_t  index)
{
	return self->flag_[index / 32].test(index % 32);

}

status::GameFlag::GameFlag()
{
	memset(this, 0, sizeof(status::GameFlag));
}

void status::GameFlag::clear(GameFlag* self)
{
	memset(self, 0, sizeof(status::GameFlag));
}

void status::GameFlag::initialize(GameFlag* self)
{
	memset(self, 0, sizeof(status::GameFlag));
}


void status::GameFlag::operator=(GameFlag* rhs)
{
	for (int i = 0; i != 32; ++i) {
		this->flag_[i].setValue(rhs->flag_[i].get());
	}
}

void status::GameFlag::remove(GameFlag* self, uint32_t index)
{
	self->flag_[index >> 5].clear(index & 0x1F);
}


void status::GameFlag::set(GameFlag* self, uint32_t index)
{
	self->flag_[index >> 5].set(index & 0x1F);
}

status::GameFlag::~GameFlag()
{
	
}
