// This fuzz driver is generated for library tinygltf, aiming to fuzz the following functions:
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsNumber at tiny_gltf.h:298:8 in tiny_gltf.h
// tinygltf::Value::GetNumberAsDouble at tiny_gltf.h:311:10 in tiny_gltf.h
// tinygltf::TinyGLTF::LoadASCIIFromFile at tiny_gltf.h:1411:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:345:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsArray at tiny_gltf.h:306:8 in tiny_gltf.h
// tinygltf::Value::Get at tiny_gltf.h:336:16 in tiny_gltf.h
// tinygltf::Value::IsObject at tiny_gltf.h:308:8 in tiny_gltf.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

static void WriteDummyFile(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    tinygltf::Model model;
    std::string err;
    std::string warn;
    tinygltf::TinyGLTF ctx;

    // Write input data to a dummy file
    WriteDummyFile(Data, Size);

    // 1. LoadASCIIFromFile
    bool loadSuccess = ctx.LoadASCIIFromFile(&model, &err, &warn, "./dummy_file");

    // We need a Value to test the subsequent API chain.
    // If loading succeeded, we try to extract a Value from the model's extras or extensions.
    // If loading failed, we construct a dummy Value to ensure the rest of the chain is tested.
    tinygltf::Value targetValue;

    if (loadSuccess) {
        // Try to get a value from model extras
        if (model.extras.IsObject()) {
            targetValue = model.extras;
        } else {
            targetValue = tinygltf::Value(); 
        }
    } else {
        // If load failed, create a dummy Value to test the rest of the chain.
        tinygltf::Value::Object obj;
        obj["dummy"] = tinygltf::Value(1.0); // Add a number
        targetValue = tinygltf::Value(obj);
    }

    // 2. Value::IsObject
    bool isObj = targetValue.IsObject();

    // 3. Value::Get (Object overload)
    std::string key = "dummy";
    if (Size > 0 && Data[0] > 127) key = "asset";
    if (Size > 0 && Data[0] > 200) key = "extensions";
    
    const tinygltf::Value &valObj = targetValue.Get(key);

    // 4. Value::IsArray
    bool isArray = valObj.IsArray();

    // 5. Value::Get (Array overload)
    // Only call Get(size_t) if valObj is actually an array to avoid assertion failure.
    const tinygltf::Value *valArrPtr = nullptr;
    tinygltf::Value valArrDefault;
    
    if (isArray) {
        size_t idx = 0;
        if (Size > 0) {
            idx = Data[0] % 10; 
        }
        // Get returns a reference, so we store a pointer to it or copy it.
        // Since Get returns a const reference to internal data, we can't assign to a local const ref directly in an if/else block without initialization.
        // We will use a pointer to the result of Get if valid, or a local default value.
        valArrPtr = &valObj.Get(idx);
    } else {
        valArrPtr = &valArrDefault;
    }
    const tinygltf::Value &valArr = *valArrPtr;

    // 6. Value::Get (Another call, maybe trying to get from the result of the previous get)
    // Check type before calling to avoid UB or assertions
    const tinygltf::Value *valArr2Ptr = nullptr;
    tinygltf::Value valArr2Default;
    
    if (valArr.IsArray()) {
        valArr2Ptr = &valArr.Get(0);
    } else if (valArr.IsObject()) {
        valArr2Ptr = &valArr.Get("test");
    } else {
        valArr2Ptr = &valArr2Default;
    }
    const tinygltf::Value &valArr2 = *valArr2Ptr;

    // 7. Value::Get (Yet another call, maybe trying to get from valArr2)
    const tinygltf::Value *valArr3Ptr = nullptr;
    tinygltf::Value valArr3Default;
    
    if (valArr2.IsArray()) {
        valArr3Ptr = &valArr2.Get(0);
    } else if (valArr2.IsObject()) {
        valArr3Ptr = &valArr2.Get("test");
    } else {
        valArr3Ptr = &valArr3Default;
    }
    const tinygltf::Value &valArr3 = *valArr3Ptr;

    // 8. Value::GetNumberAsDouble
    // Ensure type is numeric first to avoid undefined behavior
    double numVal1 = 0.0;
    if (valArr3.IsNumber()) {
        numVal1 = valArr3.GetNumberAsDouble();
    }

    // 9. Value::Get (Another call)
    const tinygltf::Value *valArr4Ptr = nullptr;
    tinygltf::Value valArr4Default;
    
    if (valArr3.IsArray()) {
        valArr4Ptr = &valArr3.Get(0);
    } else if (valArr3.IsObject()) {
        valArr4Ptr = &valArr3.Get("test");
    } else {
        valArr4Ptr = &valArr4Default;
    }
    const tinygltf::Value &valArr4 = *valArr4Ptr;

    // 10. Value::GetNumberAsDouble (Another call)
    double numVal2 = 0.0;
    if (valArr4.IsNumber()) {
        numVal2 = valArr4.GetNumberAsDouble();
    }

    // Cleanup
    std::remove("./dummy_file");

    return 0;
}