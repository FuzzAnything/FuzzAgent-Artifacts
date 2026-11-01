// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::get_memory_deallocation_function at pugixml.cpp:7878:54 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstring>
#include <cstddef>

static void* custom_allocate(size_t size) {
    if (size == 0) return nullptr;
    return malloc(size);
}

static void custom_deallocate(void* ptr) {
    free(ptr);
}

static pugi::allocation_function original_allocate_func;
static pugi::deallocation_function original_deallocate_func;

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment: Get current memory functions
    original_allocate_func = pugi::get_memory_allocation_function();
    original_deallocate_func = pugi::get_memory_deallocation_function();

    // 2. Set custom memory management functions
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    // Create a document to work with
    pugi::xml_document doc;
    if (!doc.load_string("<root></root>")) {
        // If loading fails (unlikely for static string), try to create manually or return
        // For fuzzing robustness, we ensure we have a valid document node
        doc.reset();
        pugi::xml_node root = doc.append_child(pugi::node_element);
        if (!root) return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child(pugi::node_element);
        if (!root) return 0;
        root.set_name("root");
    }

    // 3. Invoke append_child
    // We need to generate a valid name from the fuzz input if possible, or use a default.
    // The hint says to ensure null-termination.
    const char* name_str = nullptr;
    size_t max_name_len = Size > 50 ? 50 : Size;
    
    // Try to find a null terminator within the buffer to use as a name
    for (size_t i = 0; i < max_name_len; ++i) {
        if (Data[i] == '\0') {
            name_str = reinterpret_cast<const char*>(Data);
            break;
        }
    }

    // If no null terminator found, use a safe default or a fixed string
    if (name_str == nullptr) {
        name_str = "fuzz_node";
    } else {
        // Ensure we don't pass a pointer that goes out of bounds if the string is too long
        // Actually, append_child takes a string_view_t or const char_t*. 
        // The signature provided in the prompt is: xml_node append_child(xml_node_type type = node_element);
        // However, the description mentions "For elements, use the name overload."
        // The standard pugixml API has: xml_node append_child(const char_t* name);
        // Since the prompt asks to invoke the specific signature `append_child(xml_node_type type)`,
        // but also mentions "For elements, use the name overload" and "input string passed to append_child",
        // there is a slight conflict in the prompt's provided signature vs description.
        // The provided signature in the prompt is: xml_node append_child(xml_node_type type = node_element);
        // Let's stick strictly to the provided signature in the prompt for the call, 
        // but the description implies we should be testing with names.
        // Re-reading: "append_child adds a node... For elements, use the name overload."
        // But the signature block says: `xml_node append_child(xml_node_type type = node_element);`
        // I will call the version with the type as requested by the signature block, 
        // but to make it interesting, I might need to set the name afterwards if the type is element.
        // Wait, the prompt says "invoke the functions in the following order: ... append_child ...".
        // And the signature provided is the one taking xml_node_type.
        // However, the hint says "For `string_view_t` parameters, use `std::string_view a_str;`".
        // This implies the fuzzer might be expected to use the name overload.
        // Let's look at the signature again.
        // `xml_node append_child(xml_node_type type = node_element);`
        // This creates a node of the given type. If type is node_element, it has no name initially.
        // The hint about string_view_t might be a generic hint for pugixml fuzzing or implies I should use the name overload if available.
        // Given the explicit signature block, I will use the type overload. 
        // But to utilize the input data meaningfully, I can try to set the name if it's an element.
        // Or, perhaps the prompt implies the signature is `append_child(const char_t* name)`?
        // No, the prompt explicitly lists the signature. I must follow the signature.
        // However, the description says "For elements, use the name overload."
        // This is contradictory. Let's assume the prompt wants me to test the `type` overload, 
        // but maybe I should also try to set a name if possible to exercise more code paths?
        // Actually, let's re-read the hint: "Ensure the input string passed to append_child is null-terminated..."
        // This strongly suggests the name overload is intended. 
        // But the signature block says `append_child(xml_node_type type)`.
        // In pugixml, `append_child` has multiple overloads.
        // 1. append_child(xml_node_type type)
        // 2. append_child(const char_t* name)
        // 3. append_child(const char_t* name, const char_t* value)
        // The prompt lists signature 1. But the hint talks about strings.
        // To be safe and cover more states as per step 3 ("explore more program states"), 
        // I will try to use the name overload if the input looks like a valid name, 
        // otherwise fall back to the type overload.
        // But the instruction says "invoke the functions in the following order... append_child".
        // It doesn't strictly say "only the signature provided". It says "using the information provided".
        // The information includes a signature block.
        // Let's try to use the name overload if we have a valid string, as the hint suggests.
        // If I strictly follow the signature block, I can't pass a string.
        // Let's assume the prompt wants me to test the name overload because of the hint.
        // But the signature block is explicit.
        // Okay, I will call the type overload as per the signature block, 
        // but I will also try to set the name if it's an element to satisfy the "input string" hint contextually.
        // Actually, the most robust interpretation: The prompt provides the signature for the `type` overload.
        // I will call that. The hint about string_view_t might be a red herring or for a different function?
        // No, "input string passed to append_child".
        // Maybe the prompt meant the signature for the name overload but pasted the type one?
        // Let's look at the "Related Data Types". It lists `xml_node_type`.
        // Okay, I will call `append_child` with a type derived from the input if possible, 
        // or just `node_element`.
        // To satisfy the "input string" hint, I will try to use the name overload if the input has a null terminator.
        // This covers both bases.
        
        // Let's try the name overload first if we have a valid string.
        // If not, use the type overload.
        // But the prompt says "invoke ... append_child". It doesn't specify which overload.
        // I'll prioritize the name overload if valid, as it exercises more logic (parsing name).
        // If the name is invalid (no null terminator), I'll use the type overload.
    }

    pugi::xml_node new_node;
    if (name_str != nullptr && std::strlen(name_str) > 0 && std::strlen(name_str) < 1000) {
        // Use name overload if we have a safe string
        new_node = root.append_child(name_str);
    } else {
        // Fallback to type overload as per the explicit signature provided
        // Use input data to vary the node type if possible
        pugi::xml_node_type type = pugi::node_element;
        if (Size > 0) {
            int val = Data[0] % 8; // 0 to 7
            if (val == 0) type = pugi::node_element;
            else if (val == 1) type = pugi::node_pcdata;
            else if (val == 2) type = pugi::node_cdata;
            else if (val == 3) type = pugi::node_comment;
            else if (val == 4) type = pugi::node_pi;
            else if (val == 5) type = pugi::node_declaration;
            else if (val == 6) type = pugi::node_doctype;
            else type = pugi::node_element;
        }
        new_node = root.append_child(type);
        
        // If we created an element and have a valid string, set the name to exercise that path too
        if (type == pugi::node_element && name_str != nullptr && std::strlen(name_str) > 0 && std::strlen(name_str) < 1000) {
            new_node.set_name(name_str);
        }
    }

    // 4. Invoke first_child
    pugi::xml_node first = root.first_child();
    (void)first; // Suppress unused variable warning

    // Cleanup is handled by the document destructor when it goes out of scope.
    // Restore original memory functions to avoid affecting other test cases if run in a loop
    pugi::set_memory_management_functions(original_allocate_func, original_deallocate_func);

    return 0;
}