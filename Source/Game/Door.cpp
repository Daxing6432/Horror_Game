// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	BoxComp->InitBoxExtent(FVector(50, 110, 50));
	BoxComp->SetCollisionProfileName("Trigger");
	RootComponent = BoxComp;

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorAsset(TEXT("/Game/Meshes/Door.Door"));
	if(DoorAsset.Succeeded())
	{
		Door->SetStaticMesh(DoorAsset.Object);
		Door->SetRelativeLocation(FVector(-30.f ,0.f ,-100.f));
		Door->SetWorldScale3D(FVector(1.f));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Door mesh not found!, Check Door.cpp"));
	}

	isClosed = true;
	Opening = false;
	Closing = false;

	DotP = 0.f;
	MaxDegree = 0.f;
	PosNeg = 0.f;
	DoorCurrentRotation = 0.f;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Opening)
	{
		OpenDoor(DeltaTime);
	}

	if(Closing)
	{
		CloseDoor(DeltaTime);
	}
}

void ADoor::OpenDoor(float dt)
{
	DoorCurrentRotation = Door->GetRelativeRotation().Yaw;

	AddRotation = PosNeg * dt * 80;

	if (FMath::IsNearlyEqual(DoorCurrentRotation, MaxDegree, 1.5f))
	{
		Closing = false;
		Opening = false;
	}
	else if(Opening)
	{
		FRotator NewRotation = FRotator(0.f, AddRotation, 0.f);
		Door->AddRelativeRotation(FQuat(NewRotation), false, 0, ETeleportType::None);
	}
}

void ADoor::CloseDoor(float dt)
{
	DoorCurrentRotation = Door->GetRelativeRotation().Yaw;
	if(DoorCurrentRotation > 0)
	{
		AddRotation = -dt*80;
	}
	else
	{
		AddRotation = dt*80;
	}

	if(FMath::IsNearlyEqual(DoorCurrentRotation, 0.f, 1.5f))
	{
		Closing = false;
		Opening = false;

	}
	else if(Closing)
	{
		FRotator NewRotation = FRotator(0.f, AddRotation, 0.f);
		Door->AddRelativeRotation(FQuat(NewRotation), false, 0, ETeleportType::None);
	}
}

void ADoor::ToggleDoor(FVector ForwardVector)
{
	DotP = FVector::DotProduct(BoxComp->GetForwardVector(),ForwardVector);

	PosNeg = FMath::Sign(DotP);

	MaxDegree = PosNeg * 90.f;

	if(isClosed)
	{
		isClosed = false;
		Closing = false;
		Opening = true;
	}
	else
	{
		Opening = false;
		isClosed = true;
		Closing = true;
	}
}