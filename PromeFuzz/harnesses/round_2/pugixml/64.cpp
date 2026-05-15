// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
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
#include <fstream>
#include <string_view>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();
}

static void cleanup_dummy_file() {
    remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    write_dummy_file(Data, Size);
    
    // Load XML document from dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    if (!result) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Get root node to start traversal
    pugi::xml_node node = doc.root().first_child();
    
    // Prepare string parameters for API calls
    const char* child_name = "test";
    std::string_view sv_child_name = "test";
    const char* sibling_name = "sibling";
    std::string_view sv_sibling_name = "sibling";
    
    // Sequence of API calls as specified
    // Each call updates the node to explore different states
    
    // 1. next_sibling
    node = node.next_sibling(sibling_name);
    node = node.next_sibling(sv_sibling_name);
    node = node.next_sibling();
    
    // 2. next_sibling
    node = node.next_sibling(sibling_name);
    node = node.next_sibling(sv_sibling_name);
    node = node.next_sibling();
    
    // 3. previous_sibling
    node = node.previous_sibling(sibling_name);
    node = node.previous_sibling(sv_sibling_name);
    node = node.previous_sibling();
    
    // 4. previous_sibling
    node = node.previous_sibling(sibling_name);
    node = node.previous_sibling(sv_sibling_name);
    node = node.previous_sibling();
    
    // 5-10. child (6 calls)
    for (int i = 0; i < 6; i++) {
        node = node.child(child_name);
    }
    
    // 11. next_sibling
    node = node.next_sibling(sibling_name);
    node = node.next_sibling(sv_sibling_name);
    node = node.next_sibling();
    
    // 12. next_sibling
    node = node.next_sibling(sibling_name);
    node = node.next_sibling(sv_sibling_name);
    node = node.next_sibling();
    
    // 13. previous_sibling
    node = node.previous_sibling(sibling_name);
    node = node.previous_sibling(sv_sibling_name);
    node = node.previous_sibling();
    
    // 14. previous_sibling
    node = node.previous_sibling(sibling_name);
    node = node.previous_sibling(sv_sibling_name);
    node = node.previous_sibling();
    
    // 15-18. next_sibling (4 calls)
    for (int i = 0; i < 4; i++) {
        node = node.next_sibling(sibling_name);
        node = node.next_sibling(sv_sibling_name);
        node = node.next_sibling();
    }
    
    // 19-22. previous_sibling (4 calls)
    for (int i = 0; i < 4; i++) {
        node = node.previous_sibling(sibling_name);
        node = node.previous_sibling(sv_sibling_name);
        node = node.previous_sibling();
    }
    
    // Clean up
    cleanup_dummy_file();
    
    return 0;
}