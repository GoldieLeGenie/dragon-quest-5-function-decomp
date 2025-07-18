#pragma once
#include "ardq/MENUITEM_DATA.h"
#include "MenuPort/MenuPort.h"

namespace ardq {
	struct MenuItem {
		enum MENUITEM_RESULT : int32_t {
			MENUITEM_RESULT_NONE = 0x0,
			MENUITEM_RESULT_CHANGE = 0x1,
			MENUITEM_RESULT_OK = 0x2,
			MENUITEM_RESULT_CANCEL = 0x3,
			MENUITEM_RESULT_SUPERCANCEL = 0x4,
			MENUITEM_RESULT_UP = 0x5,
			MENUITEM_RESULT_DOWN = 0x6,
			MENUITEM_RESULT_LEFT = 0x7,
			MENUITEM_RESULT_RIGHT = 0x8,
			MENUITEM_RESULT_DA = 0x9,
			MENUITEM_RESULT_DB = 0xA,
			MENUITEM_RESULT_DX = 0xB,
			MENUITEM_RESULT_DY = 0xC,
		};
		enum MENUITEM_TYPE : int32_t {
			MENUITEM_TYPE_TOUCH = 0x0,
			MENUITEM_TYPE_TOUCH_PAD = 0x1,
			MENUITEM_TYPE_TOUCH_CANCEL = 0x2,
			MENUITEM_TYPE_TOUCH_PAD_CANCEL = 0x3,
		};
		enum CURSORTYPE : int32_t {
			CURSORTYPE_NONE = 0x0,
			CURSORTYPE_UP = 0x1,
			CURSORTYPE_DOWN = 0x2,
			CURSORTYPE_LEFT = 0x3,
			CURSORTYPE_RIGHT = 0x4,
			CURSORTYPE_ACTIVE = 0x5,
			CURSORTYPE_INACTIVE = 0x6,
			CURSORTYPE_WIRELESS = 0x7,
		};

		enum MENUITEM_REASON : int32_t {
			MENUITEM_REASON_NONE = 0x0,
			MENUITEM_REASON_PAD = 0x1,
			MENUITEM_REASON_TOUCH = 0x2,
		};

		ardq::MenuItem::MENUITEM_TYPE mtype_;
		ardq::MenuItem::CURSORTYPE ctype_;
		bool enablePad_;
		bool enableCancel_;
		bool enableDirectButton_;
		bool enableLoopEdge_;
		bool enableSE_;
		ardq::MENUITEM_DATA* menuitem_data_;
		int menuitem_temp_x_;
		int menuitem_temp_y_;
		int menuitem_width_;
		int menuitem_height_;
		int menuitem_min_;
		int menuitem_max_;
		int active_;
		ardq::MenuItem::MENUITEM_RESULT lastresult_;
		ardq::MenuItem::MENUITEM_RESULT result_;
		ardq::MenuItem::MENUITEM_REASON reason_;
		MenuPort * m_port;
		short m_unityButton;
		short m_unityPadPress;
		short m_unityPadToggle;
		bool m_bNewType;
		short m_bExecInput2;
		void (*m_unityOtherReceive)(const short*);
        short m_portType;
		bool m_bTermNotClose;
	};
}