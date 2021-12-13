#ifdef _WIN32

#include <Windows.h>

void nvp_app_init();
int nvp_entry_main(HINSTANCE hInstance);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    nvp_app_init();
    return nvp_entry_main(hInstance);
}

#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef __APPLE__

void nvp_app_init();
int nvp_entry_main(int argc, const char* argv[]);

int main(int argc, const char * argv[])
{
    nvp_app_init();
    return nvp_entry_main(argc, argv);
}

#endif
