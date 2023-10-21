// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Door.h"
#include "Lamp.h"
#include "FirstPersonCharacter.generated.h"


UCLASS()
class GAME_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;
public:
	// Sets default values for this character's properties
	AFirstPersonCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void OnInteraction();
	void OnItemInteraction();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);


public:
	APlayerCameraManager* PlayerCamera;

	UPROPERTY(EditDefaultsOnly)
	float InteractionRange = 100;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;

	class ADoor* CurrentDoor;
	class ALamp* CurrentLamp;
	class ARotObject* RotObject;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<class UCameraShakeBase> StandingCameraShakeClass;

	void CameraShakeControll(TSubclassOf<class UCameraShakeBase> StoppedCameraShake, TSubclassOf<class UCameraShakeBase> StartedCameraShake);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
