#ifndef _K_RESOURCE_UTIL_H_
#define _K_RESOURCE_UTIL_H_

#include <GdiPlus.h>

namespace TinyDui 
{

DWORD GetResourceStream (
	__in UINT uId, 
	__in LPCWSTR lpszResType, 
	__out IStream** pStream
	);

DWORD GetResourceImage (
	__in UINT uId, 
	__in LPCWSTR lpszResType, 
	__out Gdiplus::Image** pImage
	);
}

#endif	// _K_RESOURCE_UTIL_H_
