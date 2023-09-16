// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lamp.generated.h"

UCLASS()
class GAME_API ALamp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALamp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* LampMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
		class UPointLightComponent* LightComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
		class UBoxComponent* BoxComp;

	UFUNCTION()
		void ToggleLamp();
	UFUNCTION()
		void TimerEnd();

private:
	float TargetRadius;
	float CurrentRadius;
	float SetRadius;

	UPROPERTY(EditAnywhere)
		float TimerDelay = 0.25f;

	bool bIsTurnedOn;
	bool bIsChanging;
};
