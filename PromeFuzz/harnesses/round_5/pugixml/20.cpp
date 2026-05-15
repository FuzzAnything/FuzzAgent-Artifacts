// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
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
    const char* xml_content = 
        "<root attr1=\"value1\" attr2=\"value2\" attr3=\"value3\">"
        "<child1 attr4=\"value4\">Text1</child1>"
        "<child2 attr5=\"value5\">Text2</child2>"
        "<child3 attr6=\"value6\">Text3</child3>"
        "</root>";
    doc.load_string(xml_content);
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    static pugi::xml_document doc = create_test_document();
    pugi::xml_node root = doc.child("root");
    
    // Ensure we have valid data for string operations
    if (Size == 0) return 0;
    
    // Create null-terminated strings from fuzzer input
    size_t usable_size = Size > 256 ? 256 : Size;
    char buffer[257];
    memcpy(buffer, Data, usable_size);
    buffer[usable_size] = '\0';
    
    std::string_view sv(buffer, usable_size);
    const char* cstr = buffer;
    
    // Create xml_attribute objects for overload testing
    pugi::xml_attribute attr1 = root.attribute("attr1");
    pugi::xml_attribute attr2 = root.attribute("attr2");
    pugi::xml_attribute attr3 = root.attribute("attr3");
    
    // Sequence of API calls as specified
    // 1. remove_attribute (string_view overload)
    root.remove_attribute(sv);
    
    // 2. remove_attribute (const char* overload)
    root.remove_attribute(cstr);
    
    // 3. child (const char* overload)
    root.child(cstr);
    
    // 4. child (const char* overload)
    root.child(cstr);
    
    // 5. remove_attribute (xml_attribute overload)
    root.remove_attribute(attr1);
    
    // 6. remove_attribute (string_view overload)
    root.remove_attribute(sv);
    
    // 7. attribute (const char* overload)
    root.attribute(cstr);
    
    // 8. remove_attribute (xml_attribute overload)
    root.remove_attribute(attr2);
    
    // 9. attribute (const char* overload)
    root.attribute(cstr);
    
    // 10. remove_attribute (string_view overload)
    root.remove_attribute(sv);
    
    // 11. remove_attribute (const char* overload)
    root.remove_attribute(cstr);
    
    // 12. remove_attribute (xml_attribute overload)
    root.remove_attribute(attr3);
    
    // 13. remove_attribute (string_view overload)
    root.remove_attribute(sv);
    
    return 0;
}