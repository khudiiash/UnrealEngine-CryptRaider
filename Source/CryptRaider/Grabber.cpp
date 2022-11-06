// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateGrabbedObject();
}


void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) return;

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if (!HasHit) return;
	
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();
	ActorHit->Tags.Add(FName("Grabbed"));
	ActorHit->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	ComponentToGrab->SetSimulatePhysics(true);
	ComponentToGrab->WakeAllRigidBodies();
	
	PhysicsHandle->SetInterpolationSpeed(50.f);
	PhysicsHandle->GrabComponentAtLocationWithRotation(
		ComponentToGrab,
		NAME_None,
		HitResult.ImpactPoint,
		GetComponentRotation()
	);
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) return;
	UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	if (GrabbedComponent == nullptr) return;

	AActor* GrabbedActor = GrabbedComponent->GetOwner();
	GrabbedActor->Tags.Remove(FName("Grabbed"));
	PhysicsHandle->ReleaseComponent();
	
}

void UGrabber::UpdateGrabbedObject()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) return;
	UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	if (GrabbedComponent == nullptr) return;


	GrabbedComponent->WakeAllRigidBodies();
	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
	FRotator TargetRotation = GetComponentRotation();
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, TargetRotation);
}

void UGrabber::WakeGrabbedObject()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	if (GrabbedComponent == nullptr) return;
	GrabbedComponent->WakeAllRigidBodies();
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();	
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandle"));
	}
	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& HitResult) const
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) return false;

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		Start, 
		End, 
		FQuat::Identity, 
		ECollisionChannel::ECC_GameTraceChannel2, 
		FCollisionShape::MakeSphere(GrabRadius)
	);

	return HasHit;
}