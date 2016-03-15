
#pragma once 

#include "reflection.h"
#include <type_traits>
#include <string>
#include <iosfwd>


namespace ostream_writer
{
    namespace details
    {
        using std::enable_if;
        using std::is_arithmetic;
        using std::is_same;
        using std::string;

        template <class T>
        struct has_begin_end
        {
            struct yes { char nothing; };
            struct no  { yes  nothing[2]; };

            template <typename U>
            static auto test(U* u) -> decltype((*u).begin(), (*u).end(), yes());
            static no   test(...);

            enum { value = (sizeof(yes) == sizeof test((T*)0)) };
            typedef std::integral_constant<bool, value> type;
        };
    
        //////////////////////////////////////////////////////////////////////////

        template<class T>
        using if_primitive =
            typename enable_if
            <is_arithmetic<T>::value || is_same<T, string>::value>::type;

        template<class T>
        using if_container =
            typename enable_if
            <details::has_begin_end<T>::value && !is_same<T, string>::value>::type;

        template<class T>
        using if_neither_container_nor_prim =
            typename enable_if
            <!details::has_begin_end<T>::value && !is_arithmetic<T>::value>::type;

        //////////////////////////////////////////////////////////////////////////

        struct os_processor
        {
            os_processor(std::ostream& os)
                : os_(os)
            {}

            template<class type_t>
            void operator()(type_t const& obj, const char* name)
            {
                write(os_, obj, name);
            }

        private:
            std::ostream& os_;
        };
    } // details 



    //////////////////////////////////////////////////////////////////////////
    template<class type_t>
    details::if_primitive<type_t> write(std::ostream& os, type_t const& obj, const char* name)
    {
        os << name << ": " << obj << endl;
    }

    template<class type_t>
    details::if_container<type_t> write(std::ostream& os, type_t const& obj, const char* name)
    {
        os << name << endl;
        os << "{" << endl;

        for (auto const& item : obj)
            write(os, item, "");

        os << "}" << endl;
    }

    template<class type_t>
    details::if_neither_container_nor_prim<type_t> write(
        std::ostream& os, type_t const& obj, const char* = "")
    {
        details::os_processor proc(os);

        os << "{" << endl;
        reflect(refl::make_ref(obj), proc);

        // for more simple version 
        // reflect(obj, proc);

        os << "}" << endl;
    }

} // ostream_writer