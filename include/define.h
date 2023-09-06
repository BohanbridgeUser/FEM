#include <memory>
#include <mutex>
#include "Utility/string_utilities.h"
#include "Utility/smart_pointer.h"
#define LOTUS_POINTER_DEFINE(a) typedef a* Pointer;\
                                typedef std::shared_ptr<a > SharedPointer; \
                                typedef std::weak_ptr<a > WeakPointer; \
                                typedef std::unique_ptr<a > UniquePointer;

#define LOTUS_DEFINE_LOCAL_FLAGS(name) \
                                        static const Flags name;
#define LOTUS_CREATE_LOCAL_FLAGS(class_name,name,position) \
                                        const Flags class_name::name(Flags::Create(position));

#define LOTUS_DEFINE_VARIABLE(type,name)  extern Variable<type> name;
#define LOTUS_DEFINE_3D_VARIABLES_WITH_COMPONENTS(type,name) extern Variable<std::array<type,3> > name;\
                                                             extern Variable<type> name ## _X;\
                                                             extern Variable<type> name ## _Y;\
                                                             extern Variable<type> name ## _Z;

#define LOTUS_CREATE_VARIABLE(type,name) Variable<type> name(#name);
#define LOTUS_CREATE_3D_VARIABLES_WITH_THIS_COMPONENTS(name, component1, component2, component3)\
        Variable<std::array<double,3> > name(#name, std::array<double,3>({0.00,0.00,0.00}));\
        Variable<double > component1(#component1, &name, 0);\
        Variable<double > component2(#component2, &name, 1);\
        Variable<double > component3(#component3, &name, 2);
        
#define LOTUS_CREATE_3D_VARIABLES_WITH_COMPONENTS(name) \
        LOTUS_CREATE_3D_VARIABLES_WITH_THIS_COMPONENTS(name, name ## _X, name ## _Y, name ## _Z)

#define LOTUS_CREATE_FLAG(name, position) const Flags name(Flags::Create(position));

#define LOTUS_ERROR(content) std::cerr<< content <<std::endl; exit(0);


