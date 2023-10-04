#include <TUgine.h>


class Sandbox : public TUgine::Application
{
public:
	Sandbox()
	{
		//TUG_CORE_INFO("init");
	}
	~Sandbox()
	{

	}
};

TUgine::Application* TUgine::CreateApplication()
{
	return new Sandbox();
}

