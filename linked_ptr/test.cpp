
#include <iostream>
#include <assert.h>

#include "linked_ptr.h"
                          
using namespace smart_ptr;

static void test_empty_lptr()
{
    linked_ptr<int> lpi_empty;
    assert(!lpi_empty);
    lpi_empty.reset();
    assert(!lpi_empty);
    assert(!lpi_empty.get());
    assert(!lpi_empty.unique());
}

static void test_one_lptr()
{
    linked_ptr<int> lpi_single(new int(777));
    assert(lpi_single);
    lpi_single.reset(new int(666));
    assert(lpi_single);
    assert(lpi_single.get());
    assert(lpi_single.unique());
}

static void test_simple_assignment()
{
    using namespace std;
    linked_ptr<int> ptr1(new int(5));
    linked_ptr<int> ptr2(new int(6));
    assert(*ptr1 == 5);
    assert(*ptr2 == 6);
    ptr1 = ptr2;
    assert(*ptr1 == 6);
    assert(*ptr2 == 6);
}

static void test_self_assignment()
{
    using namespace std;
    linked_ptr<int> ptr1(new int(5));
    ptr1 = ptr1;
    assert(*ptr1 == 5);
    assert(ptr1.unique());
}

static void test_derived_assignment()
{
    struct base {};
    struct derived : base {};

    using namespace std;
    linked_ptr<base> ptr1(new base());
    linked_ptr<derived> ptr2(new derived());
    ptr1 = ptr2;
    
    linked_ptr<base> ptr3;
    auto x = new derived;
    ptr3.reset(x);

    assert(&(*ptr1) == &(*ptr2));
    assert(!ptr1.unique());
    assert(!ptr2.unique());
}

static void test_swap_yourself()
{
    using namespace std;
    linked_ptr<int> lp(new int(5));
    std::swap(lp, lp);
    assert(*lp == 5);
}

static void test_swap_neighbors()
{
    using namespace std;
    linked_ptr<int> a(new int(5));
    linked_ptr<int> b(a);
    linked_ptr<int> c(a);
    std::swap(a, b);
    assert(*a == 5);
    assert(*b == 5);
    assert(*c == 5);
}

static void test_double_swap()
{
    using namespace std;
    linked_ptr<int> a(new int(5));
    std::swap(a, a);
    assert(*a == 5);
    linked_ptr<int> b(a);
    std::swap(a, b);
    assert(*a == 5);
    assert(*b == 5);
}

static void test_swap()
{
    using namespace std;
    linked_ptr<int> a(new int(5));
    linked_ptr<int> a1(a);
    assert(*a1 == 5);
    linked_ptr<int> a2;
    a2 = a;
    assert(*a2 == 5);
    linked_ptr<int> b(new int(6));
    linked_ptr<int> b1;
    b1 = b;
    assert(*b1 == 6);
    linked_ptr<int> b2(b1);
    assert(*b2 == 6);
    std::swap(a, b);
    assert(*a == 6);
    assert(*b == 5);
    std::swap(a, b);
    assert(*a == 5);
    assert(*b == 6);
    std::swap(a1, b1);
    assert(*a1 == 6);
    assert(*b1 == 5);
    std::swap(a1, b1);
    std::swap(a2, b2);
    assert(*a2 == 6);
    assert(*b2 == 5);
}

static void test_many_lptr()
{
    linked_ptr<int> lp;
    lp = linked_ptr<int>(new int(777));
    auto lp1(lp);
    auto lp2(lp1);
    auto lp3(lp2);
    auto lp4(lp);
    auto lp5(lp1);
    auto lp6(lp1);
    auto lp7(lp3);
    auto lp8(lp7);
    auto lp9(lp5);
    lp7.reset();
    auto lp10(lp7);
    auto lp11(lp6);
    auto lp12(lp1);
    auto lp13(lp2);
    lp13.reset(new int(666));
    auto lp14(lp3);
    auto lp15(lp13);
    auto lp16(lp12);
    auto lp17(lp15);
}

static void test_lptr_conversions()
{
    struct base
    {
        virtual bool is_base() { return true; }
        virtual ~base() {}
    };
    struct derived : base
    {
        virtual bool is_base() override { return false; }
    };
    linked_ptr<base> lptr(new base());
    assert(lptr->is_base());
    lptr = linked_ptr<base>(new derived());
    assert(!lptr->is_base());
}

static void test_lptr_delete()
{
    struct foo;

    struct qwe
    {
        void init_with_incomplete(foo* ptr)
        {
            linked_ptr<foo> lptr(ptr);
            (void)lptr;
        }
    };
    struct foo {}; //compiler error if not defined here
}

// static void test_bonus_non_complete_type()
// {
//     struct foo;
//     linked_ptr<foo> not_initiazed_ptr;
//     (void)not_initiazed_ptr;
// }

int main()
{
    std::cout << "test_empty_lptr" << std::endl;
    test_empty_lptr();
    std::cout << "test_one_lptr" << std::endl;
    test_one_lptr();
    std::cout << "test_simple_assignment" << std::endl;
    test_simple_assignment();
    std::cout << "test_self_assignment" << std::endl;
    test_self_assignment();
    std::cout << "test_swap" << std::endl;
    test_swap();
    std::cout << "test_double_swap" << std::endl;
    test_double_swap();
    std::cout << "test_swap_yourself" << std::endl;
    test_swap_yourself();
    std::cout << "test_swap_neighbors" << std::endl;
    test_swap_neighbors();
    std::cout << "test_many_lptr" << std::endl;
    test_many_lptr();
    std::cout << "test_lptr_conversions" << std::endl;
    test_lptr_conversions();
    std::cout << "test_lptr_delete" << std::endl;
    test_lptr_delete();
    std::cout << "test_derived_assignment" << std::endl;
    test_derived_assignment();
//     std::cout << "test_bonus_non_complete_type" << std::endl;
//     test_bonus_non_complete_type();
    return 0;
}