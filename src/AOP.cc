#include <iostream>

#include "AOP.h"

using namespace v8;

Persistent<Function> AOP::constructor;

AOP::AOP() {

}

AOP::~AOP() {

}

void AOP::Init(Local<Object> exports) {
    Isolate* isolate = exports->GetIsolate();

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewFromUtf8(isolate, "AOP"));

    NODE_SET_PROTOTYPE_METHOD(tpl, "before", Before);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "AOP"), tpl->GetFunction());
}

void AOP::New(const FunctionCallbackInfo <Value> &args) {
    AOP* aop = new AOP();
    aop->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}



void AOP::Before(const FunctionCallbackInfo<Value> &args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> global = context->Global();




    /*Local<Array> property_names = global->GetPropertyNames();
    for (unsigned int i = 0; i < property_names->Length(); i++) {
        Local<Value> key = property_names->Get(i);
        const String::Utf8Value key_name(key->ToString());
        Local<Value> value = global->Get(key);

        const String::Utf8Value key_type(value->TypeOf(isolate)->ToString());

        std::cout << "got key: " << *key_name <<  ", type: " << *key_type << std::endl;

        if (value->IsFunction()) {

        }
    }*/


    Local<String> name = args[0]->ToString();
    Local<Object> object = args[1]->ToObject();
    Local<Function> advice = Local<Function>::Cast(args[2]);
    Local<Function> join_point = GetFunction(isolate, name, object);


    Interceptor* interceptor = new Interceptor();
    interceptor->advice.Reset(isolate, advice);
    interceptor->join_point.Reset(isolate, join_point);

    Local<External> data = External::New(isolate, (void *) interceptor);
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, ApplyBefore, data);
    Local<Function> function = tpl->GetFunction();
    object->Set(name, function);
}

void AOP::ApplyBefore(const FunctionCallbackInfo<Value> &args) {
    Isolate* isolate = args.GetIsolate();

    Local<External> data = args.Data().As<External>();
    Interceptor* interceptor = static_cast<Interceptor*>(data->Value());

    unsigned int argc = args.Length();
    // TODO - is this a source of a leak?
    Handle<Value> *argv = new Handle<Value>[argc];
    for (unsigned int i = 0; i < argc; i++) {
        argv[i] = args[i];
    }

    Local<Function>::New(isolate, interceptor->advice)->
            Call(isolate->GetCurrentContext()->Global(), argc, argv);

    Local<Function>::New(isolate, interceptor->join_point)->
            Call(isolate->GetCurrentContext()->Global(), argc, argv);
}



Local<Function> AOP::GetFunction(Isolate* isolate, Local<String> name, Local<Object> source) {
    const String::Utf8Value name_string(name->ToString());

    Local<Value> property = source->Get(name);
    if (property->IsFunction()) {
        Local<Function> function = Local<Function>::Cast(property);

        return function;
    }
}