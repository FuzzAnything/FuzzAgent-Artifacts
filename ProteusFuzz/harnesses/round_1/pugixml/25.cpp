// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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
#include <string_view>
#include <cstring>

static bool try_parse_xml(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        return false;
    }
    // Ensure null termination for safety if treating as string
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    // Replace potential null bytes in the middle to avoid early truncation issues if any
    // Though pugi handles binary data, ensuring valid UTF-8/ASCII is good practice for fuzzing
    // We will just pass it directly to parse_buffer
    pugi::xml_parse_result res = doc.load_buffer(xml_str.data(), xml_str.size());
    return res.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Attempt to parse the input as XML
    // We use load_buffer to handle arbitrary binary data safely
    pugi::xml_parse_result res = doc.load_buffer(Data, Size);
    if (res.status != pugi::status_ok) {
        // If parsing fails, we can still try to create a minimal document to test the API
        // But the target API requires a valid node structure. 
        // Let's create a root node manually if parsing fails to ensure we test the logic.
        doc.append_child("root");
    }

    // 1. pugi::xml_node::child
    // We need a node to call child on. Let's use the root or a specific child if it exists.
    pugi::xml_node node = doc.root();
    if (!node) {
        // Fallback if root is empty (shouldn't happen after append above)
        node = doc.append_child("root");
    }

    // Try to find a child named "child" or use the root itself if we want to traverse
    // The API expects a name. We can try to extract a name from the fuzz data or use a static one.
    // To explore states, let's try to use a substring from the input as the name if possible.
    const char* child_name = "target";
    if (Size >= 4) {
        // Use first 4 bytes as a potential name, ensuring it's printable or null-terminated
        // For simplicity in fuzzing, we often use static names to ensure valid XML structure
        // but here we just need to call the function.
        child_name = "test_node"; 
    }
    
    pugi::xml_node child_node = node.child(child_name);
    
    // If child_node is empty, we might need to create one to proceed with append_attribute
    // However, the task says invoke child -> append_attribute. 
    // append_attribute on an empty node returns an empty attribute, which is safe.
    // But to fully exercise set_value, we need a valid attribute.
    // Let's ensure we have a valid node to work on. If child_node is empty, we use 'node' itself.
    pugi::xml_node target_node = child_node;
    if (!target_node) {
        target_node = node;
        // Create the child if it doesn't exist to make the test more meaningful?
        // No, the sequence is fixed. We just call the functions.
        // If target_node is the root, we can still append attributes.
    }

    // 2. pugi::xml_node::append_attribute
    pugi::xml_attribute attr1 = target_node.append_attribute("attr1");
    
    // 3. pugi::xml_attribute::set_value (int)
    if (attr1) {
        int val_int = 0;
        if (Size >= 4) {
            val_int = *reinterpret_cast<const int*>(Data);
        }
        attr1.set_value(val_int);
    }

    // 3. pugi::xml_attribute::set_value (const char_t*)
    if (attr1) {
        const char* val_str = "string_value";
        if (Size > 0) {
            // Use a portion of the input as the string value if it looks like text
            // Or just use a static string to ensure valid char_t*
            val_str = "fuzzed_string";
        }
        attr1.set_value(val_str);
    }

    // 3. pugi::xml_attribute::set_value (unsigned int)
    if (attr1) {
        unsigned int val_uint = 0;
        if (Size >= 4) {
            val_uint = *reinterpret_cast<const unsigned int*>(Data);
        }
        attr1.set_value(val_uint);
    }

    // 4. pugi::xml_node::append_attribute
    pugi::xml_attribute attr2 = target_node.append_attribute("attr2");
    
    // 5. pugi::xml_node::append_attribute
    pugi::xml_attribute attr3 = target_node.append_attribute("attr3");

    // 6. pugi::xml_attribute::set_value (int) - on attr3
    if (attr3) {
        int val_int2 = 0;
        if (Size >= 4) {
            val_int2 = *reinterpret_cast<const int*>(Data + (Size >= 4 ? 4 : 0));
        }
        attr3.set_value(val_int2);
    }

    // 7. pugi::xml_node::append_attribute
    pugi::xml_attribute attr4 = target_node.append_attribute("attr4");

    // 8. pugi::xml_node::append_attribute
    pugi::xml_attribute attr5 = target_node.append_attribute("attr5");

    // 9. pugi::xml_attribute::set_value (const char_t*) - on attr5
    if (attr5) {
        const char* val_str2 = "final_value";
        if (Size > 0) {
            val_str2 = "fuzzed_final";
        }
        attr5.set_value(val_str2);
    }

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}