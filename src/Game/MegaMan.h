#ifndef MegaMan_CHAR_H
#define MegaMan_CHAR_H

#pragma once

#include <map>
#include <string>

#include "Game/Character.h"


class MegaMan : public GameCharacter
{
public:

	virtual void InitGameCharacter() override;
	virtual void UpdateGameCharacter(float deltaTime) override;
	virtual void DrawGameCharacter() override;
	virtual void UnloadGameCharacter() override;

	//State Machine
	virtual void updateState();
	// This will get called by the current state
	virtual void setState(CharacterState& newState);



	float GetMegaManJumpSpeed() { return m_MegaManJump_Speed; }
	float GetMegaManHorzontalSpeed() { return m_MegaMan_Horizontal_Speed; }
	float GetMegaManVerticalSpeed() { return m_MegaMan_Vertical_Speed; }

	float GetMegaManCurrentSpeed() { return m_Speed; }
	void  SetMegaManCurrentSpeed(float newSpeed) { m_Speed = newSpeed; }

	bool GetMegaManCanJump() { return m_bcanJump; }
	void SetMegaManCanJump(bool newCanJump) { m_bcanJump = newCanJump; }


private:

	const float m_MegaManJump_Speed = 250.f;
	const float m_MegaMan_Horizontal_Speed = 100.f;
	const float m_MegaMan_Vertical_Speed = 100.f;

	float m_Speed = {0.f};
	bool m_bcanJump = false;

};

#endif
