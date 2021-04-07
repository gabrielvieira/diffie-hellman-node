#include "DiffieHellmanWrapper.h"

Napi::FunctionReference DiffieHellmanWrapper::constructor;

Napi::Object DiffieHellmanWrapper::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "DiffieHellmanWrapper", {
    InstanceMethod("generateKeyPair", &DiffieHellmanWrapper::generateKeyPair),
    InstanceMethod("keyAgreement", &DiffieHellmanWrapper::keyAgreement),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("DiffieHellmanWrapper", func);
  return exports;
}

DiffieHellmanWrapper::DiffieHellmanWrapper(const Napi::CallbackInfo& info) : Napi::ObjectWrap<DiffieHellmanWrapper>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  this->diffieHellmanHelper_ = new DiffieHellmanHelper();
}

Napi::Value DiffieHellmanWrapper::generateKeyPair(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  std::string result = this->diffieHellmanHelper_->generateKeyPair();
  return Napi::String::New(env, result);
}


Napi::Value DiffieHellmanWrapper::keyAgreement(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "public key must be a hex string").ThrowAsJavaScriptException();
  }

  Napi::String hexPublicKey = info[0].As<Napi::String>();
  std::string result = this->diffieHellmanHelper_->keyAgreement(hexPublicKey.ToString());

  return Napi::String::New(info.Env(), result);
}