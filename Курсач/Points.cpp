#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "Points.h"

typedef struct
{
	TCHAR player[15];
	int point;
} record;

INT Kol = 0;
record *TABLICA = (record*)malloc(sizeof(record)*Kol);

int pointsRecord;
bool check;

void OpenFileRecords();
void SaveFileRecords();

#define ID_BUTTON_OK 3001
#define ID_BUTTON_CANCEL 3002
#define ID_EDIT_NAME 3003

int RegisterParentWindowClass(void);

void recPoints(int points)
{
	pointsRecord = points;
	Kol = 0;
	record *TABLICA = (record*)malloc(sizeof(record)*Kol);
	WinMain(NULL, NULL, NULL, NULL);
}

LRESULT CALLBACK ParentWndProc(HWND, UINT, WPARAM, LPARAM);

char ParentClassName[] = "ParentWindowClass";
HINSTANCE hInstance;
HWND hButtonOk, hButtonCancel, hEditPlayer, hwnd_main;
HDC hdc;
PAINTSTRUCT ps;
HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0,
	DEFAULT_CHARSET,
	0, 0, 0, VARIABLE_PITCH,
	(LPCWSTR)TEXT("Arial Bold"));

INT num = -1;
TCHAR buffer[100];
CHAR szSymbol[1];
DWORD amount = 1;

int RegisterParentWindowClass(void)
{
	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = ParentWndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszMenuName = (LPCWSTR)NULL;
	wc.lpszClassName = (LPCWSTR)ParentClassName;

	return RegisterClass(&wc);
}

int WINAPI WinMain(HINSTANCE hIns, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	hInstance = hIns;
	if (!RegisterParentWindowClass()) return -1;

	hwnd_main = CreateWindow((LPCWSTR)ParentClassName, TEXT("Таблица рекордов"),
		WS_SYSMENU | WS_OVERLAPPED | WS_BORDER,
		0, 0, 340, 400, NULL, NULL, hInstance, 0);

	if (!hwnd_main) return 0;
	ShowWindow(hwnd_main, SW_SHOW);
	UpdateWindow(hwnd_main);

	MSG msg;
	bool warning = false;
	while (GetMessage(&msg, 0, 0, 0)) {
		switch (msg.message)
		{
		case WM_CHAR:
			if (msg.wParam >= 65 && msg.wParam <= 122 || msg.wParam == 8 || msg.wParam >= 48 && msg.wParam <= 57) {}
			else 
			{
				SetFocus(hEditPlayer);
				warning = true;
			}

		}
		if (warning == false)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		warning = false;
	}

	DestroyWindow(hwnd_main); // Уничтожаем основное окно
	UnregisterClass((LPWSTR)ParentClassName, NULL); // Освобождаем память, отменяя регистрацию класса основного окна.
	return msg.wParam;
}

LRESULT CALLBACK ParentWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE: {
		hButtonOk = CreateWindow(TEXT("button"), TEXT("Ок"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			150, 330, 50, 25, hwnd, (HMENU)ID_BUTTON_OK, hInstance, 0);
		hButtonCancel = CreateWindow(TEXT("button"), TEXT("Отмена"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			220, 330, 80, 25, hwnd, (HMENU)ID_BUTTON_CANCEL, hInstance, 0);
	}break;
	case WM_PAINT:
	{
		check = false;
		OpenFileRecords();
		TCHAR buff[25];
		hdc = BeginPaint(hwnd, &ps);
		SetBkMode(hdc, DFCS_TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 0));
		SelectObject(hdc, font);
		TextOut(hdc, 7, 5, TEXT("№"), 1);
		TextOut(hdc, 80, 5, TEXT("Игрок"), 5);
		TextOut(hdc, 200, 5, TEXT("Рекорд"), 6);
		for (int i = 1, j = 0; i <= 10; i++)
		{
			wsprintf(buff, TEXT("%d"), i);
			TextOut(hdc, 10, i * 30, (LPCWSTR)buff, wcslen(buff));
			if (i <= Kol)
			{
				if (pointsRecord >= TABLICA[j].point && check == false)
				{
					if(Kol != 10)Kol++;
					check = true;
					hEditPlayer = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_TEXT | ES_AUTOHSCROLL,
						80, i * 30 - 5, 100, 25, hwnd, (HMENU)ID_EDIT_NAME, hInstance, 0);
					SendMessage(hEditPlayer, EM_LIMITTEXT, 10, 0L);
					wsprintf(buff, TEXT("%d"), pointsRecord);
					buff[wcslen(buff)] = '\0';
					TextOut(hdc, 200, i * 30, (LPCWSTR)buff, wcslen(buff));
					i++;
					if (i <= Kol)
					{
						wsprintf(buff, TEXT("%d"), i);
						TextOut(hdc, 10, i * 30, (LPCWSTR)buff, wcslen(buff));
						wcscpy_s(buff, TABLICA[j].player);
						buff[wcslen(buff)] = '\0';
						TextOut(hdc, 80, i * 30, buff, wcslen(buff));

						wsprintf(buff, TEXT("%d"), TABLICA[j++].point);
						buff[wcslen(buff)] = '\0';
						TextOut(hdc, 200, i * 30, (LPCWSTR)buff, wcslen(buff));
					}
				}
				else
				{
					wcscpy_s(buff, TABLICA[j].player);
					buff[wcslen(buff)] = '\0';
					TextOut(hdc, 80, i * 30, buff, wcslen(buff));

					wsprintf(buff, TEXT("%d"), TABLICA[j++].point);
					buff[wcslen(buff)] = '\0';
					TextOut(hdc, 200, i * 30, (LPCWSTR)buff, wcslen(buff));
				}
			}
			else
			{
				if (i == 1 || check == false)
				{
					check = true;
					hEditPlayer = CreateWindow(TEXT("edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | BS_TEXT | ES_AUTOHSCROLL,
						80, i * 30, 100, 25, hwnd, (HMENU)ID_EDIT_NAME, hInstance, 0);
					SendMessage(hEditPlayer, EM_LIMITTEXT, 10, 0L);
					wsprintf(buff, TEXT("%d"), pointsRecord);
					buff[wcslen(buff)] = '\0';
					TextOut(hdc, 200, i * 30, (LPCWSTR)buff, wcslen(buff));
					Kol++;
				}
				else
				{
					wsprintf(buff, TEXT("%ls"), TEXT("Пусто"));
					TextOut(hdc, 80, i * 30, (LPCWSTR)buff, 5);

					wsprintf(buff, TEXT("%d"), 0);
					TextOut(hdc, 200, i * 30, (LPCWSTR)buff, wcslen(buff));
				}
			}
		}
		if (check == false)
		{
			wsprintf(buff, TEXT("%ls"), TEXT("Увы! Старайся :)"));
			buff[17] = '\0';
			TextOut(hdc, 7, 11 * 30, (LPCWSTR)buff, 20);
		}
		EndPaint(hwnd, &ps);
	}break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_BUTTON_OK: {
			if (check == true)
			{
				GetWindowText(hEditPlayer, (LPWSTR)buffer, 10);
				if (wcslen(buffer) != 0)
				{
					TABLICA = (record*)realloc(TABLICA, sizeof(record)*(Kol + 1));
					wcscpy_s(TABLICA[Kol-1].player, buffer);
					TABLICA[Kol-1].point = pointsRecord;
					TABLICA = (record*)realloc(TABLICA, sizeof(record)*(Kol + 1));
					for (int i = 0; i < Kol - 1; i++) {
						for (int j = 0; j < Kol - i - 1; j++) {
							if (TABLICA[j].point < TABLICA[j + 1].point) {
								TABLICA[Kol] = TABLICA[j];
								TABLICA[j] = TABLICA[j + 1];
								TABLICA[j + 1] = TABLICA[Kol];
							}
						}
					}
					if (Kol == 11) Kol--;
					SaveFileRecords();
					PostQuitMessage(0);
				}
				else
				{
					MessageBox(hwnd, TEXT("Введите ник!"), TEXT("Ошибка"), MB_OK);
				}
			}
			else
			{
				PostQuitMessage(0);
			}
			break;
		}
		case ID_BUTTON_CANCEL: {
			if (check = true) Kol--;
			PostQuitMessage(0);
			break;
		}
		case ID_EDIT_NAME:
		{
			break;
		}
		}
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default: return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

void OpenFileRecords()
{
	std::wifstream file(L"record/record.txt", std::ios_base::app);
	file >> Kol;
	if (Kol < 1 || Kol > 10) Kol = 0;
	free(TABLICA);
	TABLICA = (record*)malloc(sizeof(record)*(Kol));
	for (int i = 0; i < Kol; i++) {
		file >> TABLICA[i].player;
		file >> TABLICA[i].point;
	}
	file.close();
}

void SaveFileRecords()
{
	std::wofstream file(L"record/record.txt", std::ios_base::out | std::ios_base::trunc);
	file << Kol << '\n';
	for (int i = 0; i < Kol; i++) {
		file << TABLICA[i].player << '\n' << TABLICA[i].point << '\n';
	}
	file.close();
}

