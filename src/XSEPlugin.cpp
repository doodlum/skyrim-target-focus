#include "DOFManager.h"

TDM_API::IVTDM2*     g_TDM = nullptr;
ENB_API::ENBSDK1001* g_ENB = nullptr;

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kPostLoad:

		g_TDM = reinterpret_cast<TDM_API::IVTDM2*>(TDM_API::RequestPluginAPI(TDM_API::InterfaceVersion::V2));
		if (g_TDM)
			logger::info("Obtained TDM API");
		else
			logger::info("Unable to acquire TDM API");

		g_ENB = reinterpret_cast<ENB_API::ENBSDK1001*>(ENB_API::RequestENBAPI(ENB_API::SDKVersion::V1001));
		if (g_ENB) {
			logger::info("Obtained ENB API");
			g_ENB->SetCallbackFunction([](ENBCallbackType calltype) {
				if (calltype == ENBCallbackType::ENBCallback_EndFrame)
					DOFManager::GetSingleton()->UpdateENBParams();
			});
		} else
			logger::info("Unable to acquire ENB API");

		break;
	}
}

void Load()
{
	SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);
	logger::info("Registered for SKSE messages");
	DOFManager::InstallHooks();
	logger::info("Installed all hooks");
}
