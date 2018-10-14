#pragma once





class Delegate
{

public:
	Delegate()
	{}
	~Delegate()
	{}

	
	void Bind(Object* VObject,void (&Function)())
	{
		//fn2 = &Call<Object, Function>;
		//Instance = VObject;
	}

	template<class C, void (C::*Function)(int i)>
	 void  Bind(C *instance)
	{

		fn2 = &Call<C, Function>;
		Instance = instance;
	}



	
	template<class C, void (C::*Function)()>
	 void Bind(C *instance)
	{

		
		fn = &Call<C,Function>;
		Instance = instance;
	}

	 void UnBind()
	{
		fn = NULL;
		fn = 0;
		Instance = 0;
		
	}

	//CallEvent
	void Execute() {
		if (fn && Instance)
		(fn)(Instance);
	}

	void Execute(int ID) {
		if (fn2 && Instance)
			(fn2)(Instance,ID);
	}

	
private:
	
	template<class C, void (C::*f)()>
	static void Call(void* instance)
	{
		(static_cast<C*>(instance)->*f)();
	}

	template<class C, void (C::*f)(int)>
	static void Call(void* instance,int ID)
	{
		(static_cast<C*>(instance)->*f)(ID);
	}

	
	
private:
	
	void*	   Instance;
	using Fn = void(*)(void*);
	using Fn2 = void(*)(void*,int);
	Fn fn;
	Fn2 fn2;
	};
