// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "MySpaceship.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

class ASpaceshipGameModeBase;
class AMySpaceship;
class AEnemySpawner;
UCLASS()
class SPACESHIP_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Component")
	UStaticMeshComponent* ShipMesh;
	

	// UPROPERTY(EditAnywhere, Category="Component")
	// TSubclassOf<ABullet> Bullet;

	//UPROPERTY(EditAnywhere, Category="Component")
	//USceneComponent* Magazine;//弹匣

	void MoveTowardsPlayer(float DeltaTime);
	

	AMySpaceship* Myship;
	
	UPROPERTY(EditAnywhere)
	float Speed;

	UFUNCTION(BlueprintImplementableEvent)
	void SetColor();
	
	ASpaceshipGameModeBase* GameModeRef;

	AEnemySpawner* EnemySpawner;

	UPROPERTY(EditAnywhere, Category="Component")
	UParticleSystem* Explore_PS;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundCue* ExploreSoundCue;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnExplosion();

	FTimerHandle WorldEndTimerHandle;
	float WorldEndTimer;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Ondeath();

	void WorldEnd();

	bool GoExplosion;

	void Explore();

	void SetSpeed(float speed);
};
