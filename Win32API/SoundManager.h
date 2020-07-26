#pragma once
class CSoundManager
{
private:
	CSoundManager();
	~CSoundManager();

public:
	enum CHANNELID { BGM, PLAYER, MONSTER, EFFECT, UI, MAXCHANNEL };

public:
	static CSoundManager* GetInstance();
	static void DestroyInstance();

public:
	void Initialize();

	void Release();
public:
	void PlayOverlapSound(TCHAR* pSoundKey, CHANNELID eID);
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void SetVolume(float volume, CHANNELID eID);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	void LoadSoundFile();

private:
	static CSoundManager* m_instance;

	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;
};

