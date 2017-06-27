
#ifndef NODE_AOP_AOP_H
#define NODE_AOP_AOP_H

#include <node.h>
#include <node_object_wrap.h>

struct Interceptor {
    v8::Persistent<v8::Function> advice;
    v8::Persistent<v8::Function> join_point;
};

class AOP : public node::ObjectWrap {
public:
    static v8::Persistent<v8::Function> constructor;
    static void Init(v8::Local<v8::Object> exports);
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void Before(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void ApplyBefore(const v8::FunctionCallbackInfo<v8::Value>& args);
private:
    AOP();
    ~AOP();

    static v8::Local<v8::Function> GetFunction(v8::Isolate* isolate, v8::Local<v8::String> name, v8::Local<v8::Object> source);
};

#endif //NODE_AOP_AOP_H
