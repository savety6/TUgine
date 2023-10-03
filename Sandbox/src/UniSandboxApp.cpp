#include <TUgine.h>


class USandbox : public TUgine::Application
{
public:
	USandbox()
	{
		TUG_CORE_INFO("init");
	}
	~USandbox()
	{

	}
};

TUgine::Application* TUgine::CreateApplication()
{
	return new USandbox();
}

