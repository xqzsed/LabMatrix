#include "pch.h"
#include <sstream>
#include <random>

#define MEMDATA_TESTS 0
#define VECTOR_TESTS 0

#ifdef MEMDATA_TESTS
#include "memdata.h"

TEST(FunctionsForMemData, calculate_capacity) {
    EXPECT_EQ(0, calculate_capacity(0));
    EXPECT_EQ(15, calculate_capacity(1));
    EXPECT_EQ(15, calculate_capacity(15));
    EXPECT_EQ(30, calculate_capacity(16));
    EXPECT_EQ(30, calculate_capacity(30));
    EXPECT_EQ(45, calculate_capacity(31));
    EXPECT_EQ(45, calculate_capacity(45));
}

TEST(ClassMemData, can_create_with_default_constructor) {
    MemData<double> mem;

    EXPECT_EQ(0, mem.size());
    EXPECT_EQ(0, mem.capacity());
    EXPECT_TRUE(mem.is_empty());
}

TEST(ClassMemData, can_create_with_constructor_by_size) {
    MemData<double> mem(10);
    EXPECT_EQ(10, mem.size());
    EXPECT_EQ(15, mem.capacity());
}

TEST(ClassMemData, can_create_with_initializer_list) {
    MemData<double> mem({ 1.1, 2.2, 3.3 });

    EXPECT_EQ(3, mem.size());
    EXPECT_DOUBLE_EQ(1.1, mem.data()[0]);
    EXPECT_DOUBLE_EQ(2.2, mem.data()[1]);
    EXPECT_DOUBLE_EQ(3.3, mem.data()[2]);
}

TEST(ClassMemData, can_create_with_array) {
    double arr[] = { 1.1, 2.2, 3.3 };
    MemData<double> mem(arr, 3);

    EXPECT_EQ(3, mem.size());
    EXPECT_DOUBLE_EQ(1.1, mem.data()[0]);
}

TEST(ClassMemData, copy_constructor) {
    MemData<double> a({ 1.1, 2.2 });
    MemData<double> b(a);

    EXPECT_EQ(a.size(), b.size());
    EXPECT_NE(a.data(), b.data());
}

TEST(ClassMemData, move_constructor) {
    MemData<double> a({ 1.1, 2.2 });
    const double* ptr = a.data();

    MemData<double> b(std::move(a));

    EXPECT_EQ(ptr, b.data());
    EXPECT_EQ(0, a.size());
}

TEST(ClassMemData, set_memory) {
    MemData<double> mem({ 1.1, 2.2 });
    mem.set_memory(30);

    EXPECT_EQ(30, mem.capacity());
    EXPECT_DOUBLE_EQ(1.1, mem.data()[0]);
}

TEST(ClassMemData, reset_memory) {
    MemData<double> mem({ 1.1, 2.2, 3.3 });
    mem.reset_memory(30);

    EXPECT_EQ(30, mem.capacity());
    EXPECT_DOUBLE_EQ(1.1, mem.data()[0]);
}

TEST(ClassMemData, clear_memory) {
    MemData<double> mem({ 1.1, 2.2 });
    mem.clear_memory();

    EXPECT_EQ(0, mem.size());
    EXPECT_EQ(nullptr, mem.data());
}

TEST(ClassMemData, assignment) {
    MemData<double> a({ 1.1, 2.2 });
    MemData<double> b;
    b = a;

    EXPECT_EQ(a.size(), b.size());
    EXPECT_NE(a.data(), b.data());
}

TEST(ClassMemData, move_assignment) {
    MemData<double> a({ 1.1, 2.2 });
    MemData<double> b;

    b = std::move(a);

    EXPECT_EQ(0, a.size());
    EXPECT_EQ(2, b.size());
}

#endif



#ifdef VECTOR_TESTS
#include "vector.h"

TEST(ClassVector, default_constructor) {
    TVector<double> v;
    EXPECT_TRUE(v.is_empty());
}

TEST(ClassVector, size_constructor) {
    TVector<double> v(10);
    EXPECT_EQ(10, v.size());
}

TEST(ClassVector, initializer_list) {
    TVector<double> v({ 1,2,3 });
    EXPECT_DOUBLE_EQ(2, v[1]);
}

TEST(ClassVector, push_back) {
    TVector<double> v;
    v.push_back(1);
    v.push_back(2);

    EXPECT_EQ(2, v.size());
    EXPECT_DOUBLE_EQ(2, v.back());
}

TEST(ClassVector, push_front) {
    TVector<double> v;
    v.push_front(2);
    v.push_front(1);

    EXPECT_DOUBLE_EQ(1, v.front());
}

TEST(ClassVector, insert) {
    TVector<double> v({ 1,2,4 });
    v.insert(3, 1);

    EXPECT_DOUBLE_EQ(3, v[1]);
}

TEST(ClassVector, pop_back) {
    TVector<double> v({ 1,2,3 });
    v.pop_back();

    EXPECT_EQ(2, v.size());
}

TEST(ClassVector, pop_front) {
    TVector<double> v({ 1,2,3 });
    v.pop_front();

    EXPECT_DOUBLE_EQ(2, v[0]);
}

TEST(ClassVector, erase) {
    TVector<double> v({ 1,2,3 });
    v.erase(1);

    EXPECT_DOUBLE_EQ(3, v[1]);
}

TEST(ClassVector, sort) {
    TVector<double> v({ 3,1,2 });
    v.sort();

    EXPECT_DOUBLE_EQ(1, v[0]);
    EXPECT_DOUBLE_EQ(2, v[1]);
    EXPECT_DOUBLE_EQ(3, v[2]);
}

TEST(ClassVector, shuffle) {
    TVector<double> v({ 1,2,3,4,5 });
    v.shuffle();

    EXPECT_EQ(5, v.size());
}

TEST(ClassVector, copy_assignment) {
    TVector<double> a({ 1,2 });
    TVector<double> b;

    b = a;

    EXPECT_EQ(a.size(), b.size());
}

TEST(ClassVector, move_assignment) {
    TVector<double> a({ 1,2 });
    TVector<double> b;

    b = std::move(a);

    EXPECT_EQ(0, a.size());
}

#endif