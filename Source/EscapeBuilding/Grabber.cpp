// Copyright Astrid Avalin Sørensen 2019


#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	LoadComponents();
}

void UGrabber::LoadComponents()
{
	LoadPhysicsHandle();
	LoadInputComponent();
}

void UGrabber::LoadPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle found on object %s."), *GetOwner()->GetName());
	}
}

void UGrabber::LoadInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::GrabObject);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::ReleaseObject);
	}
}



void UGrabber::GrabObject()
{
	//Checks if hitResult has the PhysicsBody collision detection
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) 
	{
		if (!PhysicsHandle) return;
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, //No bones needed.
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true //Allow rotation
		);
	}
}


void UGrabber::ReleaseObject()
{
	if (!PhysicsHandle) return;
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetPlayerViewLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetPlayerViewLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerViewLineStart(),
		GetPlayerViewLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}

void UGrabber::ObjectFollowPlayer()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	FVector LineTraceEnd = GetPlayerViewLineEnd();

	if (!PhysicsHandle) return;
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ObjectFollowPlayer();
}

//Debug method
void UGrabber::DebugRayCast(FVector Location, FRotator Rotation)
{
	FVector LineTraceEnd = Location + (Rotation.Vector() * Reach);
	//Drawing the raycast, can be commented out
	DrawDebugLine(
		GetWorld(),
		Location,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);
}

