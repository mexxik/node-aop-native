#include <node.h>


#include "AOP.h"



using namespace v8;


void init(Local<Object> exports) {
    AOP::Init(exports);
}

NODE_MODULE(node_aop, init)
