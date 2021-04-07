#include <napi.h>
#include "DiffieHellmanHelper.h"

class DiffieHellmanWrapper : public Napi::ObjectWrap<DiffieHellmanWrapper> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports); //Init function for setting the export key to JS
  DiffieHellmanWrapper(const Napi::CallbackInfo& info); //Constructor to initialise

 private:
  static Napi::FunctionReference constructor; //reference to store the class definition that needs to be exported to JS
  Napi::Value generateKeyPair(const Napi::CallbackInfo& info); //wrapped getValue function 
  Napi::Value keyAgreement(const Napi::CallbackInfo& info); //wrapped add function
  DiffieHellmanHelper *diffieHellmanHelper_; //internal instance of actualclass used to perform actual operations.
};