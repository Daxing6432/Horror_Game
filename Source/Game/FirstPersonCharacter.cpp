// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraShakeBase.h"
#include "DrawDebugHelpers.h"
#include "Misc/OutputDeviceNull.h"


// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

	//Set Camera component values
	//---------------------------------------------------------------------------------------
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	//---------------------------------------------------------------------------------------


	//Set Trigger Capsule component values
	//---------------------------------------------------------------------------------------
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 96.f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	//---------------------------------------------------------------------------------------

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AFirstPersonCharacter::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AFirstPersonCharacter::OnOverlapEnd);

	CurrentDoor = NULL;


}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Setup Player camera ptr
	if(UWorld* World = GetWorld())
	{
		PlayerCamera = World->GetFirstPlayerController()->PlayerCameraManager;
	}
	
	//Start idle camera shake
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(StandingCameraShakeClass);	
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//						Inputs
//-----------------------------------------------------------------
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward", this, &AFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &AFirstPersonCharacter::MoveRight);

	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AFirstPersonCharacter::OnInteraction);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AFirstPersonCharacter::OnItemInteraction);
	
	PlayerInputComponent->BindAction("Move", IE_Pressed, this, &AFirstPersonCharacter::StartedMoving);
	PlayerInputComponent->BindAction("Move", IE_Released, this, &AFirstPersonCharacter::StoppedMoving);
}
//-----------------------------------------------------------------


//					Move Functions
//-----------------------------------------------------------------
void AFirstPersonCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}
void AFirstPersonCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AFirstPersonCharacter::StartedMoving()
{
	CameraShakeControll(StandingCameraShakeClass, WalkCameraShakeClass);
}

void AFirstPersonCharacter::StoppedMoving()
{	
	CameraShakeControll(WalkCameraShakeClass, StandingCameraShakeClass);
}
//-----------------------------------------------------------------


//					Interaction voids
//-----------------------------------------------------------------
void AFirstPersonCharacter::OnInteraction()
{
	if(CurrentDoor != nullptr && PlayerCamera != nullptr)
	{
		FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
		CurrentDoor->ToggleDoor(ForwardVector);
	}

	if(CurrentLamp != nullptr )
	{
		CurrentLamp->ToggleLamp();
	}
}

void AFirstPersonCharacter::OnItemInteraction()
{
	AController* PlayerController = GetController();
	if(PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR, Player controller not found check FPC.cpp"))
		return;	
	}

	FVector Location;
	FRotator Rotation;

	PlayerController->GetPlayerViewPoint(Location, Rotation);

	FVector EndLocation = Location + Rotation.Vector() * InteractionRange;


	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, Location, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);

	if (bSuccess)
	{
		AActor* ActorHit = HitResult.GetActor();
			
		FOutputDeviceNull ar2;
		const FString TurnOffcommand = FString::Printf(TEXT("RemoveText"));
		CallFunctionByNameWithArguments(*TurnOffcommand, ar2, NULL, true);
		
		if(ActorHit->ActorHasTag("Cactus"))
		{
			FOutputDeviceNull ar;
			const FString command = FString::Printf(TEXT("DisplayText Cactus"));
			CallFunctionByNameWithArguments(*command, ar, NULL, true);		}

		if(ActorHit->ActorHasTag("Statuette"))
		{
			FOutputDeviceNull ar;
			const FString command = FString::Printf(TEXT("DisplayText Its made from clay"));
			CallFunctionByNameWithArguments(*command, ar, NULL, true);		}

		if(ActorHit->ActorHasTag("Bush"))
		{
			FOutputDeviceNull ar;
			const FString command = FString::Printf(TEXT("DisplayText It's approximately 1 meter tall"));
			CallFunctionByNameWithArguments(*command, ar, NULL, true);		}

		if(ActorHit->ActorHasTag("Painting"))
		{
			FOutputDeviceNull ar;
			const FString command = FString::Printf(TEXT("DisplayText  Landscape by Franco de Lare"));
			CallFunctionByNameWithArguments(*command, ar, NULL, true);		}
		
		if(ActorHit->ActorHasTag("PFrame"))
		{
			FOutputDeviceNull ar;
			const FString command = FString::Printf(TEXT("DisplayText One of my best creations"));
			CallFunctionByNameWithArguments(*command, ar, NULL, true);
		}
	}	
}
//-----------------------------------------------------------------


//					Overlap voids
//-----------------------------------------------------------------
void AFirstPersonCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,  AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Check for objects in range
	if((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherActor->GetClass()->IsChildOf(ADoor::StaticClass()))
	{
		CurrentDoor = Cast<ADoor>(OtherActor);
	}

	if((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherActor->GetClass()->IsChildOf(ALamp::StaticClass()))
	{
		CurrentLamp = Cast<ALamp>(OtherActor);
	}
}

void AFirstPersonCharacter::OnOverlapEnd( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		CurrentDoor = NULL;
		CurrentLamp = NULL;
	}
}
//-----------------------------------------------------------------


void AFirstPersonCharacter::CameraShakeControll(TSubclassOf<UCameraShakeBase> StoppedCameraShake, TSubclassOf<UCameraShakeBase> StartedCameraShake)
{
	if(StoppedCameraShake != nullptr && StartedCameraShake != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->ClientStopCameraShake(StoppedCameraShake);

		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(StartedCameraShake);
	}
}
