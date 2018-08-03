#ifndef _K_UI_HANDLER_CALLBACK_H_
#define _K_UI_HANDLER_CALLBACK_H_

#include "utils/kuidefine.h"

namespace TinyDui 
{
class KUIHandlerCallBack
{
public:

	KUIHandlerCallBack();
	~KUIHandlerCallBack();

public:

	virtual void UINotify(NotifyUIMsg &notifyUIMsg) = 0;
};
}

#endif	// _K_UI_HANDLER_CALLBACK_H_
