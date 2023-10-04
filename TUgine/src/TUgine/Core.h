#pragma once

#ifdef TUG_PLATFORM_WINDOWS
	#ifdef TUG_BUILD_DLL 
		#define TUGINE_API __declspec(dllexport)
	#else
		#define TUGINE_API __declspec(dllimport)
	#endif 
#else
	#error Only support Windows for now!
#endif 

#define BIT(x) (1 << x)