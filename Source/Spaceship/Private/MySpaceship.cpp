// Fill out your copyright notice in the Description page of Project Settings.


#include "MySpaceship.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AMySpaceship::AMySpaceship()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));

	//CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	
	RootComponent = ShipMesh;
	
	//CollisionBox->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));

	SpringArm->SetupAttachment(RootComponent);
	MainCamera->SetupAttachment(SpringArm);

	Magazine = CreateDefaultSubobject<USceneComponent>(TEXT("Magazine"));
	Magazine->SetupAttachment(RootComponent);

	Fly_PSComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fly_PSComponent"));
	Fly_PSComponent->SetupAttachment(RootComponent);
	Fly_PSComponent->Activate();
	
	Speed = 1500.0f;
	AttackTimer = 0.2f;
	RestartTimer = 2.0f;

	bDead = false;
}

// Called when the game starts or when spawned
void AMySpaceship::BeginPlay()
{
	Super::BeginPlay();
	
	MyShipController = Cast<APlayerController>(GetController()); 
	MyShipController->bShowMouseCursor = true;
	
}

void AMySpaceship::ControlRotationYaw()
{
	FVector MouseLocation, MouseDirection;
	MyShipController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	FVector TargetLocation = FVector(MouseLocation.X, MouseLocation.Y, GetActorLocation().Z);
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),TargetLocation);
	Rotator.Yaw -= 90.0f;
	SetActorRotation(Rotator);
	
}

void AMySpaceship::MoveForward(float Value)
{
	AddMovementInput(FVector::ForwardVector, Value);
	
}

void AMySpaceship::MoveRight(float Value)
{
	AddMovementInput(FVector::RightVector, Value);
}

void AMySpaceship::Move(float DeltaTime)
{
	FVector InputVector = ConsumeMovementInputVector();
	if (InputVector.X != 0 && InputVector.Y != 0)
	{
		AddActorWorldOffset((InputVector*Speed*DeltaTime)/(sqrt(2)), true);
	}
	else
	{
		AddActorWorldOffset(InputVector*Speed*DeltaTime, true);
	}
	
}

void AMySpaceship::RestartLevel()
{
	if (bDead) Ondead();
}

void AMySpaceship::Attack()
{
	if (!bDead)
	{
		if (Bullet)
		{
			FActorSpawnParameters SpawnParameter;
			FRotator BulletRotator = Magazine->GetComponentRotation();
			BulletRotator.Roll += 90.0f;
			GetWorld()->SpawnActor<ABullet>(Bullet, Magazine->GetComponentLocation(), BulletRotator, SpawnParameter);
			if (ShootSoundCue) UGameplayStatics::PlaySoundAtLocation(this, ShootSoundCue, GetActorLocation());
			
		}
	}
}

void AMySpaceship::AttackBegin()
{
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AMySpaceship::Attack, AttackTimer, true, 0.0f);
}

void AMySpaceship::AttackEnd()
{
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
}

// Called every frame
void AMySpaceship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bDead)
	{
		ControlRotationYaw();
		Move(DeltaTime);
	}
	else
	{
		Fly_PSComponent->Deactivate();
	}
	
}

// Called to bind functionality to input
void AMySpaceship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMySpaceship::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMySpaceship::MoveRight);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMySpaceship::AttackBegin);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMySpaceship::AttackEnd);
}

void AMySpaceship::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (Cast<AEnemy>(OtherActor))
	{
		Enemy = Cast<AEnemy>(OtherActor);
		Enemy->Ondeath();
		bDead = true;
		if (GameOverSoundCue) UGameplayStatics::PlaySoundAtLocation(this, GameOverSoundCue, GetActorLocation());
		if (Explore_PS) UGameplayStatics::SpawnEmitterAtLocation(this, Explore_PS, GetActorLocation(), FRotator::ZeroRotator, true);
		ShipMesh->SetVisibility(false);
		GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AMySpaceship::RestartLevel, RestartTimer, false);
	}
}

