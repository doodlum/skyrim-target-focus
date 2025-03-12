#pragma once
// Thanks Ershin!
RE::NiPoint3 GetCameraPos()
{
	static auto         player = RE::PlayerCharacter::GetSingleton();
	static auto playerCamera = RE::PlayerCamera::GetSingleton();
	RE::NiPoint3 ret;
	if (playerCamera->currentState == playerCamera->GetRuntimeData().cameraStates[RE::CameraStates::kFirstPerson] ||
		playerCamera->currentState == playerCamera->GetRuntimeData().cameraStates[RE::CameraStates::kThirdPerson] ||
		playerCamera->currentState == playerCamera->GetRuntimeData().cameraStates[RE::CameraStates::kMount]) {
		RE::NiNode* root = playerCamera->cameraRoot.get();
		if (root) {
			ret.x = root->world.translate.x;
			ret.y = root->world.translate.y;
			ret.z = root->world.translate.z;
		}
	} else {
		RE::NiPoint3 playerPos = player->GetLookingAtLocation();

		ret.z = playerPos.z;
		ret.x = player->GetPositionX();
		ret.y = player->GetPositionY();
	}

	return ret;
}
