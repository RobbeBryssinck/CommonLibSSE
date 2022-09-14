#include "RE/P/Projectile.h"

namespace RE
{
	Projectile::Manager* Projectile::Manager::GetSingleton()
	{
		REL::Relocation<Projectile::Manager**> singleton{ RELOCATION_ID(514313, 400473) };
		return *singleton;
	}

	BSPointerHandle<Projectile>* Projectile::Launch(BSPointerHandle<Projectile>* a_result, LaunchData& a_data) noexcept
	{
		using func_t = decltype(&Projectile::Launch);
		REL::Relocation<func_t> func{ RELOCATION_ID(42928, 44108) };
		return func(a_result, a_data);
	}
}
