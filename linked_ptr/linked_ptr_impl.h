
#pragma once 

#include <utility>

namespace smart_ptr
{

namespace details
{
    /// list_node impl
                      
    inline list_node::list_node()
        : left_ (this)
        , right_(this)
    {}
    
    inline list_node::list_node(list_node const& where)
    {
        insert(where);
    }

    list_node& list_node::operator=(list_node other)
    {
        swap(other);
        return *this;
    }

    inline list_node::~list_node()
    {
        remove();
    }

    inline void list_node::swap(list_node& other)
    {
        if (left_ == &other || right_ == &other)
            return;

        auto my_next  = right_;
        auto his_next = other.right_;
        
        remove();
        other.remove();

        insert(*his_next);
        other.insert(*my_next);
    }
        
    inline bool list_node::unique() const
    {
        return left_ == this;
    }

    void list_node::remove()
    {
        left_->right_ = right_;
        right_->left_ = left_;

        left_  = this;
        right_ = this;
    }

    void list_node::insert(list_node const& where)
    {
        left_  = where.left_;
        right_ = &where;
        
        left_->right_ = this;
        where.left_ = this;
    }
      
    /// 

    template<class T>
    void checked_delete()
    {
        static_assert(sizeof(T) != 0, "type in incomplete");
    }

} // details 

//////////////////////////////////////////////////////////////////////////
// linked_ptr 

template<class T>
linked_ptr<T>::linked_ptr(T* ptr)
    : ptr_(ptr)
{}
                     
template<class T>
linked_ptr<T>::~linked_ptr()
{
    details::checked_delete<T>();

    if (unique())
        delete ptr_;
}

template<class T>
linked_ptr<T>::linked_ptr(linked_ptr const& other)
    : ptr_ (other.ptr_ )
    , node_(other.node_)
{}

template<class T>
template<class U>
linked_ptr<T>::linked_ptr(linked_ptr<U> const& other)
    : ptr_ (other.get ())
    , node_(other.node())
{}

// template<class T>
// template<class U>
// linked_ptr<T>& linked_ptr<T>::operator=(linked_ptr<U> other)
// {
//     swap(other);
//     return *this;
// }

template<class T>
linked_ptr<T>& linked_ptr<T>::operator=(linked_ptr other)
{
    swap(other);
    return *this;
}

template<class T>
template<class U>
void linked_ptr<T>::reset(U* ptr)
{
    linked_ptr tmp(ptr);
    swap(tmp);
}

template<class T>
void linked_ptr<T>::swap(linked_ptr& other)
{
    if (ptr_ == other.ptr_)
        return;

    using std::swap;
    swap(ptr_ , other.ptr_ );
    swap(node_, other.node_);
}

template<class T>
bool linked_ptr<T>::unique() const
{
    return node_.unique() && ptr_ != nullptr;
}

template<class T>
T* linked_ptr<T>::get() const
{
    return ptr_;
}

template<class T>
T* linked_ptr<T>::operator->() const
{
    return ptr_;
}

template<class T>
T& linked_ptr<T>::operator* () const
{
    return *ptr_;
}

template<class T>
linked_ptr<T>::operator bool() const
{
    return ptr_ != nullptr;
}

template<class T>
details::list_node const& linked_ptr<T>::node() const
{
    return node_;
}


} // smart_ptr 

