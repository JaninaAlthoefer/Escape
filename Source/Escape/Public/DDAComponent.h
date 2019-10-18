// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DDAComponent.generated.h"


class ASophisticatedAIEnemy;

USTRUCT()
struct FEnemyAttributes
{	
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY()
	float fovRadius;

	UPROPERTY()
	float compassion;

	UPROPERTY()
	float maxAreaRadius;

	UPROPERTY()
	float laziness;

	UPROPERTY()
	float hearingDistance;

	//Fitness 
	float getFitness()
	{
		return (fovRadius - (compassion*compassion) +
			laziness + (0.5*maxAreaRadius) + hearingDistance);
	}

	//override the "<" operator so that this object can be sorted
	FORCEINLINE bool operator<(const FEnemyAttributes &Other) const
	{
		//float thisOne = this->getFitness();
		//float otherOne = Other.getFitness();
		float thisOne = this->fovRadius - (this->compassion*this->compassion) +
			this->laziness + (0.5*this->maxAreaRadius) + this->hearingDistance;
		float otherOne = Other.fovRadius - (Other.compassion*Other.compassion) +
			Other.laziness + (0.5*Other.maxAreaRadius) + Other.hearingDistance;

		return thisOne > otherOne;
	}

	//Constructor
	FEnemyAttributes()
	{
		fovRadius = FMath::FRandRange(50.f, 120.f);
		compassion = FMath::FRandRange(30.f, 100.f);
		maxAreaRadius = FMath::FRandRange(200.f, 600.f);
		laziness = FMath::FRandRange(25.f, 90.f);
		hearingDistance = FMath::FRandRange(50.f, 300.f);
	}

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UDDAComponent : public UActorComponent
{
	GENERATED_BODY()

	TArray<FEnemyAttributes> parents;
	TArray<FEnemyAttributes> result;
	TArray<ASophisticatedAIEnemy*> enemies; 

	int32 uniformRate = 50;
	int32 mutationRate = 25;
	int32 eliteSize = 5;
	int32 numIterations = 20;
	int32 populationSize = 30;

	//Set new Attributes to Enemies
	void SetEnemyAttributes(TArray<FEnemyAttributes> &pop);

	//another bool - to initiate dll recompile
	bool idk_whatever = false;

public:	
	
	// Sets default values for this component's properties
	UDDAComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	//Evolve population
	void Evolve();

	//Crossover attributes
	FEnemyAttributes Breed(FEnemyAttributes p1, FEnemyAttributes p2);

	//Mutate individual
	void Mutate(FEnemyAttributes* c);

	//Who's the Fittest?
	FEnemyAttributes* GetFittest();

	//Get a parent for a child
	FEnemyAttributes GetParent();

	//clear the existing array
	void ClearWard();

	/**UNUSED - Update Population Size*/
	UFUNCTION(BlueprintCallable, Category = "DDA-GA-Values")
		void SetPopulationSize(int32 size);

	/**Update Iterations - Only call before Running GA*/
	UFUNCTION(BlueprintCallable, Category = "DDA-GA-Values")
		void SetNumberIterations(int32 num);

	/**Update GA Rate for Mutation to Occur*/
	UFUNCTION(BlueprintCallable, Category = "DDA-GA-Values")
		void SetMutationRate(float val);

	/**Update the Amount of Elite Members in the Population*/
	UFUNCTION(BlueprintCallable, Category = "DDA-GA-Values")
		void SetEliteSize(int32 val);

	/**Initialize Enemy Array for Genetic Algorithm*/
	UFUNCTION(BlueprintCallable, Category="DDA-GA-Setup")
		void SetEnemyArray(UPARAM(ref) TArray<ASophisticatedAIEnemy*> enem);

	/**Set the Target Values for the Fitness Function*/
	UFUNCTION(BlueprintCallable, Category = "DDA-GA-Setup")
		void SetGATargetValues(TArray<float> targetVals);

	UFUNCTION(BlueprintCallable, Category = "DDA-GA-Setup")
		void printEnemyAttributes();

	/**Run Genetic Algorithm*/
	UFUNCTION(BlueprintCallable, Category = "DDA-GA-Setup")
		void RunGA();
};
