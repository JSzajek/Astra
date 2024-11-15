#pragma once

#include <memory>

#ifdef ASTRA_PLATFORM_WINDOWS
	#if ASTRA_DYNAMIC_DLL
		#ifdef ASTRA_BUILD_DLL
			#define ASTRA_API __declspec(dllexport)
		#else
			#define ASTRA_API __declspec(dllimport)
		#endif
	#else
		#define ASTRA_API 
	#endif
#else
	#error Astra only currently supports Windows.
#endif

#ifdef ASTRA_ENABLE_ASSERTS
	#define ASTRA_ASSERT(x, ...)		{ if (!(x)) { ASTRA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define ASTRA_CORE_ASSERT(x, ...)	{ if (!(x)) { ASTRA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ASTRA_ASSERT(x, ...)
	#define ASTRA_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#if ASTRA_DEBUG || ASTRA_RELEASE
	#define RESOURCE(arg) "../Astra/" arg
#else
	#define RESOURCE(x) x
#endif

template<typename T>
using Asset = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Asset<T> CreateAsset(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

