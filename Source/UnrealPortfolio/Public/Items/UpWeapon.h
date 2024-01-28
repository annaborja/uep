// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "UpItem.h"
#include "GAS/UpAbilitySystemComponent.h"
#include "UpWeapon.generated.h"

class USoundCue;
class UUpAmmoAttributeSet;

UCLASS()
class UNREALPORTFOLIO_API AUpWeapon : public AUpItem, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AUpWeapon();

	virtual FUpInteractionData GetInteractionData(const AUpPlayerController* PlayerController) override;
	virtual void Interact(AUpPlayerController* PlayerController) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	virtual EUpCharacterPosture::Type GetCharacterPosture() const override;
	
	float CalculateDamage() const;
	float GetAmmoGrantAmount() const;
	float GetMagazineCapacity() const;
	float GetMaxAmmo() const;
	float GetBurstInterval() const;
	float GetBurstShotInterval() const;
	int32 GetBurstSize() const;
	float GetRange() const;

	FString GetWeaponTypeNameSectionString() const;
	EUpInGameWeaponType::Type GetInGameWeaponType() const;
	bool IsAmmoCompatible(const AUpWeapon* OtherWeapon) const;
	bool IsPistolType() const;
	bool IsRifleType() const;

	FORCEINLINE UUpAmmoAttributeSet* GetAmmoAttributeSet() const { return AmmoAttributeSet; }
	FORCEINLINE EUpWeaponFiringMode::Type GetFiringMode() const { return WeaponData.FiringMode; }
	FORCEINLINE UNiagaraSystem* GetNiagaraSystem_Impact() const { return WeaponData.NiagaraSystem_Impact; }
	FORCEINLINE FUpWeaponData GetWeaponData() const { return WeaponData; }
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> EffectClass_InitAmmo;
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TObjectPtr<USoundCue> Sfx_AmmoGrant;
	
	UPROPERTY()
	TObjectPtr<UUpAmmoAttributeSet> AmmoAttributeSet;

	FUpWeaponData WeaponData;
	
	virtual void BeginPlay() override;

	void OnAmmoGrantSuccess();
};
