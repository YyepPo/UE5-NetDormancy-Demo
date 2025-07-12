#include "ExplosiveBarrel.h"

#include "Cpp_And_Multiplayer/Macros/DebugMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

AExplosiveBarrel::AExplosiveBarrel()
{
	// Disable tick for performance reasons
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	
	// Replicate 5 times per second
	NetUpdateFrequency = 1;

	// Stop replicating entirely until explicitly woken up
	SetNetDormancy(DORM_DormantAll);

	// Initialize Barrel Static Mesh Component
	SM_Barrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Static Mesh"));
	SetRootComponent(SM_Barrel);

	bEnableDormancy = true;
	
	// Initialize damage properties
	BaseDamageAmount = 50.f;
	MinimumDamage = 25.f;
	DamageInnerRadius = 150.f;
	DamageOuterRadius = 400.f;
	DamageFalloff = 1.f;
}

void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this,&AExplosiveBarrel::HandleTakeAnyDamage);
		SetNetDormancy(bEnableDormancy == true ? DORM_DormantAll : DORM_Awake);
	}
}

void AExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Enable push based replication
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged; 

	DOREPLIFETIME_WITH_PARAMS_FAST(AExplosiveBarrel,bExploded,Params);
}

void AExplosiveBarrel::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                           AController* InstigatedBy, AActor* DamageCauser)
{
	if(bExploded == true)
	{
		return;
	}
	
	if(bEnableDormancy == true)
	{
		// Wake up from dormancy
		SetNetDormancy(DORM_Awake);

		bExploded = true;
		MARK_PROPERTY_DIRTY_FROM_NAME(AExplosiveBarrel,bExploded,this);

		const TArray<AActor*> IgnoredActors;	
		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
			BaseDamageAmount,
			MinimumDamage,
			GetActorLocation(),
			DamageInnerRadius,
			DamageInnerRadius,
			DamageFalloff,
			UDamageType::StaticClass(),
			IgnoredActors);
	
		ForceNetUpdate();
	}
	else
	{
		bExploded = true;
		MARK_PROPERTY_DIRTY_FROM_NAME(AExplosiveBarrel,bExploded,this);

		const TArray<AActor*> IgnoredActors;	
		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
			BaseDamageAmount,
			MinimumDamage,
			GetActorLocation(),
			DamageInnerRadius,
			DamageInnerRadius,
			DamageFalloff,
			UDamageType::StaticClass(),
			IgnoredActors);
	}
	
	// Set a timer for destroying the object
	// Destruction is delayed because the sfx and vfx was not playing,so the object was being destroyed before it could play cosmetics
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AExplosiveBarrel::OnDestroyTimerHandleFinished, 0.2f, false);
}

void AExplosiveBarrel::OnRep_Exploded()
{
	SM_Barrel->SetVisibility(false);
	SM_Barrel->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	if(	const UWorld* World = GetWorld())
	{
		/*** Spawn cosmetics ***/
		// Spawn VFX
		if(ExplosionVFX != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World,
				ExplosionVFX,
				GetActorLocation()
				,FRotator::ZeroRotator);
		}
		else
		{
			PRINT_SCREEN_WARNING(TEXT("Explosive Barrel: ExplosionVFX is not valid.Pls select an ExplosionVFX"));
		}
		
		// Spawn SFX
		if(ExplosionSFX != nullptr)
		{
			UGameplayStatics::SpawnSoundAtLocation(World,
				ExplosionSFX,
				GetActorLocation(),
				FRotator::ZeroRotator);
		}
		else
		{
			PRINT_SCREEN_WARNING(TEXT("Explosive Barrel: ExplosionSFX is not valid.Pls select an ExplosionSFX"));
		}
	}
}

void AExplosiveBarrel::OnDestroyTimerHandleFinished()
{
	Destroy();
}
