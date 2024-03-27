// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFiredSignature);

/**
 * 
 */
UCLASS()
class COLOURSHOOTERGAME_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaximumAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int CurrentAmmo;

	UPROPERTY(EditDefaultsOnly)
	class UClass* ProjectileType;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponFiredSignature OnWeaponFired;

	void Attach(class APawn* Character);
	void Detach();

	void PullTrigger();
	void ReleaseTrigger();

	FVector GetMuzzleLocation() const;

protected:
	AWeaponBase();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Fire();

private:
	void ClearFireTimer();

	FTimerHandle FireTimer;

	class USphereComponent* CollsionSphere;
	class UPrimitiveComponent* Primitive;	
	class UArrowComponent* Muzzle;
	
};
