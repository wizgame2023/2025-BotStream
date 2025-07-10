
#include "stdafx.h"
#include "Project.h"
#include <chrono>

using namespace basecross;


LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

const wchar_t* pClassName = L"EGO";
const wchar_t* pWndTitle = L"EGO";
int g_ClientWidth = 1280;
int g_ClientHeight = 800;

BOOL IsMouseCursor = FALSE;
//--------------------------------------------------------------------------------------
//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//--------------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = pClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);
	return RegisterClassEx(&wcex);
}

//--------------------------------------------------------------------------------------
//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//--------------------------------------------------------------------------------------
HWND InitInstance(HINSTANCE hInstance, int nCmdShow, bool isFullScreen, int iClientWidth, int iClientHeight)
{

	HWND hWnd = 0;
	if (isFullScreen) {
		iClientWidth = GetSystemMetrics(SM_CXSCREEN);
		iClientHeight = GetSystemMetrics(SM_CYSCREEN);
		hWnd = CreateWindow(
			pClassName,			
			pWndTitle,		
			WS_POPUP,			
			0,					
			0,					
			iClientWidth,		
			iClientHeight,		
			nullptr,			
			nullptr,			
			hInstance,			
			nullptr				
		);
		if (!hWnd) {
			MessageBox(nullptr, L"", L"", MB_OK);
			return 0;  
		}
	}
	else {
		RECT rc = { 0, 0, iClientWidth, iClientHeight };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		hWnd = CreateWindow(
			pClassName,				
			pWndTitle,				
			WS_OVERLAPPEDWINDOW,	
			CW_USEDEFAULT,			
			CW_USEDEFAULT,			
			rc.right - rc.left,		
			rc.bottom - rc.top,		
			nullptr,				
			nullptr,				
			hInstance,				
			nullptr					
		);
		if (!hWnd) {
			MessageBox(nullptr, L"", L"", MB_OK);
			return 0;  
		}
	}
	ShowCursor(IsMouseCursor);
	//ShowWindow(
	//	hWnd,      
	//	nCmdShow    
	//);
	UpdateWindow(hWnd);
	return hWnd;
}

// �t���[�����[�g����p�w���p�֐�
// lastTime: �O�t���[���̏I�[�����i�Ăяo���̂��тɍX�V�����j
// targetMs: �ڕW�t���[������ (ms)
void RegulateFrameRate(std::chrono::high_resolution_clock::time_point& lastTime,double targetMs)
{
	using clock = std::chrono::high_resolution_clock;
	// ���ݎ���
	auto now = clock::now();
	// �o�ߎ��Ԃ� ms �P�ʂŎ擾
	double elapsed = std::chrono::duration<double, std::milli>(now - lastTime).count();
	double delay = targetMs - elapsed;

	// �c�]���Ԃ�1ms�ȏ゠��� Sleep
	if (delay > 1.0) {
		Sleep(static_cast<DWORD>(delay));
	}
	// ���t���[���p�� lastTime ���X�V
	lastTime = clock::now();
}

//--------------------------------------------------------------------------------------
//	int MainLoop(HINSTANCE hInstance, HWND hWnd, bool isFullScreen, int iClientWidth, int iClientHeight);
//	�p�r: ���C�����[�v
//--------------------------------------------------------------------------------------
int MainLoop(HINSTANCE hInstance, HWND hWnd, bool isFullScreen, int iClientWidth, int iClientHeight) {
	int RetCode = 0;
	WINDOWINFO WinInfo{};
	ZeroMemory(&WinInfo, sizeof(WinInfo));

	bool comInitialized = false;
	bool appCreated = false;

	try {
		// COM ������
		if (SUCCEEDED(::CoInitialize(nullptr))) {
			comInitialized = true;
		}
		else {
			throw std::exception("CoInitialize failed");
		}

		// �A�v��������
		App::CreateApp(hInstance, hWnd, isFullScreen, iClientWidth, iClientHeight);
		appCreated = true;

		// �V�[������
		auto ScenePtr = App::GetApp()->CreateScene<Scene>();

		// ���͊Ď��L�[���X�g
		std::vector<DWORD> UseKeyVec = {
			VK_PRIOR, VK_SPACE, VK_LBUTTON, VK_RBUTTON, VK_TAB,VK_RETURN,
			VK_LEFT,VK_RIGHT,VK_UP,VK_DOWN,VK_BACK,
			'W','A','S','D',
		};

		// �t���[�����[�g���䏀��
		using clock = std::chrono::high_resolution_clock;
		const double targetMs = 1000.0 / 60.0; // 60fps
		timeBeginPeriod(1);                     // Sleep ���x������
		auto lastTime = clock::now();

		// ���C�����[�v
		MSG msg{};
		bool running = true;
		while (running) {
			// 1) Windows ���b�Z�[�W����ɏ���
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					running = false;
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (!running) break;

			// 2) ���͏�Ԃ����Z�b�g���X�V
			App::GetApp()->ResetInputState(hWnd, UseKeyVec);

			// 3) �Q�[���̍X�V���`��
			App::GetApp()->UpdateDraw(1);

			// 4) �t���[�����[�g����
			RegulateFrameRate(lastTime, targetMs);
		}

		RetCode = static_cast<int>(msg.wParam);
	}
	catch (BaseException& e) {
		LPCWSTR text = e.what_w().c_str();
		LPCWSTR title = L"Error";
		if (GetWindowInfo(hWnd, &WinInfo)) {
			MessageBox(hWnd, text, title, MB_OK);
		}
		else {
			MessageBox(nullptr, text, title, MB_OK);
		}
		RetCode = 1;
	}
	catch (BaseMBException& e) {
		const char* text = e.what_m().c_str();
		const char* title = "Error";
		if (GetWindowInfo(hWnd, &WinInfo)) {
			MessageBoxA(hWnd, text, title, MB_OK);
		}
		else {
			MessageBoxA(nullptr, text, title, MB_OK);
		}
		RetCode = 1;
	}
	catch (std::exception& e) {
		const char* text = e.what();
		const char* title = "Error";
		if (GetWindowInfo(hWnd, &WinInfo)) {
			MessageBoxA(hWnd, text, title, MB_OK);
		}
		else {
			MessageBoxA(nullptr, text, title, MB_OK);
		}
		RetCode = 1;
	}
	catch (...) {
		LPCWSTR title = L"Unknown Error";
		if (GetWindowInfo(hWnd, &WinInfo)) {
			MessageBox(hWnd, L"", title, MB_OK);
		}
		else {
			MessageBox(nullptr, L"", title, MB_OK);
		}
		RetCode = 1;
	}

	// ��n��
	if (appCreated)     App::DeleteApp();
	if (comInitialized) ::CoUninitialize();
	timeEndPeriod(1);

	return RetCode;
}


//--------------------------------------------------------------------------------------
//	int APIENTRY _tWinMain();
//	�p�r: �G���g���|�C���g
//--------------------------------------------------------------------------------------
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	setlocale(LC_ALL, "JPN");

	bool isFullScreen = false;
	wstring wstrcmd = lpCmdLine;
	if (wstrcmd == L"/f" || wstrcmd == L"/F") {
		isFullScreen = true;
	}

	MyRegisterClass(hInstance);
	HWND hWnd = InitInstance(hInstance, nCmdShow, isFullScreen, g_ClientWidth, g_ClientHeight);

	if (!hWnd)
	{
		return FALSE;
	}

	return  MainLoop(hInstance, hWnd, isFullScreen, g_ClientWidth, g_ClientHeight);

}

//--------------------------------------------------------------------------------------
//
//  �֐� : MakeWindowModeRectFunc()
//
//	���������ɂ��s��
//
//--------------------------------------------------------------------------------------
void MakeWindowModeRectFunc(RECT& rc) {
	rc = { 0, 0, g_ClientWidth, g_ClientHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	int CXWidth = GetSystemMetrics(SM_CXSCREEN);
	int CYHeight = GetSystemMetrics(SM_CYSCREEN);
	int RCWidth = rc.right - rc.left;
	int RCHeight = rc.bottom - rc.top;
	int OffsetLeft = CXWidth / 2 - RCWidth / 2;
	int OffsetTop = CYHeight / 2 - RCHeight / 2;
	OffsetRect(&rc, OffsetLeft, OffsetTop);
}

//--------------------------------------------------------------------------------------
//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:    ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
//
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		case VK_RETURN:
			if (GetAsyncKeyState(VK_CONTROL)) {
				if (App::AppCheck()) {
					if (App::GetApp()->IsFullScreen()) {
						RECT rc;
						MakeWindowModeRectFunc(rc);
						App::GetApp()->SetWindowMode(rc);
					}
					else {
						App::GetApp()->SetFullScreenMode();
					}
				}
			}
			break;
		}
		break;
	case WM_SIZE:
		if (App::AppCheck()) {
			App::GetApp()->OnSize();
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
