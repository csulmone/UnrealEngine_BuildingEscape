// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"

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

    auto hr = GetFirstPhysicsBodyInReach();
    if (hr.GetActor())
    {
        _physicsHandle->GrabComponent(hr.GetComponent(), NAME_None, hr.Actor->GetActorLocation(), true);
    }
}

void UGrabber::Release()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab released"));

    _physicsHandle->ReleaseComponent();
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

    FindPhysicsHandleComponent();

    SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
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

void UGrabber::FindPhysicsHandleComponent()
{
    _physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (_physicsHandle)
    {

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Missing physics handle component from %s"), *GetOwner()->GetName());
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

    if (_physicsHandle->GrabbedComponent)
    {
        _physicsHandle->SetTargetLocation(traceEnd);
    }
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
    FVector loc;
    FRotator rot;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(loc, rot);

    FVector traceEnd = loc + rot.Vector() * _reach;

    FHitResult hr;
    FCollisionObjectQueryParams coqp = { ECollisionChannel::ECC_PhysicsBody };
    FCollisionQueryParams tp = { FName(TEXT("")), false, GetOwner() };
    if (GetWorld()->LineTraceSingleByObjectType(hr, loc, traceEnd, coqp, tp))
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *hr.Actor.Get()->GetName());
    }

    return hr;
}

