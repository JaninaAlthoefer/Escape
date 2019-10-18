// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ChildrenAiCharacter.generated.h"

UCLASS()
class ESCAPE_API AChildrenAiCharacter : public ACharacter
{
	GENERATED_BODY()

	UWorld* World;


public:
	// Sets default values for this character's properties
	AChildrenAiCharacter();

	void PostInitProperties();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Will the AI run through the mine field? */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
		float risktaking;

	
};
