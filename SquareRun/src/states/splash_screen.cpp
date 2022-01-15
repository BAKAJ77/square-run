#include <states/splash_screen.h>
#include <core/input_system.h>

void SplashScreen::Init()
{
	this->texture = Memory::LoadTextureFromFile("transition_bkg.png");
	this->font = Memory::LoadFontFromFile("cascadia_code_bold.ttf", 100);

	this->collider1 = RectangleCollider(this->pos, { 100, 100 });
	this->collider2 = RectangleCollider({ 500, 500 }, { 200, 200 });
}

void SplashScreen::Destroy()
{

}

void SplashScreen::Update(const double& deltaTime)
{
	if (InputSystem::GetInstance().WasKeyPressed(KeyCode::KEY_ESCAPE))
		this->PopState();

	if (InputSystem::GetInstance().WasKeyPressed(KeyCode::KEY_W))
		this->pos.y -= 500 * (float)deltaTime;
	else if (InputSystem::GetInstance().WasKeyPressed(KeyCode::KEY_S))
		this->pos.y += 500 * (float)deltaTime;
	if (InputSystem::GetInstance().WasKeyPressed(KeyCode::KEY_A))
		this->pos.x -= 500 * (float)deltaTime;
	else if (InputSystem::GetInstance().WasKeyPressed(KeyCode::KEY_D))
		this->pos.x += 500 * (float)deltaTime;

	if (this->collider1.IsColliding(this->collider2) == CollidingSide::TOP)
		std::printf("Colliding with top side.\n");
	else if (this->collider1.IsColliding(this->collider2) == CollidingSide::BOTTOM)
		std::printf("Colliding with bottom side.\n");
	else if (this->collider1.IsColliding(this->collider2) == CollidingSide::LEFT)
		std::printf("Colliding with left side.\n");
	if (this->collider1.IsColliding(this->collider2) == CollidingSide::RIGHT)
		std::printf("Colliding with right side.\n");

	this->collider1.SetPosition(this->pos);
}

void SplashScreen::Render() const
{
	Renderer::GetInstance().RenderRect(this->camera, { 255, 255, 0, 255 }, this->pos, { 100, 100 });
	Renderer::GetInstance().RenderRect(this->camera, { 255, 0, 255, 255 }, { 500, 500 }, { 200, 200 });
}

SplashScreen* SplashScreen::GetGameState()
{
	static SplashScreen gameState;
	return &gameState;
}