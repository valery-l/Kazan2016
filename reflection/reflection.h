
#pragma once 

namespace refl
{
    template<class ref_type>
    struct ref_wrap_impl
    {
        typedef ref_type reference_type;

        explicit ref_wrap_impl(reference_type ref)
            : ref_(ref)
        {}

        reference_type get() const { return ref_; }

    private:
        reference_type ref_;
    };


    template<bool is_const, class T>
    struct ref_wrapper
        : ref_wrap_impl < T& >
    {
        ref_wrapper(reference_type ref)
            : ref_wrap_impl < T& >(ref)
        {}
    };

    template<class T>
    struct ref_wrapper<true, T>
        : ref_wrap_impl < T const& >
    {
        ref_wrapper(reference_type ref)
        : ref_wrap_impl < T const& >(ref)
        {}
    };

    template<class T>
    ref_wrapper<false, T> make_ref(T& t) { return ref_wrapper<false, T>(t); }

    template<class T>
    ref_wrapper<true, T> make_ref(T const& t) { return ref_wrapper<true, T>(t); }
} // refl
 

#define REFL_STRUCT(...)                                      \
template<bool is_const_ref, class processor>                  \
void reflect(refl::ref_wrapper<is_const_ref, __VA_ARGS__> obj,\
             processor& proc)                                 \
{                                                             \
    (void) obj;                                               \
    (void) proc;

// simple way - only for const operations

// #define REFL_STRUCT(...)            \
// template<class processor>           \
// void reflect(__VA_ARGS__ const& obj,\
//              processor& proc)       \
// {                                   \
//     (void) obj;                     \
//     (void) proc;

#define REFL_ENTRY(field)       \
    proc(obj.get().field, #field);

#define REFL_END()              \
}