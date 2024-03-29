// Copyright Astrid Avalin Sørensen 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEBUILDING_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void PlayDoorOpenSound();
	void OpenDoor(float time);
	void CloseDoor(float time);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float GetTotalMassOfActorsOnPressurePlate();

private:
	float InitialYaw;	
	float CurrentYaw;	
	
	UPROPERTY(EditAnywhere)
	USoundBase* SFXOnDoorOpen = nullptr;

	UPROPERTY(EditAnywhere)
	USoundBase* SFXOnDoorClose = nullptr;

	UPROPERTY(EditAnywhere)
	float TargetYaw = -150.f;	
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float WeightNecessaryToOpenDoor = 51.f;
};
