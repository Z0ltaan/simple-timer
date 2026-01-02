#ifndef DATA_CALLBACK_H
#define DATA_CALLBACK_H

#include "miniaudio/miniaudio.h"

void
data_callback(ma_device* pDevice,
              void* pOutput,
              const void* pInput,
              ma_uint32 frameCount);
#endif
