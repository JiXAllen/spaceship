// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MySpaceship.h"
#include "Engine/World.h"
#include "ctime"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;

	MinDistanceToPlayer = 1800.0f;

	SpawnInterval = 1.0f;

	MaxEnemyNum = 50;
	CurrentEnemyNum = 0;

	srand((unsigned)time(0));
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	MyShip = Cast<AMySpaceship>(UGameplayStatics::GetPlayerPawn(this, 0));
	
	GetWorldTimerManager().SetTimer(EnemySpawnerTimerHandle, this, &AEnemySpawner::SpawnEnemy, SpawnInterval, true);
}

FVector AEnemySpawner::GetSpawnLocation()
{
	FVector Location = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->Bounds.Origin, SpawnArea->Bounds.BoxExtent);
	while ((Location - MyShip->GetActorLocation()).Size() < MinDistanceToPlayer)
	{
		Location = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->Bounds.Origin, SpawnArea->Bounds.BoxExtent);
	}
	return Location;
}

void AEnemySpawner::SpawnEnemy()
{
	if (!MyShip->GetbDead() && CurrentEnemyNum < MaxEnemyNum)
	{
		float Speed = 1200.0f + rand()%400;
		FActorSpawnParameters SpawnParameters;
		GetWorld()->SpawnActor<AEnemy>(Enemy, GetSpawnLocation(), FRotator::ZeroRotator, SpawnParameters)->SetSpeed(Speed);
		CurrentEnemyNum++;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(EnemySpawnerTimerHandle);
		CurrentEnemyNum = 0;
	}
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::DecreaseEnemyNum()
{
	CurrentEnemyNum--;
}

