
#pragma once

namespace smart_ptr 
{
    namespace details
    {
        struct list_node
        {
            list_node();

            list_node(list_node const& other);
            list_node& operator=(list_node other);

            ~list_node();

            void swap(list_node& other);
            bool unique() const;

        private:
            void remove();
            void insert(list_node const& where);

        private:
            mutable list_node const* left_;
            mutable list_node const* right_;
        };
    } // details
} // smart_ptr 



namespace smart_ptr 
{    
    template<class T>
    struct linked_ptr 
    {
        explicit linked_ptr(T* ptr = nullptr);
        ~linked_ptr();
        
        linked_ptr(linked_ptr const&);

        template<class U>
        linked_ptr(linked_ptr<U> const& other);

        linked_ptr& operator=(linked_ptr other);

        template<class U = T>
        void reset(U* = nullptr);

        void swap(linked_ptr&);

        bool unique() const;
        
        T* get() const;

        T* operator->() const;
        T& operator* () const;

        explicit operator bool() const;

    public:
        details::list_node const& node() const;

    private:
        T*                  ptr_;
        details::list_node  node_;
    };
} // smart_ptr 

#include "linked_ptr_impl.h"