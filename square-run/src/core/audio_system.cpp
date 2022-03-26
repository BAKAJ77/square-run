#include <core/audio_system.h>
#include <util/logging_system.h>
#include <util/directory_system.h>

#include <algorithm>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AudioSystem::AudioSystem()
{
	this->engine = irrklang::createIrrKlangDevice();
	if (!this->engine)
		LogSystem::GetInstance().OutputLog("Failed to create IrrKlang device engine", Severity::FATAL);
}

AudioSystem::~AudioSystem()
{
	this->engine->drop();
}

void AudioSystem::SetMasterVolume(float volume)
{
	this->engine->setSoundVolume(volume);
}

GlobalAudioPtr AudioSystem::LoadAudioFromFile(const std::string_view& fileName)
{
	// Construct the full path to the audio file
	const std::string filePath = Util::GetGameRequisitesDirectory() + "assets/" + fileName.data();

	// Load the audio file
	irrklang::ISoundSource* loadedAudio = this->engine->addSoundSourceFromFile(filePath.c_str(), irrklang::ESM_AUTO_DETECT, true);
	return std::make_shared<GlobalAudio>(this->engine, loadedAudio);
}

AudioSystem& AudioSystem::GetInstance()
{
	static AudioSystem instance;
	return instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GlobalAudio::GlobalAudio(irrklang::ISoundEngine* engine, irrklang::ISoundSource* audio) :
	engine(engine), audio(audio), channel(nullptr)
{
	this->engine->grab(); // We get a pointer to the engine, so increase the ref count to the engine
}

GlobalAudio::~GlobalAudio()
{
	this->Stop();

	this->engine->removeSoundSource(this->audio);
	this->engine->drop();
}

void GlobalAudio::SetPlayPosition(uint32_t milliseconds)
{
	if (this->channel)
		this->channel->setPlayPosition(milliseconds);
}

void GlobalAudio::SetVolume(float volume)
{
	if (this->channel)
		this->channel->setVolume(std::clamp(volume, 0.0f, 1.0f));
}

void GlobalAudio::Play(bool loop)
{
	// Drop previously used sound channel
	if (this->channel)
	{
		this->channel->stop();
		this->channel->drop();
	}

	// Play the audio
	this->channel = this->engine->play2D(this->audio, loop, false, true, false);
}

void GlobalAudio::Stop()
{
	if (this->channel)
	{
		this->channel->stop();
		this->channel->drop();
	}
}

void GlobalAudio::Resume()
{
	if (this->channel)
		this->channel->setIsPaused(false);
}

void GlobalAudio::Pause()
{
	if (this->channel)
		this->channel->setIsPaused(true);
}

uint32_t GlobalAudio::GetPlayPosition() const
{
	if (this->channel)
		return this->channel->getPlayPosition();

	return 0;
}

float GlobalAudio::GetVolume() const
{
	if (this->channel)
		return this->channel->getVolume();

	return 1.0f;
}

bool GlobalAudio::isPaused() const
{
	if (this->channel)
		return this->channel->getIsPaused();

	return false;
}

bool GlobalAudio::isFinished() const
{
	if (this->channel)
		return this->channel->isFinished();

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////