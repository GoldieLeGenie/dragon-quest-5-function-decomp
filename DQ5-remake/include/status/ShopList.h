#pragma once
#include "iostream"

namespace status {
	struct ShopList
	{
		enum class ChurchType : int32_t
		{
			NoExist = 0x0,
			Exist = 0x1,
			NoPray = 0x2,
		};

		~ShopList();
		ShopList();
		static int section_;
		static void initialize(ShopList* self);
		static int getDataIndex(const ShopList* self, int shop);
		static int getShopCount(const status::ShopList* self, int shop);
		static int getShopItem(const ShopList* self, int shop, int index);
		static status::ShopList::ChurchType getChurchType(const ShopList* self, bool second);
		static int getHotelPrice(const ShopList* self, bool second);
		static int getAreaOffset(int area);
		
	};
	extern int ShopDataIndex;
}