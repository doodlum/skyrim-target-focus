#pragma once

#define TDM_API_COMMONLIB
#include "API/TrueDirectionalMovementAPI.h"

#include "API/ENBSeriesAPI.h"


class DOFManager
{
public:

	TDM_API::IVTDM2*	 g_TDM = nullptr;
	ENB_API::ENBSDK1001* g_ENB = nullptr;
	
	[[nodiscard]] static DOFManager* GetSingleton()
	{
		static DOFManager singleton;
		return std::addressof(singleton);
	}

	static void InstallHooks()
	{
		Hooks::Install();

	}

	bool    targetFocusEnabled = false;
	float	targetFocusDistance = 3000.0; 
	float   targetFocusPercent = 0.00;

	bool	GetTargetLockEnabled();
	float	GetDistanceToTarget();

	void	UpdateENBParams();     
	void    UpdateDOF(float delta);  


protected:

	struct Hooks
	{
		struct PlayerCharacter_Update
		{
			static void thunk(RE::PlayerCharacter* player, float delta)
			{
				func(player, delta);
				GetSingleton()->UpdateDOF(delta);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};


		static void Install()
		{
			stl::write_vfunc<RE::PlayerCharacter, 0xAD, PlayerCharacter_Update>();
		}
	};


private:
	constexpr DOFManager() noexcept = default;
	DOFManager(const DOFManager&) = delete;
	DOFManager(DOFManager&&) = delete;

	~DOFManager() = default;

	DOFManager& operator=(const DOFManager&) = delete;
	DOFManager& operator=(DOFManager&&) = delete;

};