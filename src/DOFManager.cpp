#include "DOFManager.h"
#include "Util.h"

bool DOFManager::GetTargetLockEnabled()
{
	return g_TDM && g_TDM->GetCurrentTarget();
}

float DOFManager::GetDistanceToTarget()
{
	RE::TESObjectREFR* target = g_TDM->GetCurrentTarget().get().get();
	RE::NiPoint3 cameraPosition = GetCameraPos();
	RE::NiPoint3 targetPosition = target->GetPosition();
	return cameraPosition.GetDistance(targetPosition);
}

void DOFManager::UpdateENBParams()
{
	if (!g_ENB)
		return;

	ENB_SDK::ENBParameter param;

	param.Type = ENB_SDK::ENBParameterType::ENBParam_BOOL;
	param.Size = ENBParameterTypeToSize(param.Type);

	memcpy(param.Data, &targetFocusEnabled, param.Size);
	g_ENB->SetParameter(NULL, "ENBDEPTHOFFIELD.FX", "Target Focus Enabled", &param);

	param.Type = ENB_SDK::ENBParameterType::ENBParam_FLOAT;
	param.Size = ENBParameterTypeToSize(param.Type);

	memcpy(param.Data, &targetFocusDistanceENB, param.Size);
	g_ENB->SetParameter(NULL, "ENBDEPTHOFFIELD.FX", "Target Focus Distance", &param);

	memcpy(param.Data, &targetFocusPercent, param.Size);
	g_ENB->SetParameter(NULL, "ENBDEPTHOFFIELD.FX", "Target Focus Percent", &param);
}

void DOFManager::UpdateDOF(float delta)
{
	targetFocusEnabled = GetTargetLockEnabled();
	if (GetTargetLockEnabled()) {
		targetFocusDistanceGame = GetDistanceToTarget();
		targetFocusDistanceENB = static_cast<float>(targetFocusDistanceGame / 70.0280112 / 1000);
	}

	targetFocusPercent = std::lerp(targetFocusPercent, targetFocusEnabled, delta);

	if (auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>()) {
		if (auto script = scriptFactory->Create()) {
			script->SetCommand(fmt::format(FMT_STRING("configureddof farblur {}"), targetFocusPercent * 0.8));
			script->CompileAndRun(nullptr);
			script->SetCommand(fmt::format(FMT_STRING("configureddof farrange {}"), max(500, targetFocusDistanceGame)));
			script->CompileAndRun(nullptr);
		}
	}


}
