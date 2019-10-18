// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "SophisticatedAIEnemy.generated.h"

enum vectorValidReturns
{
	TOOSMALL,
	TOOLARGE,
	OUTOFRANGE,
	BLOCKED,
	VALID
};

enum availableTasks
{
	SHIFT,
	GUARD,
	PATROL
};

UCLASS()
class ESCAPE_API ASophisticatedAIEnemy : public ACharacter
{
	GENERATED_BODY()

	float compassion, maxAreaRadius, laziness, waitTime;
	FVector startingPoint, Waypoint1, Waypoint2, Waypoint3;


	float blockedDistance;

	bool isSeen = false;

	//uncomment this to trigger rebuild
	bool idk_sth = false;
	bool debug = false;

	//either in seconds or points/seconds
	float seenDecayRate = 7.f, seenRaiseRate = 20.f, maxWaitTime = 15.f, waitTimeDecayRate = 1.f;

	//all actors of this class, including self
	TArray<AActor*> found;

	TArray<availableTasks> oldTasks;
	TArray<FVector> oldWPs;
	TArray<ASophisticatedAIEnemy*> others;

public:
	// Sets default values for this character's properties
	ASophisticatedAIEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PostInitProperties();

	float getFovRadius();
	void setFovRadius(float val);
	float getCompassion();
	void setCompassion(float val);
	float getMaxAreaRadius();
	void setMaxAreaRadius(float val);
	float getLaziness();
	void setLaziness(float val);
	float getHearingDistance();
	void setHearingDistance(float val);

	//uses Waypoint1 for location & Waypoint2 for rotation to look 
	void planGuardTask();
	//sets Waypoint1 as current location and Waypoint2 & Waypoint3 angled from avoiding others  and old waypoints
	void planPatrolTask();
	//Computes Vector the Character will avoid - simple usage to ensure AI doesn't crowd on one spot
	void getAvoidanceVector(FVector *vec);
	//Checks if the Waypoint is reachable and useable for its purpose
	vectorValidReturns isValidVector(FVector end, FVector start);

	//dummy function - implemented in Blueprints
	void checkSeenRate();
	//Computes how often enemy has guarded to influence next task
	float getTaskRate();
	//Adds to the list of latest tasks, max length is 5
	void addTaskToList(availableTasks task);
	//Adds to the list of latest Waypoints, max length is 5
	void addWPtoList(FVector vector);

	UFUNCTION(BlueprintCallable, Category = "DDA-GA")
		void printAttributes();

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
		float seenRate = 0.f;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
		float hearingDistance;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
		float fovRadius;

	UPROPERTY(BlueprintReadWrite)
		bool taskIsUpdated;

	//Used to artificially raise SeenRate - was supposed to be used for Touch events
	UFUNCTION(BlueprintCallable, Category = "Custom_AI")
		void AddToSeenRate(float value);

	UFUNCTION(BlueprintCallable, Category = "Custom_AI")
		bool IsCompassionate();

	//Enables / Disables SeenRate raising through ticks
	UFUNCTION(BlueprintCallable, Category = "Custom_AI")
		void SetIsSeen(bool seen);

	//returns given Waypoint for Behaviour Tree
	UFUNCTION(BlueprintCallable, Category = "Custom_AI")
		FVector GetWaypoint(int num);
};
