// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UpPlayerController.generated.h"

class ADefaultPawn;
class AUpHud;
class AUpPlayableCharacter;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UENUM(BlueprintType)
namespace EUpPlayerCameraViewType
{
	enum Type : uint8
	{
		FirstPerson,
		ThirdPerson,
		ThirdPerson_OverTheShoulder
	};
}

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUpPlayerController();
	
	void Init();
	void CloseCharacterSwitcher();
	void EnableMouse();
	void DisableMouse();
	
	void SetCurrentCameraViewType(const EUpPlayerCameraViewType::Type InCameraViewType) { CurrentCameraViewType = InCameraViewType; }
	
	FORCEINLINE TEnumAsByte<EUpPlayerCameraViewType::Type> GetCurrentCameraViewType() const { return CurrentCameraViewType; }
	FORCEINLINE AUpHud* GetCustomHud() const { return CustomHud; }
	FORCEINLINE UInputAction* GetOpenCharacterSwitcherInputAction() const { return OpenCharacterSwitcherInputAction; }
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

	UPROPERTY(Transient)
	TObjectPtr<AUpHud> CustomHud;
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayableCharacter> PossessedCharacter;
	
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayableCharacter> DebugCharacter;
	UPROPERTY(Transient)
	TObjectPtr<ADefaultPawn> DebugPawn;
	
	TEnumAsByte<EUpPlayerCameraViewType::Type> CurrentCameraViewType = EUpPlayerCameraViewType::ThirdPerson;
	bool bInitialized = false;
	
	void ToggleCameraView(const FInputActionValue& InputActionValue);
	void ToggleDebugCamera(const FInputActionValue& InputActionValue);
	
	void PauseGame(const FInputActionValue& InputActionValue);
	void Interact(const FInputActionValue& InputActionValue);
	
	void OpenCharacterSwitcher(const FInputActionValue& InputActionValue);
	void NavigateCharacterSwitcher(const FInputActionValue& InputActionValue);

	void ToggleCrouch(const FInputActionValue& InputActionValue);
	void Jump(const FInputActionValue& InputActionValue);

	void StartSprint(const FInputActionValue& InputActionValue);
	void StopSprint(const FInputActionValue& InputActionValue);
	
	void Look(const FInputActionValue& InputActionValue);
	void Move(const FInputActionValue& InputActionValue);

	void ActivateInputMappingContext(const UInputMappingContext* InputMappingContext, const bool bClearExisting = true, const int32 Priority = 0) const;
	void DeactivateInputMappingContext(const UInputMappingContext* InputMappingContext) const;
};
