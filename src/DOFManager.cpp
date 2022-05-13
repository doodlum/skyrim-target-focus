#include "DOFManager.h"
#include "Util.h"

bool DOFManager::GetTargetLockEnabled()
{
	return g_TDM && g_TDM->GetCurrentTarget();
}

float DOFManager::GetDistanceToTarget()
{
	if (GetTargetLockEnabled()) {
		RE::TESObjectREFR* target = g_TDM->GetCurrentTarget().get().get();
		RE::NiPoint3 cameraPosition = GetCameraPos();
		RE::NiPoint3 targetPosition = target->GetPosition();
		double distance = cameraPosition.GetDistance(targetPosition) / 70.0280112 / 1000;
		return static_cast<float>(distance);
	}

	return targetFocusDistance;
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

	memcpy(param.Data, &targetFocusDistance, param.Size);
	g_ENB->SetParameter(NULL, "ENBDEPTHOFFIELD.FX", "Target Focus Distance", &param);

	memcpy(param.Data, &targetFocusPercent, param.Size);
	g_ENB->SetParameter(NULL, "ENBDEPTHOFFIELD.FX", "Target Focus Percent", &param);
}

void DOFManager::UpdateDOF(float delta)
{
	targetFocusEnabled = GetTargetLockEnabled();
	targetFocusDistance = GetDistanceToTarget();
	targetFocusPercent = std::lerp(targetFocusPercent, targetFocusEnabled, delta);
}
