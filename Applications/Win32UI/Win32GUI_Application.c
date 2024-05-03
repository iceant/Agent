#include <Win32GUI_Application.h>

////////////////////////////////////////////////////////////////////////////////
////
static char szAppName[]="Window1";
static HWND MainWindow;

////////////////////////////////////////////////////////////////////////////////
////
LRESULT CALLBACK WndProc(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam);

BOOL Register(HINSTANCE hInstance);

HWND Create(HINSTANCE hInstance, int nCmdShow);


////////////////////////////////////////////////////////////////////////////////
////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    MSG Msg;
    
    if(!hPrevInstance){
        if(!Register(hInstance)){
            return FALSE;
        }
    }
    MainWindow = Create(hInstance, nCmdShow);
    if(!MainWindow){
        return FALSE;
    }
    
    while(GetMessage(&Msg, NULL, 0, 0)){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

////////////////////////////////////////////////////////////////////////////////
////

BOOL Register(HINSTANCE hInstance)
{
    WNDCLASS WndClass;
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = szAppName;
    
    return (RegisterClass(&WndClass)!=0);
}

////////////////////////////////////////////////////////////////////////////////
////
HWND Create(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWindow = CreateWindowEx(0, szAppName, szAppName,
                                  WS_OVERLAPPEDWINDOW,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  NULL, NULL, hInstance, NULL
                                  );
    if(hWindow == NULL){
        return hWindow;
    }
    
    ShowWindow(hWindow, nCmdShow);
    UpdateWindow(hWindow);
    
    return hWindow;
}

////////////////////////////////////////////////////////////////////////////////
////
static void Window1_OnDestroy(HWND hWnd){
    PostQuitMessage(0);
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message){
        HANDLE_MSG(hWindow, WM_DESTROY, Window1_OnDestroy);
        default:
            return DefWindowProc(hWindow, Message, wParam, lParam);
    }
}
