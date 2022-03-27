// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "MySpaceship.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemy;
UCLASS()
class SPACESHIP_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Enemy")
	TSubclassOf<AEnemy> Enemy;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* SpawnArea;

	FVector GetSpawnLocation();

	float MinDistanceToPlayer;

	AMySpaceship* MyShip;

	void SpawnEnemy();

	FTimerHandle EnemySpawnerTimerHandle;

	UPROPERTY(EditAnywhere, Category="Enemy")
	float SpawnInterval;

	UPROPERTY(EditAnywhere, Category="Enemy")
	int MaxEnemyNum;

	int CurrentEnemyNum;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DecreaseEnemyNum();

};
