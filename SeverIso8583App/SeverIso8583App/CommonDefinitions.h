#pragma once
#define WM_SOCKET (WM_USER + 1)
#define WSAGETSELECTEVENT(lParam)       LOWORD(lParam)
#define WSAGETSELECTERROR(lParam)       HIWORD(lParam)
#define DEFAULT_BUFLEN 512