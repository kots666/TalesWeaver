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

	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;
};

