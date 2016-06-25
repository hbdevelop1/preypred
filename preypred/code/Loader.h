
#if !defined(LOADER__INCLUDED_)
#define LOADER__INCLUDED_


class Loader
{

	Loader(){};
	Loader(const Loader &);
	Loader & operator=(const Loader &);


public:

	static Loader & GetInstance()
	{
		static Loader instance;
		return instance;
	}

	bool LoadGame(char *);

	bool SetSize(char *directory);
	bool LoadMap(char *);
	void LoadPredatorConfiguration(char *);
	void LoadPreyConfiguration(char *);
};

#endif //!defined(LOADER__INCLUDED_)
