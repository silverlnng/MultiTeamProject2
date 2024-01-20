// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "PlayerPawn.generated.h"

UCLASS()
class NETWORKPROJECT1_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveIA;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* FireIA;
	
	UPROPERTY(EditAnywhere,Category = "Mysetting")
	class UBoxComponent* myBoxComp; 

	UPROPERTY(EditAnywhere,Category = "Mysetting")
	class USceneComponent* cannonPivot;
	
	UPROPERTY(EditAnywhere,Category = "Mysetting")
	class UStaticMeshComponent* cannonComp;

	UPROPERTY(EditAnywhere,Category = "Mysetting")
	class UArrowComponent* ArrowComp;
	
	UPROPERTY(EditAnywhere,Category = "Mysetting")
	class UWidgetComponent* crossHairWidget;
	
	
	UPROPERTY(EditAnywhere, Category = "Mysetting")
    float crossHairOffest = 1.2f;
	UPROPERTY(EditAnywhere, Category = "Mysetting")
	class UStaticMeshComponent* cannon;
	UPROPERTY(EditAnywhere, Category = "Mysetting")
	float cannonOffest_x =0.7f;
	UPROPERTY(EditAnywhere, Category = "Mysetting")
	float cannonOffest_y = 0.6f;


	UPROPERTY(EditAnywhere, Category = "Grenade")
	TSubclassOf<class ABullect> bullectFactory;
	UPROPERTY(EditAnywhere, Category = "Grenade")
	ABullect* bullect;
	
private:
	
	void Move(const FInputActionValue& Value);
	void InputFire(const FInputActionValue& Value);
	void spawnBullect();
};
