#include "SNFSMAPDootState.h"
#include "SNAnimator.h"
#include "SNFSMData.h"
#include "SNWorld.h"

void SNFSMAPDootState::Enter(SNFSMData* fsmData)
{
	if (fsmData->stateMachine->previousStateIndex == DOOT_STATE)
	{
		fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->dootAnim, 9);
		dootDuration = 8 * fsmData->world->dootAnim->nextFrameDelay;
	}
	else
	{
		fsmData->autonomousProxy->animator->SetCurrentAnimation(fsmData->world->dootAnim);
		dootDuration = fsmData->world->dootAnim->duration;
	}

	timer = 0.0f;
	packet.flag = DOOT_FLAG;
	reDoot = false;
}

void SNFSMAPDootState::Update(float dt, SNFSMData* fsmData)
{
	SNInput* input = fsmData->input;

	timer += dt;

	if (input->jump)
	{
		packet.dootFlag = 0;
		fsmData->world->dootAnim->sprites[9]->audio = fsmData->world->audioManager->dootSound1;
		reDoot = true;
		return;
	}

	if (input->bBtn)
	{
		packet.dootFlag = 1;
		fsmData->world->dootAnim->sprites[9]->audio = fsmData->world->audioManager->dootSound2;
		reDoot = true;
		return;
	}

	if (input->attack)
	{
		packet.dootFlag = 2;
		fsmData->world->dootAnim->sprites[9]->audio = fsmData->world->audioManager->dootSound3;
		reDoot = true;
		return;
	}

	if (input->yBtn)
	{
		packet.dootFlag = 3;
		fsmData->world->dootAnim->sprites[9]->audio = fsmData->world->audioManager->dootSound4;
		reDoot = true;
		return;
	}

	if (reDoot)
	{
		fsmData->autonomousProxy->EnterState(DOOT_STATE);

		if (fsmData->world->HasAuthority())
		{
			fsmData->world->server.SendData(&packet);
		}
		else
		{
			fsmData->world->client.SendData(&packet);
		}
	}

	if (timer >= dootDuration)
	{
		fsmData->autonomousProxy->EnterState(IDLE_STATE);
	}
}

void SNFSMAPDootState::Exit(SNFSMData* fsmData)
{
	
}
