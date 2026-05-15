// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
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

static pugi::xml_document create_test_document() {
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Add some attributes to the root node
    root.append_attribute("attr1");
    root.append_attribute("attr2");
    root.append_attribute("attr3");
    root.append_attribute("attr4");
    root.append_attribute("attr5");
    
    // Add some child nodes
    root.append_child("child1");
    root.append_child("child2");
    root.append_child("child3");
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Create a test document
    pugi::xml_document doc = create_test_document();
    pugi::xml_node node = doc.child("root");
    
    // Use the fuzzer input to create string views and names
    // Ensure we have at least some data for string operations
    if (Size == 0) return 0;
    
    // Create null-terminated strings from fuzzer input
    char attr_name1[256] = {0};
    char attr_name2[256] = {0};
    char attr_name3[256] = {0};
    char child_name1[256] = {0};
    char child_name2[256] = {0};
    
    size_t copy_size1 = Size > 255 ? 255 : Size;
    memcpy(attr_name1, Data, copy_size1);
    attr_name1[copy_size1] = '\0';
    
    // Use different offsets for different names to get variation
    size_t offset = Size > 100 ? 100 : Size/2;
    size_t copy_size2 = (Size - offset) > 255 ? 255 : (Size - offset);
    if (copy_size2 > 0 && offset < Size) {
        memcpy(attr_name2, Data + offset, copy_size2);
        attr_name2[copy_size2] = '\0';
    } else {
        strcpy(attr_name2, "attr2");
    }
    
    // Create string views
    std::string_view sv1(attr_name1, strlen(attr_name1));
    std::string_view sv2(attr_name2, strlen(attr_name2));
    
    // Get some attributes for the remove_attribute(const xml_attribute&) overload
    pugi::xml_attribute attr1 = node.attribute("attr1");
    pugi::xml_attribute attr2 = node.attribute("attr2");
    pugi::xml_attribute attr3 = node.attribute("attr3");
    pugi::xml_attribute attr4 = node.attribute("attr4");
    pugi::xml_attribute attr5 = node.attribute("attr5");
    
    // Execute the sequence of function calls
    // 1. remove_attribute (by xml_attribute)
    node.remove_attribute(attr1);
    
    // 2. remove_attribute (by string_view)
    node.remove_attribute(sv1);
    
    // 3. child (by const char*)
    pugi::xml_node child1 = node.child("child1");
    
    // 4. child (by const char*)
    pugi::xml_node child2 = node.child("child2");
    
    // 5. remove_attribute (by const char*)
    node.remove_attribute("attr3");
    
    // 6. remove_attribute (by xml_attribute)
    node.remove_attribute(attr4);
    
    // 7. attribute (by const char*)
    pugi::xml_attribute retrieved_attr = node.attribute("attr5");
    
    // 8. remove_attribute (by string_view)
    node.remove_attribute(sv2);
    
    // 9. attribute (by const char*)
    pugi::xml_attribute retrieved_attr2 = node.attribute("nonexistent");
    
    // 10. remove_attribute (by xml_attribute)
    node.remove_attribute(attr5);
    
    // 11. remove_attribute (by const char*)
    node.remove_attribute("nonexistent1");
    
    // 12. remove_attribute (by string_view)
    std::string_view sv3("nonexistent2");
    node.remove_attribute(sv3);
    
    // 13. remove_attribute (by xml_attribute)
    // Try to remove an attribute that was already removed
    node.remove_attribute(attr2);
    
    return 0;
}