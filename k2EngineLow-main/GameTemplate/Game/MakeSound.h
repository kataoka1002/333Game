#pragma once

/// <summary>
/// �T�E���h�̖��O
/// </summary>
enum SoundName
{
	enSoundName_TitleBGM,
	enSoundName_Num,
};

class MakeSound : public IGameObject
{
public:
	~MakeSound();
	bool Start();
	void Update();

private:

};

