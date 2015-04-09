#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <node.h>
#include <node_object_wrap.h>

class Pointer : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::Object> exports) {
        v8::Isolate *isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, Pointer::New);
        tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Pointer"));
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        v8::Local<v8::Function> ctor = tpl->GetFunction();
        Pointer::constructor.Reset(isolate, ctor);
        exports->Set(v8::String::NewFromUtf8(isolate, "Pointer"), ctor);
    }

    static void NewInstance(void *ptr, const v8::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate *isolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(isolate);
        v8::Local<v8::Value> argv[] = { v8::External::New(isolate, ptr) };
        v8::Local<v8::Function> ctor = v8::Local<v8::Function>::New(isolate, Pointer::constructor);
        args.GetReturnValue().Set(ctor->NewInstance(1, argv));
    }

    void *ptr;

private:
    static void New(const v8::FunctionCallbackInfo<v8::Value>& args) {
        v8::Isolate *isolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(isolate);

        if (args.IsConstructCall()) {
            if (!args[0]->IsExternal()) {
                isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Unsupported operation")));
                return;
            }

            void *ptr = v8::External::Cast(*args[0])->Value();
            Pointer *pointer = new Pointer(ptr);
            pointer->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        } else {
            v8::Local<v8::Value> argv[] = { args[0] };
            v8::Local<v8::Function> ctor = v8::Local<v8::Function>::New(isolate, Pointer::constructor);
            args.GetReturnValue().Set(ctor->NewInstance(1, argv));
        }
    }

    static v8::Persistent<v8::Function> constructor;

    explicit Pointer(void *ptr = NULL) {
        this->ptr = ptr;
    }

    ~Pointer() {
    }
};

v8::Persistent<v8::Function> Pointer::constructor;

static void __alloc(const v8::FunctionCallbackInfo<v8::Value>& args) {
    int i = 2015;
    void *ptr = calloc(1, sizeof(int));
    memcpy(ptr, &i, sizeof(int));
    Pointer::NewInstance(ptr, args);
}


static void __release(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);

    if (args.Length() < 0) {
        isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Too few arguments")));
        return;
    }

    v8::Local<v8::Object> holder = v8::Local<v8::Object>::Cast(args[0]);
    Pointer *pointer = node::ObjectWrap::Unwrap<Pointer>(holder);

    if (NULL == pointer) {
        isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Invalid argument")));
        return;
    }

    if (NULL == pointer->ptr) {
        return;
    }

    free(pointer->ptr);
    pointer->ptr = NULL;
}

void initialize(v8::Handle<v8::Object> exports) {
    Pointer::Init(exports);

    NODE_SET_METHOD(exports, "alloc",   __alloc);
    NODE_SET_METHOD(exports, "release", __release);
}

NODE_MODULE(addon, initialize);
