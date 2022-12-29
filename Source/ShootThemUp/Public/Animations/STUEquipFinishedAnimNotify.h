// Shoot Them Up Game 2022

#pragma once

#include "CoreMinimal.h"
#include "Animations/STUAnimNotify.h"
#include "STUEquipFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUEquipFinishedAnimNotify : public USTUAnimNotify
{
	GENERATED_BODY()
	
};
