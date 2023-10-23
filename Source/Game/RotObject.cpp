// Fill out your copyright notice in the Description page of Project Settings.


#include "RotObject.h"
#include "FirstPersonCharacter.h"
#include "Kismet/GameplayStatics.h"
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

	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();

	EnableInput(GetWorld()->GetFirstPlayerController());

	if(InputComponent)
	{
		FInputAxisBinding& RotateBinding = InputComponent->BindAxis("Move Right", this ,&ARotObject::RotateObjectZ);
		FInputActionBinding& PutDownObjectBinding = InputComponent->BindAction("PutObjectDown", IE_Released ,this, &ARotObject::CastToFPC);
   		
		RotateBinding.bConsumeInput = false;
		RotateBinding.bExecuteWhenPaused = true;

		PutDownObjectBinding.bConsumeInput = false;
		PutDownObjectBinding.bExecuteWhenPaused = true;
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
	else
	{
		CurrentLocaction = FMath::Lerp(CurrentLocaction, StartLocation, 0.1f);
		SetActorRotation(StartRotation);
		SetActorLocation(CurrentLocaction);
	}
}

void ARotObject::TakeObject()
{
	// Get Player camera rotation and location 

	bIsActive = true;
	AController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player controller not found"))
		return;	
	}

	FVector Location;
	FRotator Rotation;
	
	PlayerController->GetPlayerViewPoint(Location, Rotation);

	TargetLocation = Location + Rotation.Vector() * 70;
	TargetLocation.Z -= 10;

	SetActorRotation(Rotation);

	NewRotation = Rotation;
}

void ARotObject::RotateObjectZ(float AxisValue)
{	
	if(bIsActive)
	{
		float NewYawRotation = NewRotation.Yaw + AxisValue * -1.5;

		NewRotation.Roll = NewRotation.Roll;
		NewRotation.Pitch = NewRotation.Pitch;
		NewRotation.Yaw = NewYawRotation;
		SetActorRotation(NewRotation);
	}
}

void ARotObject::PutDownObject()
{	

}

void ARotObject::CastToFPC()
{

	if(bIsActive && FirstPersonCharacterPTR != nullptr)
	{
		FirstPersonCharacterPTR->PutItemDown();
		bIsActive = false;
	}
}