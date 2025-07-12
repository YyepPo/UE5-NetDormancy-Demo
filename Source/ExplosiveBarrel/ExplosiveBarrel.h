// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

UCLASS()
class CPP_AND_MULTIPLAYER_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	
	AExplosiveBarrel();

protected:
	
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor,
			float Damage,
			const UDamageType* DamageType,
			AController* InstigatedBy,
			AActor* DamageCauser);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
		float BaseDamageAmount = 50.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
		float MinimumDamage = 10.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
		float DamageInnerRadius = 50.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
		float DamageOuterRadius = 10.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
		float DamageFalloff = 10.f;

private:

	/** Barrel Static Mesh Component **/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* SM_Barrel;

	UPROPERTY(ReplicatedUsing = OnRep_Exploded,VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
		bool bExploded = false;
	UFUNCTION()
		void OnRep_Exploded();

	/** Timer responsible for destroying this object **/
	FTimerHandle DestroyTimerHandle;
	UFUNCTION()
		void OnDestroyTimerHandleFinished();

	UPROPERTY(EditAnywhere)
	bool bEnableDormancy = true;
	
	/***
	 * Cosmetics
	 ***/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
		UParticleSystem* ExplosionVFX;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
		USoundBase* ExplosionSFX;
	
};
