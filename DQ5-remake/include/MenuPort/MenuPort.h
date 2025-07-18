#pragma once

struct MenuPort
{
	void* guid;
	short port;
	short flag;
	void* noticeParam;
	void (*noticeCallback)(void*, const short*);
};