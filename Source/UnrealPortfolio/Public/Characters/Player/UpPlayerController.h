// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/PlayerController.h"
#include "Utils/Enums.h"
#include "UpPlayerController.generated.h"

class AUpPlayableNpc;
class ADefaultPawn;
class AUpHud;
class AUpPlayableCharacter;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUpPlayerController();

	void EnableMouse();
	void DisableMouse();
	bool IsDebugCameraActive() const;
	bool ProjectReticleToWorld(FVector& WorldPosition, FVector& WorldDirection) const;
	
	void CloseCharacterSwitcher();
	void SwitchCharacter(AUpPlayableNpc* Npc);
	
	void ActivateInputMappingContext(const UInputMappingContext* InputMappingContext, const bool bClearExisting = true, const int32 Priority = 0) const;
	void DeactivateInputMappingContext(const UInputMappingContext* InputMappingContext) const;
	void ResetInputMappingContexts() const;

	void CreateTemporaryCamera(const AActor* LookTarget, const float CameraBlendTime, const float AspectRatio, const float FieldOfView);
	void DestroyTemporaryCamera(const float CameraBlendTime);
	void SetCameraView(const EUpCameraView::Type InCameraView) { CameraView = InCameraView; }
	void SetTargetCameraFov(const float InTargetCameraFov) { TargetCameraFov = InTargetCameraFov; }
	
	FORCEINLINE UInputAction* GetInputAction_CloseCharacterSwitcher() const { return InputAction_CloseCharacterSwitcher; }
	
	FORCEINLINE AActor* GetActiveInteractable() const { return ActiveInteractable; }
	FORCEINLINE TEnumAsByte<EUpCameraView::Type> GetCameraView() const { return CameraView; }
	FORCEINLINE AUpHud* GetCustomHud() const { return CustomHud; }
	FORCEINLINE AUpPlayableCharacter* GetPossessedCharacter() const { return PossessedCharacter; }
	FORCEINLINE bool IsInitialized() const { return bInitialized; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Mapping Contexts")
	TObjectPtr<UInputMappingContext> InputMappingContext_Base;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Mapping Contexts")
	TObjectPtr<UInputMappingContext> InputMappingContext_CharacterSwitcher;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Mapping Contexts")
	TObjectPtr<UInputMappingContext> InputMappingContext_Gun;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Mapping Contexts")
	TObjectPtr<UInputMappingContext> InputMappingContext_InteractionOnly;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_ToggleDebugCamera;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_Move;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_Sprint;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_Look;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_SwitchCameraView;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_Jump;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_Reload;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_Interact;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_Weapon1;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_Weapon2;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_Character1;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_Character2;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_AimWeapon;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_FireWeapon;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_CloseCharacterSwitcher;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_Crouch;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_NavigateCharacterSwitcher;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_OpenCharacterSwitcher;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InputAction_PauseGame;
	
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugAimAssist = false;
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugInputMappingContexts = false;
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugPossession = false;

	UPROPERTY(EditAnywhere, Category="UP Params")
	float InterpSpeed_CameraFov = 5.f;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	float LookInputMultiplier_Aiming = 0.5f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float LookInputMultiplier_AimAssist = 0.5f;
	
	UPROPERTY(Transient)
	TObjectPtr<AActor> ActiveInteractable;
	UPROPERTY(Transient)
	TEnumAsByte<EUpAimAssistLevel::Type> AimAssistLevel = EUpAimAssistLevel::None;
	UPROPERTY(Transient)
	TEnumAsByte<EUpCameraView::Type> CameraView = EUpCameraView::FirstPerson;
	UPROPERTY(Transient)
	TObjectPtr<AUpHud> CustomHud;
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayableCharacter> PossessedCharacter;
	UPROPERTY(Transient)
	TObjectPtr<ACameraActor> TemporaryCamera;
	
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayableCharacter> DebugCharacter;
	UPROPERTY(Transient)
	TObjectPtr<ADefaultPawn> DebugPawn;

	bool bInitialized = false;
	float TargetCameraFov = -1.f;
	
	FGameplayTag PostPossessionGameplayEventTag = FGameplayTag::EmptyTag;
	FGameplayEventData PostPossessionGameplayEventData;
	
	void ToggleDebugCamera(const FInputActionValue& InputActionValue);
	
	void Move(const FInputActionValue& InputActionValue);
	void HandleGroundMovement(const FInputActionValue& InputActionValue);
	void HandleClimbingMovement(const FInputActionValue& InputActionValue);
	
	void StartSprint(const FInputActionValue& InputActionValue);
	void StopSprint(const FInputActionValue& InputActionValue);
	
	void Look(const FInputActionValue& InputActionValue);
	
	void SwitchCameraView(const FInputActionValue& InputActionValue);
	
	void Jump(const FInputActionValue& InputActionValue);
	
	void Reload(const FInputActionValue& InputActionValue);
	
	void StartInteraction(const FInputActionValue& InputActionValue);
	void EndInteraction(const FInputActionValue& InputActionValue);
	
	void ToggleWeapon1(const FInputActionValue& InputActionValue);
	void ToggleWeapon2(const FInputActionValue& InputActionValue);
	void ToggleWeapon(const EUpEquipmentSlot::Type EquipmentSlot) const;
	
	void SwitchToCharacter1(const FInputActionValue& InputActionValue);
	void SwitchToCharacter2(const FInputActionValue& InputActionValue);
	
	void StartAimingWeapon(const FInputActionValue& InputActionValue);
	void StopAimingWeapon(const FInputActionValue& InputActionValue);
	
	void StartFiringWeapon(const FInputActionValue& InputActionValue);
	void StopFiringWeapon(const FInputActionValue& InputActionValue);
	
	void PauseGame(const FInputActionValue& InputActionValue);
	
	void OpenCharacterSwitcher(const FInputActionValue& InputActionValue);
	void TriggerCloseCharacterSwitcher(const FInputActionValue& InputActionValue);
	void NavigateCharacterSwitcher(const FInputActionValue& InputActionValue);
	
	void ToggleCrouch(const FInputActionValue& InputActionValue);
};
