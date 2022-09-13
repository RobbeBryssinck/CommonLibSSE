#pragma once

#include "RE/B/BSExtraData.h"
#include "RE/E/ExtraDataTypes.h"

namespace RE
{
	class SeenData
	{
	public:
		virtual		 ~SeenData();									// 00
		virtual void SaveGame(BGSSaveFormBuffer* a_saveGameBuffer); // 01
		virtual void LoadGame(BGSLoadFormBuffer* a_loadGameBuffer); // 02

		std::uint32_t seenBits[8]; // 08
	};
	static_assert(sizeof(SeenData) == 0x28);

	class IntSeenData : SeenData
	{
	public:
		~IntSeenData() override;  // 00

		char		 sectionX;	  // 28
		char		 sectionY;	  // 29
		IntSeenData* nextSection; // 30
	};
	static_assert(sizeof(IntSeenData) == 0x38);

	class ExtraSeenData : public BSExtraData
	{
	public:
		inline static constexpr auto RTTI = RTTI_ExtraSeenData;
		inline static constexpr auto EXTRADATATYPE = ExtraDataType::kSeenData;

		~ExtraSeenData() override;  // 00

		// override (BSExtraData)
		[[nodiscard]] ExtraDataType GetType() const override;  // 01 - { return kSeenData; }

		// members
		SeenData* seenData;  // 10
	};
	static_assert(sizeof(ExtraSeenData) == 0x18);
}
