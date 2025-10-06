// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCommandHandler.h"
#include "ICommand.h"

#include "SlideCommand.h"

// Sets default values for this component's properties
UPlayerCommandHandler::UPlayerCommandHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
	characterBase = (ACharacterBase*)GetOwner();
}


// Called when the game starts
void UPlayerCommandHandler::BeginPlay()
{
	Super::BeginPlay();

	//PERHAPS THE COMMANDS SHOULD BE TAKEN FROM A JSON FILE

	
}


void UPlayerCommandHandler::SendSignal() {

}

void UPlayerCommandHandler::ExecuteCommand(FString commandString ) {

	if (!mutex) {
		if (playerCommandsMap.Contains(commandString)) {

			mutex = true;
			IICommand* command = *playerCommandsMap.Find(commandString);
			command->ExecuteCommand(characterBase);



		}
	}

}
