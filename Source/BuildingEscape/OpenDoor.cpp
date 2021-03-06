// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f));
}

float UOpenDoor::GetMassOfActorsOnPlate()
{
    float totalMass = 0.f;
    TSet<AActor*> overlappingActors;
    PressurePlate->GetOverlappingActors(overlappingActors);

    for (const auto actor : overlappingActors)
    {
        totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        UE_LOG(LogTemp, Error, TEXT("Actor |%s| Total Mass |%f|"), *actor->GetName(), actor->FindComponentByClass<UPrimitiveComponent>()->GetMass());
    }


    UE_LOG(LogTemp, Error, TEXT("Total Mass %f"), totalMass);

    return totalMass;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (GetMassOfActorsOnPlate() > PlateMassThreshold)
    {
        OpenDoor();
        LastOpenTime = GetWorld()->GetTimeSeconds();
    }

    if (LastOpenTime != 0.f)
    {
        if (GetWorld()->GetTimeSeconds() - LastOpenTime > DoorCloseDelay)
        {
            CloseDoor();
            LastOpenTime = 0.f;
        }
    }
}

