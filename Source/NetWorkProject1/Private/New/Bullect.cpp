// Fill out your copyright notice in the Description page of Project Settings.


#include "New/Bullect.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullect::ABullect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = collisionComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetupAttachment(collisionComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//발사체 컴포넌트 생성
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	// 발사체 컴포넌트를 update 시킬 컴포넌트 지정, 
	// update == tick, update ->매프레임 연산하겠다
	movementComp->SetUpdatedComponent(collisionComp);
}

// Called when the game starts or when spawned
void ABullect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

