// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/IslandInterface.h"

// Add default functionality here for any IIslandInterface functions that are not pure virtual.
FRandomStream IIslandInterface::IslandSeed()
{
	return FRandomStream();
}

void IIslandInterface::SpawningComplete()
{
}

void IIslandInterface::ScaleUp(float Delay)
{
}

void IIslandInterface::IslandGenComplete()
{
}
