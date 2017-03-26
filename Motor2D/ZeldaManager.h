#ifndef _ZELDAMANAGER_H_
#define _ZELDAMANAGER_H_

class Zelda;

class ZeldaManager
{
public:
	ZeldaManager();
	~ZeldaManager();

	void CleanUp();

private:

public:

private:
	Zelda*	zelda = nullptr;

};

#endif
