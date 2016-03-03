#include <node.h>
#include <Windows.h>
namespace demo {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  ::MessageBox(0, "Message from addon", "message", MB_OK);
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "hello", Method);
}

//NODE_MODULE(addon, init)

extern "C" {
	static node::node_module _module =
	{
		46,
		0,
		NULL,
		__FILE__,
		(node::addon_register_func) (init),
		NULL,
		NODE_STRINGIFY(addon),
		NULL,
		NULL
	};

	static void __cdecl _register_mq(void)
	{
		node_module_register(&_module);
	}

	__declspec(dllexport, allocate(".CRT$XCU")) void(__cdecl*_register_mq_)(void) = _register_mq;

}

}  // namespace demo