// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullect.generated.h"

UCLASS()
class NETWORKPROJECT1_API ABullect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* movementComp;

	UPROPERTY(VisibleAnywhere, Category = "Col")
	class USphereComponent* collisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* meshComp;
};
