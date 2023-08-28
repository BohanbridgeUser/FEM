#ifndef _SMART_POINTER_H_
#define _SMART_POINTER_H_

template<typename C, typename...Args>
std::shared_ptr<C> make_shared(Args &&...args) {
    return std::make_shared<C>(std::forward<Args>(args)...);
}
#endif