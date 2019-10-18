// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnemyAICharacter.generated.h"

UCLASS()
class ESCAPE_API AEnemyAICharacter : public ACharacter
{
	GENERATED_BODY()


		/** Will the AI run through the mine field? /
		UPROPERTY(EditInstanceOnly)
		float risktaking = 0.0f;*/

public:
	// Sets default values for this character's properties
	AEnemyAICharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** First Waypoint for Patrolling */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Meta = (MakeEditWidget = true))
		FVector Waypoint1;

	/** Second Waypoint for Patrolling */
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Meta = (MakeEditWidget = true))
		FVector Waypoint2;
	
};
