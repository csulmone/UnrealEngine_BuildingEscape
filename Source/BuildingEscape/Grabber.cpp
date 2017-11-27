// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
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

    FVector loc;
    FRotator rot;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(loc, rot);
    
    FVector traceEnd = loc + rot.Vector() * _reach;

    DrawDebugLine(GetWorld(), loc, traceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);

    FHitResult hr;
    FCollisionObjectQueryParams coqp = { ECollisionChannel::ECC_PhysicsBody };
    FCollisionQueryParams tp = { FName(TEXT("")), false, GetOwner() };
    if (GetWorld()->LineTraceSingleByObjectType(hr, loc, traceEnd, coqp, tp))
    {
	    UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *hr.Actor.Get()->GetName());
    }
}

