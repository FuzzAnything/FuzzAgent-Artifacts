// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzzer input
    FILE* file = fopen("./dummy_file", "wb");
    if (!file) {
        return 0;
    }
    fwrite(Data, 1, Size, file);
    fclose(file);

    // Parse the XML document from the dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // If parsing failed, we can still work with an empty document
    // or create a simple document structure for testing
    if (!result) {
        // Create a minimal document for fuzzing
        doc.append_child("root").append_child("child1").text().set("default");
        doc.child("root").append_child("child2").text().set("test");
    }

    // Get the root node (or create one if missing)
    pugi::xml_node root = doc.first_child();
    if (!root) {
        root = doc.append_child("root");
    }

    // Prepare string data for function calls
    // Use a safe buffer to ensure null-termination
    char buffer[256] = {0};
    size_t copy_size = Size < 255 ? Size : 255;
    if (copy_size > 0 && Data) {
        memcpy(buffer, Data, copy_size);
        buffer[copy_size] = '\0';
    }
    
    std::string_view str_view(buffer, copy_size);

    // Execute the specified sequence of API calls
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(str_view.data());
    
    // 2. pugi::xml_node::text
    pugi::xml_text text_obj = child_node.text();
    
    // 3. pugi::xml_text::set
    if (Size > 0) {
        text_obj.set(reinterpret_cast<const char*>(Data), Size);
    }
    
    // 4. pugi::xml_node::first_child
    pugi::xml_node first_child = child_node.first_child();
    
    // 5. pugi::xml_text::set (on the same text_obj)
    if (Size > 0) {
        text_obj.set(reinterpret_cast<const char*>(Data), Size);
    }
    
    // 6. pugi::xml_node::first_child (on first_child)
    pugi::xml_node first_child2 = first_child.first_child();
    
    // 7. pugi::xml_node::last_child (on first_child)
    pugi::xml_node last_child = first_child.last_child();
    
    // 8. pugi::xml_text::set (on text_obj again)
    if (Size > 0) {
        text_obj.set(reinterpret_cast<const char*>(Data), Size);
    }
    
    // 9. pugi::xml_node::first_child (on first_child2)
    pugi::xml_node first_child3 = first_child2.first_child();
    
    // 10. pugi::xml_node::last_child (on first_child2)
    pugi::xml_node last_child2 = first_child2.last_child();
    
    // 11. pugi::xml_text::set (on text_obj again)
    if (Size > 0) {
        text_obj.set(reinterpret_cast<const char*>(Data), Size);
    }
    
    // 12. pugi::xml_node::first_child (on first_child3)
    pugi::xml_node first_child4 = first_child3.first_child();

    // Clean up the dummy file
    remove("./dummy_file");
    
    return 0;
}