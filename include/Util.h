#pragma once
// Thanks Ershin!
RE::NiPoint3 GetCameraPos()
{
	auto         player = RE::PlayerCharacter::GetSingleton();
	auto         playerCamera = RE::PlayerCamera::GetSingleton();
	RE::NiPoint3 ret;

	if (playerCamera->currentState == playerCamera->cameraStates[RE::CameraStates::kFirstPerson] ||
		playerCamera->currentState == playerCamera->cameraStates[RE::CameraStates::kThirdPerson] ||
		playerCamera->currentState == playerCamera->cameraStates[RE::CameraStates::kMount]) {
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
