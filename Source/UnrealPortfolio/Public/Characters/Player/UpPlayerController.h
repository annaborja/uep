// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
	
	void CloseCharacterSwitcher();
	void SwitchCharacter(AUpPlayableNpc* Npc);
	
	void ActivateInputMappingContext(const UInputMappingContext* InputMappingContext, const bool bClearExisting = true, const int32 Priority = 0) const;
	void DeactivateInputMappingContext(const UInputMappingContext* InputMappingContext) const;
	void ResetInputMappingContexts() const;

	void CreateTemporaryCamera(const AActor* LookTarget, const float CameraBlendTime, const float AspectRatio, const float FieldOfView);
	void DestroyTemporaryCamera(const float CameraBlendTime);
	void SetCameraView(const EUpCameraView::Type InCameraView) { CameraView = InCameraView; }
	
	FORCEINLINE UInputAction* GetCloseCharacterSwitcherInputAction() const { return CloseCharacterSwitcherInputAction; }
	
	FORCEINLINE AActor* GetActiveInteractable() const { return ActiveInteractable; }
	FORCEINLINE TEnumAsByte<EUpCameraView::Type> GetCameraView() const { return CameraView; }
	FORCEINLINE AUpHud* GetCustomHud() const { return CustomHud; }
	FORCEINLINE AUpPlayableCharacter* GetPossessedCharacter() const { return PossessedCharacter; }
	FORCEINLINE bool IsInitialized() const { return bInitialized; }

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Mapping Contexts")
	TObjectPtr<UInputMappingContext> BaseInputMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Mapping Contexts")
	TObjectPtr<UInputMappingContext> CharacterSwitcherInputMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Mapping Contexts")
	TObjectPtr<UInputMappingContext> GunInputMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Mapping Contexts")
	TObjectPtr<UInputMappingContext> InteractionOnlyInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> CloseCharacterSwitcherInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> CrouchInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> InteractInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> JumpInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> LookInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> MoveInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> NavigateCharacterSwitcherInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> OpenCharacterSwitcherInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> PauseGameInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> SprintInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> ToggleCameraViewInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> ToggleDebugCameraInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> Weapon1InputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions")
	TObjectPtr<UInputAction> Weapon2InputAction;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions|Weapon")
	TObjectPtr<UInputAction> AimGunInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions|Weapon")
	TObjectPtr<UInputAction> FireWeaponInputAction;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Input Actions|Weapon")
	TObjectPtr<UInputAction> ReloadInputAction;
	
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugInputMappingContexts = false;
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugPossession = false;
	
	UPROPERTY(Transient)
	TObjectPtr<AActor> ActiveInteractable;
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
	
	void ToggleCameraView(const FInputActionValue& InputActionValue);
	void ToggleDebugCamera(const FInputActionValue& InputActionValue);
	
	void PauseGame(const FInputActionValue& InputActionValue);
	void Reload(const FInputActionValue& InputActionValue);
	
	void StartInteraction(const FInputActionValue& InputActionValue);
	void EndInteraction(const FInputActionValue& InputActionValue);
	
	void OpenCharacterSwitcher(const FInputActionValue& InputActionValue);
	void TriggerCloseCharacterSwitcher(const FInputActionValue& InputActionValue);
	void NavigateCharacterSwitcher(const FInputActionValue& InputActionValue);

	void ToggleWeapon1(const FInputActionValue& InputActionValue);
	void ToggleWeapon2(const FInputActionValue& InputActionValue);
	void ToggleWeapon(const EUpEquipmentSlot::Type EquipmentSlot) const;

	void ToggleCrouch(const FInputActionValue& InputActionValue);
	void Jump(const FInputActionValue& InputActionValue);

	void StartSprint(const FInputActionValue& InputActionValue);
	void StopSprint(const FInputActionValue& InputActionValue);
	
	void Look(const FInputActionValue& InputActionValue);
	void Move(const FInputActionValue& InputActionValue);
	
	void StartAimingGun(const FInputActionValue& InputActionValue);
	void StopAimingGun(const FInputActionValue& InputActionValue);
	void StartFiringWeapon(const FInputActionValue& InputActionValue);
	void StopFiringWeapon(const FInputActionValue& InputActionValue);
};
