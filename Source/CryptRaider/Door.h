// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UDoor : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:	
	UPROPERTY(EditAnywhere)
	float MoveTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Door")
	bool ShouldOpen = false;

	UPROPERTY(EditAnywhere, Category = "Door")
	bool ShouldClose = false;

	UFUNCTION(BlueprintCallable, Category = "Door")
	bool SetShouldOpen(AActor* Actor);

	UStaticMeshComponent* DoorMesh = nullptr;

	FRotator OriginalRotation;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

};
