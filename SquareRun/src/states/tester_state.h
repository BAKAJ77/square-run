#ifndef TESTER_STATE_H
#define TESTER_STATE_H

#include <core/game_state.h>

class TesterState : public GameState
{
protected:
	void Init() override;
	void Destroy() override;

	void Update(const double& deltaTime) override;
	void Render() const override;
public:
	static TesterState* GetGameState();
};

#endif