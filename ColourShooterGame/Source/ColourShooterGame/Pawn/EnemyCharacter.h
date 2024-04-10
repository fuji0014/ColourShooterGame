// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacter.generated.h"

//UENUM(BlueprintType)
//enum class EEnemyType :uint8 {
//	RedEnemy = 0,
//	BlueEnemy = 1
//};
UCLASS()
class COLOURSHOOTERGAME_API AEnemyCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	class USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	UStaticMeshComponent* VisualMesh;

	float MaxHealth = 100;
	float Health = MaxHealth;
	void DecreaseHealth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHitActor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnCollisionSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class AMainCharacter* MainChar;

	/*UPROPERTY(EditAnywhere, Category = "EnemyType")
	EEnemyType EnemyType;*/

private:
	void PrintMessageOnScreen(FString Message);
};
