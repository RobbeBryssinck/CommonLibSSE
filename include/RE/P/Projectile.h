#pragma once

#include "RE/B/BGSProjectile.h"
#include "RE/B/BSAtomic.h"
#include "RE/B/BSPointerHandle.h"
#include "RE/B/BSSoundHandle.h"
#include "RE/B/BSTList.h"
#include "RE/C/CollisionLayers.h"
#include "RE/F/FormTypes.h"
#include "RE/I/ImpactResults.h"
#include "RE/N/NiSmartPointer.h"
#include "RE/N/NiTransform.h"
#include "RE/T/TESObjectREFR.h"
#include "RE/B/BSTSingleton.h"

namespace RE
{
	class bhkCollisionObject;
	class bhkShape;
	class bhkSimpleShapePhantom;
	class BGSMaterialType;
	class QueuedFile;
	class CombatController;
	class MagicItem;

	class Projectile : public TESObjectREFR
	{
	public:
		inline static constexpr auto RTTI = RTTI_Projectile;

		struct WobbleControl
		{
		public:
			// members
			uint8_t unk00[0x2C];
		};
		static_assert(sizeof(WobbleControl) == 0x2C);

		class Manager : public BSTSingletonSDM<Manager>
		{
		public:
			static Manager* GetSingleton();

			// members
			BSTArray<ProjectileHandle> unlimited;	   // 08
			BSTArray<ProjectileHandle> limited;		   // 20
			BSTArray<ProjectileHandle> pending;		   // 38
			BSSpinLock                 projectileLock; // 50
			BSTArray<WobbleControl>    wobble;         // 58
		};

		struct LaunchData
		{
			virtual ~LaunchData();

			// members
			NiPoint3					origin;					// 08
			NiPoint3					contactNormal;			// 14
			BGSProjectile*				projectileBase;			// 20
			TESObjectREFR*				shooter;				// 28
			CombatController*			combatController;		// 30
			TESObjectWEAP*				fromWeapon;				// 38
			TESAmmo*					fromAmmo;				// 40
			float						zAngle;					// 48
			float						xAngle;					// 4C
			float						yAngle;					// 50
			std::uint8_t				unk54[0x14];			// 5C
			TESObjectCELL*				parentCell;				// 70
			MagicItem*					spell;					// 78
			MagicSystem::CastingSource	castingSource;			// 80
			bool						unkBool1;				// 84
			std::uint8_t				unk7D[0xB];				// 85
			AlchemyItem*				poison;					// 90
			std::int32_t				area;					// 98
			float						power;					// 9C
			float						scale;					// A0
			bool						bAlwaysHit;				// A4
			bool						bNoDamageOutsideCombat; // A5
			bool						bAutoAim;				// A6
			bool						bUnkBool2;				// A7
			bool						bUseOrigin;				// A8
			bool						bDeferInitialization;	// A9
			bool						bForceConeOfFire;		// AA
		};
		static_assert(sizeof(LaunchData) == 0xA8);

		struct ImpactData
		{
		public:
			// members
			NiPoint3                                  desiredTargetLoc;    // 00
			NiPoint3                                  negativeVelocity;    // 0C
			ObjectRefHandle                           collidee;            // 18
			NiPointer<bhkCollisionObject>             colObj;              // 20
			BGSMaterialType*                          material;            // 28
			std::int32_t                              damageRootNodeType;  // 30
			stl::enumeration<COL_LAYER, std::int32_t> collidedLayer;       // 34
			NiNode*                                   damageRootNode;      // 38
			ImpactResult                              impactResult;        // 40
			std::uint16_t                             unk44;               // 44
			std::uint16_t                             unk46;               // 46
			std::uint8_t                              unk48;               // 48
			std::uint8_t                              unk49;               // 49
		};
		static_assert(sizeof(ImpactData) == 0x50);

		~Projectile() override;  // 00

		// override (TESObjectREFR)
		bool         Load(TESFile* a_mod) override;                                         // 06 - { return TESObjectREFR::Load(a_mod); }
		void         SaveGame(BGSSaveFormBuffer* a_buf) override;                           // 0E
		void         LoadGame(BGSLoadFormBuffer* a_buf) override;                           // 0F
		void         InitLoadGame(BGSLoadFormBuffer* a_buf) override;                       // 10
		void         FinishLoadGame(BGSLoadFormBuffer* a_buf) override;                     // 11
		void         Revert(BGSLoadFormBuffer* a_buf) override;                             // 12
		bool         GetAllowPromoteToPersistent() const override;                          // 47 - { return false; }
		bool         HasKeywordHelper(const BGSKeyword* a_keyword) const override;          // 48
		void         SetActorCause(ActorCause* a_cause) override;                           // 50 - { actorCause = a_cause; }
		ActorCause*  GetActorCause() const override;                                        // 51 - { return actorCause; }
		MagicCaster* GetMagicCaster(MagicSystem::CastingSource a_source) override;          // 5C
		bool         DetachHavok(NiAVObject* a_obj3D) override;                             // 65
		void         InitHavok() override;                                                  // 66
		NiAVObject*  Load3D(bool a_backgroundLoading) override;                             // 6A
		void         Set3D(NiAVObject* a_object, bool a_queue3DTasks = true) override;      // 6C
		void         MoveHavok(bool a_forceRec) override;                                   // 85 - { return; }
		void         GetLinearVelocity(NiPoint3& a_velocity) const override;                // 86
		NiNode*      GetFireNode() override;                                                // 8B
		Projectile*  AsProjectile() override;                                               // 8F - { return this; }
		bool         OnAddCellPerformQueueReference(TESObjectCELL& a_cell) const override;  // 90 - { return false; }

		// add
		virtual bool          IsMissileProjectile();                                                                                                                                             // A2 - { return 0; }
		virtual bool          IsGrenadeProjectile();                                                                                                                                             // A3 - { return 0; }
		virtual bool          IsFlameProjectile();                                                                                                                                               // A4 - { return 0; }
		virtual bool          IsBeamProjectile();                                                                                                                                                // A5 - { return 0; }
		virtual void          Unk_A6(void);                                                                                                                                                      // A6 - { return 0; }
		virtual bool          IsBarrierProjectile();                                                                                                                                             // A7 - { return 0; }
		virtual void          OnKill();                                                                                                                                                          // A8 - { return; }
		virtual void          Process3D();                                                                                                                                                       // A9 - { return; }
		virtual void          PostLoad3D(NiAVObject* a_root);                                                                                                                                    // AA
		virtual void          UpdateImpl(float a_delta) = 0;                                                                                                                                     // AB
		virtual bool          ProcessImpacts();                                                                                                                                                  // AC
		virtual void          Update3D();                                                                                                                                                        // AD
		virtual void          Unk_AE(void);                                                                                                                                                      // AE - { return 0; }
		virtual float         GetPowerSpeedMult() const;                                                                                                                                         // AF - { if (unk158) return 1.0; else return unk188; } - "float GetSpeed()"?
		virtual float         GetWeaponSpeedMult() const;                                                                                                                                        // B0 - { return 1.0; }
		virtual bool          GetStopMainSoundAfterImpact();                                                                                                                                     // B1 - { return 0; }
		virtual void          ReportHavokDeactivation();                                                                                                                                         // B2 - { return; }
		virtual bool          TurnOff(Actor* a_owner, bool a_noDeactivateSound);                                                                                                                 // B3
		virtual bool          IsPermanent() const;                                                                                                                                               // B4 - { return TESDataHandler::GetSingleton()->IsGeneratedFormID(formID) == 0; }
		virtual float         GetGravity();                                                                                                                                                      // B5 - { void* var = unk40; if ((var->unk80 >> 17) & 1) return 1.0; else return var->unk84; }
		virtual void          CleanUpPointersOnDisable();                                                                                                                                        // B6
		virtual bool          RunTargetPick();                                                                                                                                                   // B7
		virtual bool          GetKillOnCollision();                                                                                                                                              // B8 - { return 1; }
		virtual bool          ShouldBeLimited();                                                                                                                                                 // B9 - { return 0; }
		virtual bool          TargetsWholeBody();                                                                                                                                                // BA - { return 0; }
		virtual std::uint32_t GetCollisionGroup();                                                                                                                                               // BB
		virtual bhkShape*     GetCollisionShape();                                                                                                                                               // BC
		virtual void          AddImpact(TESObjectREFR* a_ref, const NiPoint3& a_targetLoc, const NiPoint3& a_velocity, hkpCollidable* a_collidable, std::int32_t a_arg6, std::uint32_t a_arg7);  // BD
		virtual bool          HandleHits(hkpCollidable* a_collidable);                                                                                                                           // BE
		virtual void          OnTriggerEnter();                                                                                                                                                  // BF - { return; }
		virtual void          Handle3DLoaded();                                                                                                                                                  // C0 - { return; }
		virtual bool          ShouldUseDesiredTarget();                                                                                                                                          // C1 - { return 0; }

		inline float GetHeight() const
		{
			auto obj = GetObjectReference();
			auto projectile = obj ? obj->As<BGSProjectile>() : nullptr;
			return projectile ? projectile->data.collisionRadius * 2 : 0.0f;
		}
		inline float GetSpeed() const
		{
			auto obj = GetObjectReference();
			auto projectile = obj ? obj->As<BGSProjectile>() : nullptr;
			if (!projectile)
				return 0.0f;

			return projectile->data.speed * GetPowerSpeedMult() * GetWeaponSpeedMult() * speedMult;
		}

		// Has been extensively tested in Skyrim Together, can leave the unknown fields null.
		static BSPointerHandle<Projectile>* Launch(BSPointerHandle<Projectile>* a_result, LaunchData& a_data) noexcept;

		// members
		BSSimpleList<ImpactData*>  impacts;            // 098
		NiTransform                unk0A8;             // 0A8
		float                      unk0DC;             // 0DC
		bhkSimpleShapePhantom*     unk0E0;             // 0E0 - smart ptr
		mutable BSSpinLock         unk0E8;             // 0E8
		NiPoint3                   velocity;           // 0F0
		NiPoint3                   linearVelocity;     // 0FC
		void*                      unk108;             // 108 - smart ptr
		void*                      unk110;             // 110 - smart ptr
		NiPointer<ActorCause>      actorCause;         // 118
		ObjectRefHandle            shooter;            // 120
		ObjectRefHandle            desiredTarget;      // 124
		BSSoundHandle              sndHandle;          // 128
		BSSoundHandle              sndCountdown;       // 134
		std::uint32_t*             unk140;             // 140
		InventoryEntryData*        unk148;             // 148
		BGSExplosion*              explosion;          // 150
		MagicItem*                 spell;              // 158
		MagicSystem::CastingSource castingSource;      // 160
		std::uint32_t              pad164;             // 164
		EffectSetting*             avEffect;           // 168
		NiPointer<QueuedFile>      projectileDBFiles;  // 170
		std::uint64_t              unk178;             // 178
		std::uint64_t              unk180;             // 180
		float                      power;              // 188 - 14074B774
		float                      speedMult;          // 18C - 1407501B2
		float                      range;              // 190
		float                      livingTime;         // 194
		float                      weaponDamage;       // 198
		float                      transparency;       // 19C - for beam disappearing
		std::uint64_t              unk1A0;             // 1A0
		float                      unk1A8;             // 1A8
		float                      unk1AC;             // 1AC
		TESObjectWEAP*             weaponSource;       // 1B0
		TESAmmo*                   ammoSource;         // 1B8
		float                      distanceMoved;      // 1C0
		std::uint32_t              unk1C4;             // 1C4 - pad?
		float                      scale;              // 1C8 - for double cast model scale
		std::uint32_t              flags;              // 1CC
		std::uint64_t              unk1D0;             // 1D0
	};
	static_assert(sizeof(Projectile) == 0x1D8);
}
