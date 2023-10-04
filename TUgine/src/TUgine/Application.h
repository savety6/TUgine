#pragma once
#include "Core.h"

#include "Events/Event.h"

namespace TUgine
{
	class TUGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}

