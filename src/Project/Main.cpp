#include <Project/ProjectApplication.hpp>

int main(int argc, char *argv[])
{
    TUgine::Log::Init();
    TUG_CORE_ERROR("Initialized Log!");
    TUG_WARN("Initialized Log!");
    ProjectApplication application;
    application.Run();
    return 0;
}