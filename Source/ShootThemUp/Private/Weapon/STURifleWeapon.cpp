// Shoot Them Up Game 2022


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

ASTURifleWeapon::ASTURifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire()
{
	
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot()
{
	//UE_LOG(LogTemp, Display, TEXT("FIRE"));
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}
	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}
	
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	
	if(HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		//DrawDebugLine(GetWorld(),GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.f, 0, 3.f);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 24, FColor::Red, false, 5.f);
		WeaponFXComponent->PlayImpactFX(HitResult);
		//UE_LOG(LogTemp, Display, TEXT("ACTOR %s HIT (RIFLE)"), *HitResult.GetActor()->GetName());
	}
	else
	{
		DrawDebugLine(GetWorld(),GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.f, 0, 3.f);
	}

	DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}