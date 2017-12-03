// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
}

void UGrabber::Release()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab released"));
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

    _physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (_physicsHandle)
    {

    }
    else
    {
	    UE_LOG(LogTemp, Error, TEXT("Missing physics handle component from %s"), *GetOwner()->GetName());
    }

    _inputHandle = GetOwner()->FindComponentByClass<UInputComponent>();
    if (_inputHandle)
    {
        _inputHandle->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        _inputHandle->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else
    {
	    UE_LOG(LogTemp, Error, TEXT("Missing input handle component from %s"), *GetOwner()->GetName());
    }
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

