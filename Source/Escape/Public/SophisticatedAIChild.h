// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "SophisticatedAIChild.generated.h"

UCLASS()
class ESCAPE_API ASophisticatedAIChild : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASophisticatedAIChild();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PostInitProperties();

	
	/** Will the AI run through the mine field? */
	UPROPERTY(EditAnywhere)
		float risktaking;
	
	/** Target To Follow for Children*/
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
		AActor* targetToFollow;

};
