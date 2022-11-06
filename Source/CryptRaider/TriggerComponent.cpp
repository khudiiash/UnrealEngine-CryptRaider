// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	this->OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin);
	this->OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd);
	// ...
}


// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
}


// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Actor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!Actor->ActorHasTag(UnlockTag)) return;
	UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
	if (Component) {
		Component->SetSimulatePhysics(false);
		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	}
	Mover->SetShouldMove(true);
	Mover->PlaySound();
}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Actor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (!Actor->ActorHasTag(UnlockTag)) return;
	UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
	if (Component) {
		Component->SetSimulatePhysics(true);
		Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	Mover->SetShouldMove(false);
	Mover->PlaySound();
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}