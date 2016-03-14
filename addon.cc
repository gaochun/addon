#include <node.h>
#include <Windows.h>

#include "SampleApplication.h"
#include "shader_utils.h"

class HelloTriangleSample : public SampleApplication
{
public:
  HelloTriangleSample()
    : SampleApplication("HelloTriangle", 1280, 720)
  {
  }

  virtual bool initialize()
  {
    const std::string vs = SHADER_SOURCE
      (
        attribute vec4 vPosition;
    void main()
    {
      gl_Position = vPosition;
    }
    );

    const std::string fs = SHADER_SOURCE
      (
        precision mediump float;
    void main()
    {
      gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    );

    mProgram = CompileProgram(vs, fs);
    if (!mProgram)
    {
      return false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return true;
  }

  virtual void destroy()
  {
    glDeleteProgram(mProgram);
  }

  virtual void draw()
  {
    GLfloat vertices[] =
    {
      0.0f,  0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
    };

    // Set the viewport
    glViewport(0, 0, getWindow()->getWidth(), getWindow()->getHeight());

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object
    glUseProgram(mProgram);

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

private:
  GLuint mProgram;
};


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

void Draw(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HelloTriangleSample app;
  app.run();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

void CreateShader(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 1 || !args[0]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t type = args[0]->Uint32Value();
  if (type != GL_FRAGMENT_SHADER || type != GL_FRAGMENT_SHADER) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  GLuint result = glCreateShader(type);

  args.GetReturnValue().Set(v8::Integer::New(isolate, result));
}

void ShaderSource(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 2 || !args[0]->IsUint32() || !args[1]->IsString()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t shader = args[0]->Uint32Value();
  v8::String::Utf8Value source(args[1]->ToString());
  const char *shaderSrc = *source;

  glShaderSource(shader, 1, &shaderSrc, NULL);
}

void CompileShader(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 1 || !args[0]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t shader = args[0]->Uint32Value();
  glCompileShader(shader);
}

void GetShaderParameter(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 2 || !args[0]->IsUint32() || !args[1]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t shader = args[0]->Uint32Value();
  uint32_t type = args[1]->Uint32Value();

  GLint value;
  switch (type) {
//  case GL_DELETE_STATUS:
//    return WebGLAny(scriptState, shader->isDeleted());
  case GL_COMPILE_STATUS:
    glGetShaderiv(shader, type, &value);
    args.GetReturnValue().Set(v8::Boolean::New(isolate, value != 0));
    return;
//  case GL_SHADER_TYPE:
//    webContext()->getShaderiv(objectOrZero(shader), pname, &value);
//    return WebGLAny(scriptState, static_cast<unsigned>(value));
//  default:
//    synthesizeGLError(GL_INVALID_ENUM, "getShaderParameter", "invalid parameter name");
//    return ScriptValue::createNull(scriptState);
  }

}

void CreateProgram(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  GLuint program = glCreateProgram();
  args.GetReturnValue().Set(v8::Integer::New(isolate, program));
}

void AttachShader(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 2 || !args[0]->IsUint32() || !args[1]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t program = args[0]->Uint32Value();
  uint32_t shader = args[1]->Uint32Value();
  glAttachShader(program, shader);
}

void LinkProgram(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 1 || !args[0]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t program = args[0]->Uint32Value();
  glLinkProgram(program);
}

void GetProgramParameter(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 2 || !args[0]->IsUint32() || !args[1]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t program = args[0]->Uint32Value();
  uint32_t pname = args[1]->Uint32Value();

  switch (pname) {
  case GL_LINK_STATUS:
    args.GetReturnValue().Set(v8::Boolean::New(isolate, true));
    return;
  }
}

void UseProgram(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 1 || !args[0]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t program = args[0]->Uint32Value();
  glUseProgram(program);
}

void GetAttribLocation(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 2 || !args[0]->IsUint32() || !args[1]->IsString()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t program = args[0]->Uint32Value();
  v8::String::Utf8Value nameUtf8(args[1]->ToString());
  const char *name = *nameUtf8;

  GLint result = glGetAttribLocation(program, name);
  args.GetReturnValue().Set(v8::Integer::New(isolate, result));
}

void EnableVertexAttribArray(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 1 || !args[0]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t index = args[0]->Uint32Value();

  glEnableVertexAttribArray(index);
}

void GetUniformLocation(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 2 || !args[0]->IsUint32() || !args[1]->IsString()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t program = args[0]->Uint32Value();
  v8::String::Utf8Value nameUtf8(args[1]->ToString());
  const char *name = *nameUtf8;

  GLint result = glGetUniformLocation(program, name);
  args.GetReturnValue().Set(v8::Integer::New(isolate, result));
}

void CreateBuffer(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  GLuint buffer;
  glGenBuffers(1, &buffer);
  args.GetReturnValue().Set(v8::Integer::New(isolate, buffer));
}

void BindBuffer(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 2 || !args[0]->IsUint32() || !args[1]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t pname = args[0]->Uint32Value();
  uint32_t buffer = args[1]->Uint32Value();

  switch (pname) {
  case GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    return;
  }
}

void BufferData(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 3 || !args[0]->IsUint32() || !args[1]->IsFloat32Array() || !args[2]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t target = args[0]->Uint32Value();
  uint32_t usage = args[2]->Uint32Value();

  Local<v8::Float32Array> array = args[1].As<v8::Float32Array>();
  Local<v8::ArrayBuffer> buffer = array->Buffer();
  v8::ArrayBuffer::Contents contents = buffer->Externalize();
  float* data = static_cast<float*>(contents.Data());

  if (target == GL_STATIC_DRAW && usage == GL_ARRAY_BUFFER) {
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array->Length(), data, GL_STATIC_DRAW);
  }

  delete[] reinterpret_cast<char*>(data);
}

void ClearColor(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 4 || !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  float a = (float)args[0]->NumberValue();
  float b = (float)args[1]->NumberValue();
  float c = (float)args[2]->NumberValue();
  float d = (float)args[3]->NumberValue();

  glClearColor(a, b, c, d);
}

void Enable(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 1 || !args[0]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  GLuint cap = args[0]->Uint32Value();
  if (cap == GL_DEPTH_TEST) {
    glEnable(cap);
  }
}

void Viewport(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 4 || !args[0]->IsUint32() || !args[1]->IsUint32() || !args[2]->IsUint32() || !args[3]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t x = args[0]->Uint32Value();
  uint32_t y = args[1]->Uint32Value();
  uint32_t w = args[2]->Uint32Value();
  uint32_t h = args[3]->Uint32Value();

  glViewport(x, y, w, h);
}

void Clear(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 1 || !args[0]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t type = args[0]->Uint32Value();

  glClear(type);
}

void VertexAttribPointer(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 6 || !args[0]->IsUint32() || !args[1]->IsUint32() || !args[2]->IsUint32() || !args[3]->IsBoolean() || !args[4]->IsUint32() || !args[5]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t a = args[0]->Uint32Value();
  uint32_t b = args[1]->Uint32Value();
  uint32_t c = args[2]->Uint32Value();
  uint32_t d = args[3]->BooleanValue();
  uint32_t e = args[3]->Uint32Value();
  uint32_t f = args[3]->Uint32Value();

  glVertexAttribPointer(a, b, c, d, e, (void*)f);
}

void DrawArrays(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 3 || !args[0]->IsUint32() || !args[1]->IsUint32() || !args[2]->IsUint32()) {
    isolate->ThrowException(v8::Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  uint32_t a = args[0]->Uint32Value();
  uint32_t b = args[1]->Uint32Value();
  uint32_t c = args[2]->Uint32Value();

  glDrawArrays(a, b, c);
}

void GetContext(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Local<Object> obj = Object::New(isolate);
  obj->Set(String::NewFromUtf8(isolate, "FRAGMENT_SHADER"), v8::Integer::New(isolate, 35632));
  NODE_SET_METHOD(obj, "createShader", CreateShader);
  NODE_SET_METHOD(obj, "shaderSource", ShaderSource);
  NODE_SET_METHOD(obj, "compileShader", CompileShader);
  NODE_SET_METHOD(obj, "getShaderParameter", GetShaderParameter);
  NODE_SET_METHOD(obj, "createProgram", CreateProgram);
  NODE_SET_METHOD(obj, "attachShader", AttachShader);
  NODE_SET_METHOD(obj, "linkProgram", LinkProgram);
  NODE_SET_METHOD(obj, "getProgramParameter", GetProgramParameter);
  NODE_SET_METHOD(obj, "useProgram", UseProgram);
  NODE_SET_METHOD(obj, "getAttribLocation", GetAttribLocation);
  NODE_SET_METHOD(obj, "enableVertexAttribArray", EnableVertexAttribArray);
  NODE_SET_METHOD(obj, "getUniformLocation", GetUniformLocation);
  NODE_SET_METHOD(obj, "createBuffer", CreateBuffer);
  NODE_SET_METHOD(obj, "bindBuffer", BindBuffer);
  NODE_SET_METHOD(obj, "bufferData", BufferData);
  NODE_SET_METHOD(obj, "clearColor", ClearColor);
  NODE_SET_METHOD(obj, "enable", Enable);
  NODE_SET_METHOD(obj, "viewport", Viewport);
  NODE_SET_METHOD(obj, "clear", Clear);
  NODE_SET_METHOD(obj, "vertexAttribPointer", VertexAttribPointer);
  NODE_SET_METHOD(obj, "drawArrays", DrawArrays);
  
  args.GetReturnValue().Set(obj);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "hello", Method);
  NODE_SET_METHOD(exports, "draw", Draw);

  NODE_SET_METHOD(exports, "getContext", GetContext);
}

NODE_MODULE(addon, init)

/*xtern "C" {
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
		node::node_module_register(&_module);
	}

	__declspec(dllexport, allocate(".CRT$XCU")) void(__cdecl*_register_mq_)(void) = _register_mq;

}*/

}  // namespace demo