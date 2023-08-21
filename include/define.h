#include <memory>
#define LOTUS_POINTER_DEFINE(a) typedef a* Pointer;\
                                typedef std::shared_ptr<a > SharedPointer; \
                                typedef std::weak_ptr<a > WeakPointer; \
                                typedef std::unique_ptr<a > UniquePointer;

#define LOTUS_DEFINE_LOCAL_FLAGS(name) static const Flags name;

