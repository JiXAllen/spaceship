// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Sound/SoundCue.h"
#include "MySpaceship.generated.h"


class ABullet;
UCLASS()
class SPACESHIP_API AMySpaceship : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMySpaceship();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UStaticMeshComponent* ShipMesh;

	//UPROPERTY(VisibleAnywhere, Category="Component")
	//UBoxComponent* CollisionBox;
	
	UPROPERTY(VisibleAnywhere, Category="Component")
	UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, Category="Component")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category="Component")
	TSubclassOf<ABullet> Bullet;

	UPROPERTY(EditAnywhere, Category="Component")
	USceneComponent* Magazine;//µ¯Ï»
	
	APlayerController* MyShipController;

	AEnemy* Enemy;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundCue* GameOverSoundCue;
	UPROPERTY(EditAnywhere, Category="Sound")
	USoundCue* ShootSoundCue;

	UPROPERTY(EditAnywhere, Category="Component")
	UParticleSystemComponent* Fly_PSComponent;

	UPROPERTY(EditAnywhere, Category="Component")
	UParticleSystem* Explore_PS;

	UFUNCTION(BlueprintImplementableEvent)
	void Ondead();

	void ControlRotationYaw();

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Move(float DeltaTime);

	void RestartLevel();

	//Éä»÷
	void Attack();
	void AttackBegin();
	void AttackEnd();
	
	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	float AttackTimer;

	float RestartTimer;

	bool bDead;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	FTimerHandle AttackTimerHandle;

	FTimerHandle RestartTimerHandle;

	FORCEINLINE bool GetbDead()
	{
		return bDead;
	}

	
};
