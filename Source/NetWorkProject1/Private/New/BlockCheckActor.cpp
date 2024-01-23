// Fill out your copyright notice in the Description page of Project Settings.


#include "New/BlockCheckActor.h"

#include "NetGameStateBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameStateBase.h"

// Sets default values
ABlockCheckActor::ABlockCheckActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mouse = CreateDefaultSubobject<USceneComponent>(TEXT("another"));
	mouse->SetupAttachment(meshComp);
}

// Called when the game starts or when spawned
void ABlockCheckActor::BeginPlay()
{
	Super::BeginPlay();
	GameState = GetWorld()->GetGameState<ANetGameStateBase>();
}

// Called every frame
void ABlockCheckActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<FOverlapResult> hitInfos;
	FCollisionObjectQueryParams objParams;
	objParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);

	if (GetWorld()->OverlapMultiByObjectType(hitInfos, GetActorLocation(), FQuat::Identity, objParams, 
	FCollisionShape::MakeBox(FVector(400.f,800.f,500.f))))
	{
		
		for (const FOverlapResult& hitObj : hitInfos)
		{
		
			if (hitObj.GetActor()->ActorHasTag(FName(TEXT("Block"))))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s(%d) block : %d "), *FString(__FUNCTION__), __LINE__,hitInfos.Num());
			}
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s(%d) block : zero "), *FString(__FUNCTION__), __LINE__);
		GameState->setMatchEnd(true);
		GameState->setWinTeam(team);
	}
	
	DrawDebugBox(GetWorld(),GetActorLocation(),FVector(400.f,800.f,500.f),FQuat::Identity,FColor::Magenta,false,0,0,1.0f);

	
}

