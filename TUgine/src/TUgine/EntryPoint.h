#pragma once

#ifdef TUG_PLATFORM_WINDOWS
extern TUgine::Application* TUgine::CreateApplication();


int main(int argc, char** argv)
{
	TUgine::Log::Init();   
	auto app = TUgine::CreateApplication();
	app->Run();
	delete app;
}

#endif  
