#include "audio/data_callback.h"

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                   ma_uint32 frameCount) {
  ma_decoder *pDecoder = (ma_decoder *)pDevice->pUserData;
  if (pDecoder == NULL) {
    return;
  }

  ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

  (void)pInput;
}
