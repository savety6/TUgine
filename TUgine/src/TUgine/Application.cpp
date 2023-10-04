#include "tugpch.h"

#include "Application.h"

#include "TUgine/Events/ApplicationEvent.h"
#include "TUgine/Log.h"

namespace TUgine 
{

	Application::Application() 
	{

	}

	Application::~Application() 
	{
	
	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			TUG_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			TUG_TRACE(e);
		}

		while (true);
	}
}