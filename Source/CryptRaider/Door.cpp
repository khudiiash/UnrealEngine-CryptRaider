// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Math/UnrealMathUtility.h"


// Sets default values for this component's properties
UDoor::UDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDoor::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> Children;
	GetOwner()->GetComponents<UStaticMeshComponent>(Children);
	if (Children.Num() > 0) {
		DoorMesh = Children[1];
	};
    OriginalRotation = GetOwner()->GetActorRotation();
}


// Called every frame
void UDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (ShouldOpen) OpenDoor(DeltaTime);
	if (ShouldClose) CloseDoor(DeltaTime);
}

bool UDoor::SetShouldOpen(AActor* Actor)
{
	if (ShouldOpen) return false;

	if (Actor->ActorHasTag("Key")) {
		ShouldOpen = true;
		Actor->Destroy();
		return true;
   	} else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("You need a key to open this door!"));
		return false;
	}
}

void UDoor::OpenDoor(float DeltaTime)
{
    FRotator CurrentRotation = DoorMesh->GetRelativeRotation();
	FRotator TargetRotation = FRotator(0, -90, 0);
	
	float Speed = CurrentRotation.GetManhattanDistance(TargetRotation) / MoveTime;
	FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, Speed);
	DoorMesh->SetRelativeRotation(NewRotation);
}

void UDoor::CloseDoor(float DeltaTime)
{
    FRotator CurrentRotation = DoorMesh->GetComponentRotation();
	FRotator TargetRotation = FRotator(0, 0, 0);
	
	float Speed = CurrentRotation.Yaw / MoveTime;
	FRotator NewRotation = FRotator(0, FMath::FInterpConstantTo(CurrentRotation.Yaw, TargetRotation.Yaw, DeltaTime, Speed), 0);
	DoorMesh->SetWorldRotation(NewRotation);
}
