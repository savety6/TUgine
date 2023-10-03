#pragma once
#include "Core.h"

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

