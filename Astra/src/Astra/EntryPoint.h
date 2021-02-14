#pragma once

#ifdef ASTRA_PLATFORM_WINDOWS

extern Astra::Application* Astra::CreateApplication();

int main(int argc, char** argv)
{
	Astra::Log::Init();
	ASTRA_CORE_INFO("Initialized Log!");
	
	auto app = Astra::CreateApplication();
	
	app->Run();

	delete app;
	return 0;
}

#endif