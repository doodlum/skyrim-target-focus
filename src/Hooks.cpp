#include "Hooks.h"
#include "DOFManager.h"

namespace Hooks
{
	void Hooks::Install()
	{
	    DOFManager::InstallHooks();
		logger::info("Installed all hooks");
	}
}
