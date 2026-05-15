// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string_view>

static void* custom_allocate(size_t size) {
    return malloc(size);
}

static void custom_deallocate(void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE* file = fopen("./dummy_file", "wb");
    if (!file) return 0;
    fwrite(Data, 1, Size, file);
    fclose(file);

    // Parse XML from dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    if (!result) return 0;

    // Get root node
    pugi::xml_node root = doc.root().first_child();
    if (!root) return 0;

    // 1. first_child
    pugi::xml_node child1 = root.first_child();
    
    // 2. next_sibling
    pugi::xml_node sibling1 = child1.next_sibling();
    
    // 3. value
    if (sibling1) {
        const char* val = sibling1.value();
        (void)val; // Use value to avoid unused variable warning
    }
    
    // 4. set_value
    if (sibling1) {
        sibling1.set_value("test_value");
    }
    
    // 5. first_child (again)
    pugi::xml_node child2 = sibling1.first_child();
    
    // 6. next_sibling
    pugi::xml_node sibling2 = child2.next_sibling();
    
    // 7. next_sibling (again)
    pugi::xml_node sibling3 = sibling2.next_sibling();
    
    // 8. parent
    if (sibling3) {
        pugi::xml_node parent = sibling3.parent();
        (void)parent; // Use parent to avoid unused variable warning
    }
    
    // 9. remove_child
    if (root && child1) {
        root.remove_child(child1);
    }
    
    // 10. set_memory_management_functions
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    return 0;
}