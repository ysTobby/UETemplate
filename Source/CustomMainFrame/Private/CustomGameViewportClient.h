#pragma once
#include "Engine/GameViewportClient.h"
#include "CustomGameViewportClient.generated.h"

UCLASS()
class UCustomGameViewportClient : public UGameViewportClient
{
	GENERATED_UCLASS_BODY()
//public:
//	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice) override;
//	virtual void Draw(FViewport* Viewport, FCanvas* SceneCanvas) override;
//	virtual void MouseMove(FViewport* InViewport, int32 X, int32 Y) override;
//	virtual void CapturedMouseMove(FViewport* InViewport, int32 InMouseX, int32 InMouseY) override;
//	virtual void FinalizeViews(class FSceneViewFamily* ViewFamily, const TMap<ULocalPlayer*, FSceneView*>& PlayerViewMap) override;
//	virtual bool InputKey(const FInputKeyEventArgs& EventArgs) override;
//	virtual bool InputAxis(FViewport* InViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad) override;
//	virtual bool RequiresHitProxyStorage() override { return true; }
//	virtual EMouseCursor::Type GetCursor(FViewport* Viewport, int32 X, int32 Y) override;
//	void LockCamera(bool bLockCamera) { bCameraLocked = bLockCamera; }
};