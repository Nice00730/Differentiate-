#include "framework.h"
#include "Main.h"

using namespace std;

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Settings(HWND, UINT, WPARAM, LPARAM);

RECT Wrect; 
RECT ScreenSize;
HBITMAP hBmp1 = NULL;
HBITMAP hBmp2 = NULL;

HCURSOR hOldCursor = NULL;

COLORREF* Pict1;
COLORREF* Pict2;

bool frst = true;
int pict1w = 0;
int pict1h = 0;
int pict2w = 0;
int pict2h = 0;
int windw = 300;
int windh = 200;
wchar_t debug[256];
wchar_t File1N[256];
wchar_t File2N[256];

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GENAL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GENAL));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GENAL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GENAL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, pict1w+15, pict1h+59, nullptr, nullptr, hInstance, nullptr);

   SystemParametersInfo(SPI_GETWORKAREA, 0, &ScreenSize, 0);
   GetWindowRect(hWnd, &Wrect);

   SetTimer(hWnd, 0, 3, nullptr);
   srand((unsigned)time(0));

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void readHWbmp(wchar_t* _1, int* _2, int* _3)
{
	ifstream in(_1, ios::binary);

	for (int i = 0; i < 4; i++)
	{
		in.read((char*)_2, 4);
	}

	in.read((char*)_2, 2);

	*_2 = 0x0;
	in.read((char*)_2, 2);

	in.read((char*)_3, 2);

	*_3 = 0x0;
	in.read((char*)_3, 2);

	in.close();

	return;
}

void DelStr(wchar_t* _1, int size)
{
	for (int i = 0; i < size; i++)
	{
		_1[i] = 0x0;
	}

	return;
}

void CalcWinWH(int* _1, int* _2)
{
	if (pict1h > pict2h)
	{
		*_1 = pict1w + pict2w + 15;
		*_2 = pict1h;
	}
	else
	{
		*_1 = pict1w + pict2w + 15;
		*_2 = pict2h;
	}

	return;
}

char LoadPicts(HWND* hWnd)
{
	DelStr(File1N, 256);
	DelStr(File2N, 256);
	pict1w = 0;
	pict1h = 0;
	pict2w = 0;
	pict2h = 0;
	hBmp1 = NULL;
	hBmp2 = NULL;

	OPENFILENAME OFN;

	ZeroMemory(&OFN, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = *hWnd;
	OFN.lpstrFile = (LPWSTR)&File1N;
	OFN.nMaxFile = sizeof(File1N);
	OFN.lpstrFilter = L"Image files\0*.bmp\0";
	OFN.nFilterIndex = 1;
	OFN.lpstrFileTitle = NULL;
	OFN.nMaxFileTitle = 0;
	OFN.lpstrInitialDir = NULL;
	OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&OFN))
	{
		frst = false;
		readHWbmp(File1N, &pict1w, &pict1h);

		hBmp1 = (HBITMAP)LoadImage(NULL, File1N, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		Pict1 = new COLORREF[pict1w*pict1h];

		CalcWinWH(&windw, &windh);

		OFN.lpstrFile = (LPWSTR)&File2N;
		OFN.nMaxFile = sizeof(File2N);

		if (GetOpenFileName(&OFN))
		{
			readHWbmp(File2N, &pict2w, &pict2h);

			hBmp2 = (HBITMAP)LoadImage(NULL, File2N, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			Pict2 = new COLORREF[pict2w*pict2h];

			CalcWinWH(&windw, &windh);

		}
		else
		{
			return 0x1;
		}

		return 0x2;
	}

	return 0x0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			SystemParametersInfo(SPI_GETWORKAREA, 0, &ScreenSize, 0);
			SetWindowPos(hWnd, NULL, ScreenSize.right / 2-150, ScreenSize.bottom / 2-100, 300, 200, NULL);
			windw = 300;
			windh = 200;
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
			case IDM_SETSIZE:
				SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, NULL);
				if (hBmp1 || hBmp2)
				{
					SetWindowPos(hWnd, NULL, ScreenSize.right / 2 - (pict1w + pict2w + 15) / 2, ScreenSize.bottom / 2 - (windh + 59) / 2, pict1w + pict2w + 15, windh + 59 + 30, NULL);
				}
				else
				{
					SetWindowPos(hWnd, NULL, ScreenSize.right / 2 - 150, ScreenSize.bottom / 2 - 100, 300, 200, NULL);

					windw = 300;
					windh = 200;
				}
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case IDM_SETTINGS:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTINGS), hWnd, Settings);
				break;
			case IDM_OPENPICT:
				if (LoadPicts(&hWnd))
				{
					SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, NULL);
					SetWindowPos(hWnd, NULL, ScreenSize.right / 2 - (pict1w + pict2w + 15) / 2, ScreenSize.bottom / 2 - (windh + 59) / 2, pict1w + pict2w + 15, windh + 59 + 30, NULL);
				}
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
			SystemParametersInfo(SPI_GETWORKAREA, 0, &ScreenSize, 0);
			GetWindowRect(hWnd, &Wrect);

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			RECT TextRect;

			HDC bmpDC = CreateCompatibleDC(hdc);

			SelectObject(bmpDC, hBmp1);

			int tmpi = 0;
			int tmpy = pict1h;

			if (pict1h < pict2h)
			{
				tmpi = windh / 2 - pict1h / 2;
				tmpy = pict2h / 2 - pict1h / 2 + pict1h;
			}

			TextRect.left = 5;
			TextRect.top = tmpi + 5;

			if (!frst && hBmp1 != NULL)
			{
				hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

				swprintf_s(debug, L"%s | Loading...", File1N);
				DrawText(hdc, debug, -1, &TextRect, DT_NOCLIP);

				for (int i = 0; i < pict1h; i++)
				{
					for (int j = 0; j < pict1w; j++)
					{
						Pict1[i*pict1w + j] = GetPixel(bmpDC, j, i);
					}
				}

				SetCursor(hOldCursor);
			}

			BitBlt(hdc, 0, tmpi, pict1w, tmpy, bmpDC, 0, 0, SRCCOPY);

			if (hBmp1)
			{
				swprintf_s(debug, L"%s | Loaded", File1N);
				DrawText(hdc, debug, -1, &TextRect, DT_NOCLIP);
			}

			SelectObject(bmpDC, hBmp2);

			tmpi = 0;
			tmpy = pict2h;

			if (pict1h > pict2h)
			{
				tmpi = windh / 2 - pict2h / 2;
				tmpy = pict1h / 2 - pict2h / 2 + pict2h;
			}

			TextRect.left = pict1w + 5;
			TextRect.top = tmpi + 5;

			if (!frst && hBmp2 != NULL)
			{
				hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

				swprintf_s(debug, L"%s | Loading...", File2N);
				DrawText(hdc, debug, -1, &TextRect, DT_NOCLIP);

				for (int i = 0; i < pict2h; i++)
				{
					for (int j = 0; j < pict2w; j++)
					{
						Pict2[i*pict2w + j] = GetPixel(bmpDC, j, i);
					}
				}

				SetCursor(hOldCursor);
			}

			BitBlt(hdc, pict1w, tmpi, pict1w + pict2w, tmpy, bmpDC, 0, 0, SRCCOPY);

			if (hBmp2)
			{
				swprintf_s(debug, L"%s | Loaded", File2N);
				DrawText(hdc, debug, -1, &TextRect, DT_NOCLIP);
			}

			if (!frst)
			{
				TextRect.left = 5;
				TextRect.top = windh+5;

				if (hBmp1 != NULL && hBmp2 != NULL)
				{
					hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
					swprintf_s(debug, L"Comparing...");
					DrawText(hdc, debug, -1, &TextRect, DT_NOCLIP);

					bool Acce = true;

					wchar_t error[256]{L" "};
					wchar_t overlap[256]{L" "};

					if (pict2h == pict1h && pict1w == pict2w)
					{
						char rnum = 0;
						int was[50];
						
						for (int i = 0; i < 50; i++)
						{
							bool stop = true;
							int now = 0;

							while (stop)
							{
								now = rand() % pict1h * pict1w;
								stop = false;

								for (int j = 0; j < 50; j++)
								{
									if (now == was[j])
									{
										stop = true;
										break;
									}
								}
							}

							was[i] = now;

							stop = false;

							for (int j = 0; j < pict1w; j++)
							{
								if (Pict1[now + j] != Pict2[now + j])
								{
									stop = true;
									break;
								}
							}

							if (!stop)
							{
								rnum++;
							}
						}

						if (int(float(rnum) / 50.0 * 100) < 60)
						{
							swprintf_s(overlap, L"overlap - %i%% < 60%% (not the same pict)", int(float(rnum) / 50.0 * 100));
						}
						else 
						{
							swprintf_s(overlap, L"overlap - %i%% > 60%% (same pict)", int(float(rnum) / 50.0 * 100));
						}	
					}
					else
					{
						swprintf_s(error, L"not the same size");
						Acce = false;
					}

					swprintf_s(debug, L"                                                                      ");
					DrawText(hdc, debug, -1, &TextRect, DT_NOCLIP);

					if (Acce)
					{
						swprintf_s(debug, L"Compare: %s %s", overlap, error);
						DrawText(hdc, debug, -1, &TextRect, DT_NOCLIP);
					}
					else
					{
						swprintf_s(debug, L"Fail: %s", error);
						DrawText(hdc, debug, -1, &TextRect, DT_NOCLIP);
					}

					SetCursor(hOldCursor);
				}
				else
				{
					swprintf_s(debug, L"Nothing compare");
					DrawText(hdc, debug, -1, &TextRect, DT_NOCLIP);
				}

				frst = true;
			}

			DeleteDC(bmpDC);

			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Settings(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}