// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Create a document and nodes to work with
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create some child nodes to use for copying
    pugi::xml_node child1 = root.append_child("child1");
    child1.append_attribute("attr1") = "value1";
    child1.text() = "text1";
    
    pugi::xml_node child2 = root.append_child("child2");
    child2.append_attribute("attr2") = "value2";
    child2.text() = "text2";
    
    pugi::xml_node child3 = root.append_child("child3");
    child3.append_attribute("attr3") = "value3";
    child3.text() = "text3";
    
    // Create a string from the fuzzer input for node names/values
    // Ensure null termination for string parameters
    std::string name_str;
    if (Size > 0) {
        name_str.assign(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(100)));
        if (name_str.back() != '\0') {
            name_str.push_back('\0');
        }
    } else {
        name_str = "default\0";
    }
    
    // Get string view for functions that need it
    std::string_view name_sv(name_str.c_str(), name_str.length() - 1); // Exclude null terminator
    
    // Start the sequence of API calls
    pugi::xml_node current = root;
    
    // 1. prepend_copy
    current = current.prepend_copy(child1);
    
    // 2. child
    current = current.child(name_str.c_str());
    
    // 3. prepend_copy
    current = current.prepend_copy(child2);
    
    // 4. prepend_copy
    current = current.prepend_copy(child3);
    
    // 5. child
    current = current.child(name_str.c_str());
    
    // 6. prepend_copy
    current = current.prepend_copy(child1);
    
    // 7. child
    current = current.child(name_str.c_str());
    
    // 8. first_child
    current = current.first_child();
    
    // 9. value
    const char* val1 = current.value();
    (void)val1; // Use the value to avoid unused variable warning
    
    // 10. child
    current = current.child(name_str.c_str());
    
    // 11. prepend_copy
    current = current.prepend_copy(child2);
    
    // 12. child
    current = current.child(name_str.c_str());
    
    // 13. child
    current = current.child(name_str.c_str());
    
    // 14. name
    const char* node_name = current.name();
    (void)node_name; // Use the name to avoid unused variable warning
    
    // 15. child
    current = current.child(name_str.c_str());
    
    // 16. child
    current = current.child(name_str.c_str());
    
    // 17. prepend_copy
    current = current.prepend_copy(child3);
    
    // 18. child
    current = current.child(name_str.c_str());
    
    // 19. first_child
    current = current.first_child();
    
    // 20. next_sibling
    current = current.next_sibling();
    
    // 21. value
    const char* val2 = current.value();
    (void)val2; // Use the value to avoid unused variable warning
    
    return 0;
}