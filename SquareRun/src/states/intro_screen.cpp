#include <states/intro_screen.h>
#include <core/input_system.h>

void IntroScreen::Init()
{
	this->music = AudioSystem::GetInstance().LoadAudioFromFile("title_screen.wav");
	this->music->Play();
}

void IntroScreen::Destroy()
{

}

void IntroScreen::Update(const double& deltaTime)
{

}

void IntroScreen::Render() const
{
	
}

IntroScreen* IntroScreen::GetGameState()
{
	static IntroScreen gameState;
	return &gameState;
}