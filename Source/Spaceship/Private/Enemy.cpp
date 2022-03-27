// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemySpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MySpaceship.h"
#include "Kismet/KismetMathLibrary.h"
#include "Spaceship/SpaceshipGameModeBase.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));

	//CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

	RootComponent = ShipMesh;
	//CollisionBox->SetupAttachment(RootComponent);

	//Magazine = CreateDefaultSubobject<USceneComponent>(TEXT("Magazine"));
	//Magazine->SetupAttachment(RootComponent);
	
	Speed = 1200.0f;
	WorldEndTimer = 1.0f;
	GoExplosion = false;
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Myship = Cast<AMySpaceship>(UGameplayStatics::GetPlayerPawn(this, 0));

	SetColor();

	GameModeRef = Cast<ASpaceshipGameModeBase>(UGameplayStatics::GetGameMode(this));

	TArray<AActor*> EnemySpawnerArray;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawner::StaticClass(), EnemySpawnerArray);
	EnemySpawner = Cast<AEnemySpawner>(EnemySpawnerArray[0]);

	
}

void AEnemy::MoveTowardsPlayer(float DeltaTime)
{
	FVector Direction = (Myship->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	AddActorWorldOffset(Direction*Speed*DeltaTime, true);
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Myship->GetActorLocation());
	Rotator.Yaw -= 90.0f;
	SetActorRotation(Rotator);
}

void AEnemy::Ondeath()
{
	GameModeRef->CountScore();
	EnemySpawner->DecreaseEnemyNum();
	if (ExploreSoundCue) UGameplayStatics::PlaySoundAtLocation(this, ExploreSoundCue, GetActorLocation());
	SpawnExplosion();
	//if (Explore_PS) UGameplayStatics::SpawnEmitterAtLocation(this, Explore_PS, GetActorLocation(), FRotator::ZeroRotator, true);
	Destroy();
}

void AEnemy::WorldEnd()
{
	if (!GoExplosion)
	{
		GetWorldTimerManager().SetTimer(WorldEndTimerHandle, this, &AEnemy::Explore, WorldEndTimer, false);
		GoExplosion = true;
	}
}

void AEnemy::Explore()
{
	//if (ExploreSoundCue) UGameplayStatics::PlaySoundAtLocation(this, ExploreSoundCue, GetActorLocation());
	SpawnExplosion();
	GetWorldTimerManager().ClearTimer(WorldEndTimerHandle);
	Destroy();
}

void AEnemy::SetSpeed(float speed)
{
	Speed = speed;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Myship->GetbDead()) MoveTowardsPlayer(DeltaTime);
	else WorldEnd();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

