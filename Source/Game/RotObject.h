// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotObject.generated.h"

UCLASS()
class GAME_API ARotObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotObject();
	
	UPROPERTY(EditAnywhere)
	bool bIsActive = false;

	//Function called in FPC while interacting with this class
	void RotateObjectZ(float AxisValue);

	void TakeObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	
	
	FVector CurrentLocaction;
	FVector TargetLocation;
};
