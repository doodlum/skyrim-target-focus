#pragma once

#define TDM_API_COMMONLIB
#include "TDM/TrueDirectionalMovementAPI.h"

#include "ENB/ENBSeriesAPI.h"

class DOFManager
{
public:
	[[nodiscard]] static DOFManager* GetSingleton()
	{
		static DOFManager singleton;
		return std::addressof(singleton);
	}

	static void InstallHooks()
	{
		Hooks::Install();
	}

	bool  targetFocusEnabled = false;
	float targetFocusDistanceGame = 1000.0;
	float targetFocusDistanceENB = 3000.0;
	float targetFocusPercent = 0.00;

	bool  GetTargetLockEnabled();
	float GetDistanceToLockedTarget();

	bool  GetInDialogue();
	float GetDistanceToDialogueTarget();

	void UpdateENBParams();
	void UpdateDOF(float a_delta);

protected:
	struct Hooks
	{
		struct PlayerCharacter_Update
		{
			static void thunk(RE::PlayerCharacter* a_player, float a_delta)
			{
				func(a_player, a_delta);
				GetSingleton()->UpdateDOF(a_delta);
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
