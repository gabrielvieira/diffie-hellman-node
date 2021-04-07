#include <napi.h>
#include "DiffieHellmanWrapper.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return DiffieHellmanWrapper::Init(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)