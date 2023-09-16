// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class GAME_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CloseDoor(float dt);

	UFUNCTION()
	void OpenDoor(float dt);

	class UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere, Category = "Box Comps")
		class UBoxComponent* BoxComp;

	UFUNCTION()
	void ToggleDoor(FVector ForwardVector);

	bool Opening;
	bool Closing;
	bool isClosed;

	float DotP;
	float MaxDegree;
	float AddRotation;
	float PosNeg;
	float DoorCurrentRotation;
};
