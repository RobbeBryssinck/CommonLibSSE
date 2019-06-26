#pragma once

#include <type_traits>  // underlying_type_t

#include "RE/BSTEvent.h"  // BSTEventSource
#include "RE/BSFixedString.h"  // BSFixedString
#include "RE/BSInputDevice.h"  // BSInputDevice
#include "RE/BSTArray.h"  // BSTArray
#include "RE/BSTSingleton.h"  // BSTSingletonSDM
#include "RE/DeviceTypes.h"  // DeviceType


namespace RE
{
	class InputEvent;


	class InputMappingManager :
		public BSTSingletonSDM<InputMappingManager>,
		public BSTEventSource<InputEvent*>
	{
	public:
		struct Contexts
		{
			enum Context : UInt32
			{
				kGameplay = 0,
				kMenuMode,
				kConsole,
				kItemMenu,
				kInventory,
				kDebugText,
				kFavorites,
				kMap,
				kStats,
				kCursor,
				kBook,
				kDebugOverlay,
				kJournal,
				kTFCMode,
				kMapDebug,
				kLockpicking,
				kFavor,

				kTotal = 17,

				kInvalid = 18
			};
		};
		using Context = Contexts::Context;


		enum class ControlState : UInt32
		{
			kInvalid = static_cast<std::underlying_type_t<ControlState>>(-1),
			kNone = 0,
			kLooking = 1 << 1,
			kFlying = 1 << 6,
			kSneaking = 1 << 7,
			kMenu = 1 << 8,
			kMovement = 1 << 10 | 1 << 0
		};


		enum : UInt32 { kInvalid = static_cast<UInt32>(-1) };


		struct InputContext
		{
			struct Mapping
			{
				BSFixedString	name;		// 00 - User Event Name
				UInt16			buttonID;	// 08
				UInt16			modifier;	// 0A
				UInt32			sortIndex;	// 0C
				UInt32			flags;		// 10 - User Event Binary Flag
				UInt32			pad14;		// 14
			};
			STATIC_ASSERT(sizeof(Mapping) == 0x18);


			BSTArray<Mapping>	keyboardMap;	// 00
			BSTArray<Mapping>	mouseMap;		// 18
			BSTArray<Mapping>	gamepadMap;		// 30
		};
		STATIC_ASSERT(sizeof(InputContext) == 0x48);


		static InputMappingManager*	GetSingleton();

		UInt8						AllowTextInput(bool a_allow);
		UInt32						GetMappedKey(const BSFixedString& a_name, DeviceType a_deviceType, Context a_contextIdx = Context::kGameplay) const;
		const BSFixedString&		GetUserEventName(UInt32 a_buttonID, DeviceType a_deviceType, Context a_contextIdx = Context::kGameplay) const;
		bool						IsLookingControlsEnabled() const;
		bool						IsFlyingControlsEnabled() const;
		bool						IsSneakingControlsEnabled() const;
		bool						IsMenuControlsEnabled() const;
		bool						IsMovementControlsEnabled() const;


		// members
		InputContext*	context[Context::kTotal];	// 060
		BSTArray<void*>	unk0E8;						// 0E8
		BSTArray<void*>	unk100;						// 100
		ControlState	controlState;				// 118 - init'd to kInvalid
		UInt32			unk11C;						// 11C - init'd to 0x80000000
		UInt8			allowTextInput;				// 120 - init'd to 0
		UInt8			unk121;						// 121 - init'd to 0
		UInt8			unk122;						// 122 - init'd to 0
		UInt8			pad[5];						// 123
	};
	STATIC_ASSERT(offsetof(InputMappingManager, context) == 0x060);
	STATIC_ASSERT(sizeof(InputMappingManager) == 0x128);
}
