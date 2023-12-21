// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpGunFireAbility.h"

#include "Kismet/GameplayStatics.h"
#include "Utils/Constants.h"

void UUpGunFireAbility::HandleRepeatAction(int32 ActionNumber)
{
	Super::HandleRepeatAction(ActionNumber);

	if (const auto World = GetWorld())
	{
		FVector2D ViewportSize;
		if (GEngine && GEngine->GameViewport) GEngine->GameViewport->GetViewportSize(ViewportSize);
	
		FVector CrosshairWorldPosition;
		FVector CrosshairWorldDirection;

		if (const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			PlayerController && UGameplayStatics::DeprojectScreenToWorld(PlayerController,
			FVector2D(ViewportSize.X / 2.f, ViewportSize.Y / 2.f), CrosshairWorldPosition, CrosshairWorldDirection))
		{
			const auto LineTraceStart = CrosshairWorldPosition;
			const auto LineTraceEnd = CrosshairWorldPosition + CrosshairWorldDirection * LineTraceLength;
			FCollisionQueryParams LineTraceParams;

			if (const auto AvatarActor = GetAvatarActorFromActorInfo())
			{
				LineTraceParams.AddIgnoredActor(AvatarActor);
				LineTraceParams.AddIgnoredActors(AvatarActor->Children);
			}
			
			FHitResult HitResult;
			World->LineTraceSingleByChannel(HitResult, LineTraceStart, LineTraceEnd, TRACE_CHANNEL_WEAPON, LineTraceParams);

			if (bDebug)
			{
				UKismetSystemLibrary::DrawDebugLine(this, LineTraceStart, LineTraceEnd, FColor::Cyan, 1.f);
			}
			
			if (HitResult.bBlockingHit)
			{
				if (bDebug)
				{
					UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s, hit component: %s"), *HitResult.GetActor()->GetName(), *HitResult.GetComponent()->GetName())
				}
				
				if (ImpactParticleSystem) UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticleSystem, HitResult.Location);
			}
		}
	}
}
