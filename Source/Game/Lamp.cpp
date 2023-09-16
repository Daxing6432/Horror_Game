// Fill out your copyright notice in the Description page of Project Settings.


#include "Lamp.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Components/LightComponent.h"


// Sets default values
ALamp::ALamp()
{
	PrimaryActorTick.bCanEverTick = false;
	//Setup Lamp Components
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	BoxComp->InitBoxExtent(FVector(25, 25, 25));
	BoxComp->SetCollisionProfileName("Trigger");
	RootComponent = BoxComp;

	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lamp Mesh"));
	LampMesh->SetupAttachment(RootComponent);

	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	LightComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALamp::BeginPlay()
{
	Super::BeginPlay();
	//Turn off lamp
	LightComponent->SetMobility(EComponentMobility::Movable);
	
	//Set Variables for lamp functioning
	SetRadius = 0.f;
	CurrentRadius = 0.f;
	TargetRadius = 0.f;
	bIsChanging = false;
	bIsTurnedOn = false;

	LightComponent->SetAttenuationRadius(SetRadius);
}

// Called every frame
void ALamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALamp::ToggleLamp()
{
	FTimerHandle TimerHandle;
	if(bIsTurnedOn && !bIsChanging)
	{	
		SetRadius = 0;
		bIsTurnedOn = false;
		bIsChanging = true;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ALamp::TimerEnd, TimerDelay, false);
	}
	else if(!bIsChanging)
	{
		SetRadius = 1000;
		bIsTurnedOn = true;
		bIsChanging = true;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ALamp::TimerEnd, TimerDelay, false);
	}
	LightComponent->SetAttenuationRadius(SetRadius);
}

void ALamp::TimerEnd()
{
	bIsChanging = false;
}