// Fill out your copyright notice in the Description page of Project Settings.


#include "New/CaptureCamActor.h"
#include "Components/SceneCaptureComponent2D.h"
// Sets default values
ACaptureCamActor::ACaptureCamActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	captureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture Comp"));
	SetRootComponent(captureComp);
}

// Called when the game starts or when spawned
void ACaptureCamActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACaptureCamActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

