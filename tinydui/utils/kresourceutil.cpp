#include "kpch.h"
#include "kresourceutil.h"

namespace TinyDui 
{

DWORD GetResourceStream (
	__in UINT uId, 
	__in LPCWSTR lpszResType, 
	__out IStream** pStream
	)
{
	if ( lpszResType == NULL || pStream == NULL ) {
		return ERROR_INVALID_PARAMETER;
	}
	*pStream = NULL;

	HRSRC hSource = ::FindResourceW(
		g_hResourceInstance, 
		MAKEINTRESOURCEW(uId), 
		lpszResType
		);
	if ( hSource == NULL ) {
		return ::GetLastError();
	}

	HGLOBAL hGlobalRes = ::LoadResource(
		g_hResourceInstance, 
		hSource
		);
	if ( hGlobalRes == NULL ) {
		::FreeResource(hGlobalRes);
		return ::GetLastError();
	}

	LPVOID lpVoid = ::LockResource(hGlobalRes);
	if ( lpVoid == NULL ) {
		::FreeResource(hGlobalRes);
		return ::GetLastError();
	}

	UINT nSize = (UINT)SizeofResource(
		g_hResourceInstance, 
		hSource
		);
	HGLOBAL hAlloc = ::GlobalAlloc(GMEM_MOVEABLE, nSize);
	if ( hAlloc == NULL ) {
		UnlockResource(hGlobalRes);
		::FreeResource(hGlobalRes);
		return ::GetLastError();
	}

	void* pData = ::GlobalLock(hAlloc);
	memcpy(pData, (void *)hGlobalRes, nSize);
	::GlobalUnlock(hAlloc);

	UnlockResource(hGlobalRes);    
	::FreeResource(hGlobalRes); 

	HRESULT hr = ::CreateStreamOnHGlobal(hAlloc, TRUE, pStream);
	if ( hr != S_OK ) {
		*pStream = NULL;
		::GlobalFree(hAlloc);
		//NSELog(L"CreateStreamOnHGlobal failed, HRESULT=0x%08X", hr);
		return ERROR_OUTOFMEMORY;
	}

	return ERROR_SUCCESS;
}

DWORD GetResourceImage (
	__in UINT uId, 
	__in LPCWSTR lpszResType, 
	__out Gdiplus::Image** pImage
	)
{
	IStream* pStream;
	DWORD dwErr = GetResourceStream(uId, lpszResType, &pStream);
	if ( dwErr != ERROR_SUCCESS ) {
		return dwErr;
	}

	*pImage = Gdiplus::Image::FromStream(pStream, FALSE);
	pStream->Release();

	return ERROR_SUCCESS;
}
}

