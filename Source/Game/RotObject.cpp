// Fill out your copyright notice in the Description page of Project Settings.


#include "RotObject.h"

// Sets default values
ARotObject::ARotObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARotObject::BeginPlay()
{
	Super::BeginPlay();
	CurrentLocaction = GetActorLocation();
	EnableInput(GetWorld()->GetFirstPlayerController());

	if(InputComponent)
	{
		FInputAxisBinding& LookBinding = InputComponent->BindAxis("Move Forward", this ,&ARotObject::RotateObjectZ);

   		LookBinding.bConsumeInput = false;
		LookBinding.bExecuteWhenPaused = true;


	}
  
}

// Called every frame
void ARotObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsActive)
	{
		CurrentLocaction = FMath::Lerp(CurrentLocaction, TargetLocation, 0.1f);
		SetActorLocation(CurrentLocaction);
	}


}

void ARotObject::TakeObject()
{
	// Get Player camera rotation and location 
	

	AController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player controller not found"))
		return;	
	}

	FVector Location;
	FRotator Rotation;

	PlayerController->GetPlayerViewPoint(Location, Rotation);

	TargetLocation = Location + Rotation.Vector() * 80;
	TargetLocation.Z -= 10;
	SetActorRotation(Rotation);
}

void ARotObject::RotateObjectZ(float AxisValue)
{	
	if(bIsActive)
	{
		float NewYawRotation = GetActorRotation().Yaw + AxisValue * -1.5;
		FRotator NewRotation; 

		NewRotation.Roll = GetActorRotation().Roll;
		NewRotation.Pitch = GetActorRotation().Pitch;
		NewRotation.Yaw = NewYawRotation;
		SetActorRotation(NewRotation);
	}
}
