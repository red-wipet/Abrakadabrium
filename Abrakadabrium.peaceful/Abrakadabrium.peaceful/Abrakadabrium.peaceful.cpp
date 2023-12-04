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

DWORD WINAPI payload1(LPVOID lpParam) {
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
DWORD WINAPI payload2(LPVOID lpParam) {
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
DWORD WINAPI payload3(LPVOID lpParam) {
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
DWORD WINAPI payload4(LPVOID lpParam) {
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
DWORD WINAPI payload5(LPVOID lpParam) {
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
DWORD WINAPI payload6(LPVOID lpParam) {
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
DWORD WINAPI payload7(LPVOID lpParam) {
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
DWORD WINAPI payload8(LPVOID lpParam) {
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
DWORD WINAPI payload9(LPVOID lpParam) {
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
DWORD WINAPI payload10(LPVOID lpParam) {
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
DWORD WINAPI payload11(LPVOID lpParam) {
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
DWORD WINAPI payload12(LPVOID lpParam) {
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
DWORD WINAPI payload13(LPVOID lpParam) {
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
DWORD WINAPI payload14(LPVOID lpParam) {
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
DWORD WINAPI payload15(LPVOID lpParam) {
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
		"This software is safe to run this on your real machine\r\n"
		"it won't damage your system and don't overwrite your bootloader or MBR\r\n"
		"(Master boot record).\r\n", L"MALWARE WARNING!!!", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"          LAST WARNING          \r\nIT'S SAFE VERSION :)", L"THIS IS YOUR LAST WARNING!!!", MB_YESNO | MB_ICONWARNING) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
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
		}
	}
}