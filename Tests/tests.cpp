//
// Created by User on 03.11.2018.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../Vector.h"
#include "../Heap.h"
#include "../BinomialHeap.h"
#include "../FibonacciHeap.h"

using testing::Eq;

// Vector tests

TEST(Vector, Empty_Size_Constructor) {
    Vector<int> a = Vector<int> ();
    EXPECT_EQ(a.size(), 0);
}

TEST(Vector, Push_Back) {
    Vector<int> a = Vector<int>();
    a.Push_Back(1);
    a.Push_Back(3);
    a.Push_Back(7);
    a.Push_Back(8);
    a.Push_Back(9);
    a.Push_Back(5);
    a[5] = 100;

    EXPECT_EQ(a.size(), 6);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 3);
    EXPECT_EQ(a[2], 7);
    EXPECT_EQ(a[3], 8);
    EXPECT_EQ(a[4], 9);
    EXPECT_EQ(a[5], 100);
}

TEST (Vector, Vector_Constructor) {
    Vector<int> z;
    Vector<int> a(6, 1, 3, 7, 8, 9, 10);
    a[5] = 100;

    EXPECT_EQ(a.size(), 6);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 3);
    EXPECT_EQ(a[2], 7);
    EXPECT_EQ(a[3], 8);
    EXPECT_EQ(a[4], 9);
    EXPECT_EQ(a[5], 100);

    Vector<int> b(0);
    EXPECT_EQ(b.size(), 0);

    Vector<int> c(10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    for (int i = 0; i < 10; ++i)
        EXPECT_EQ(c[i], i + 1);

}

TEST(Vector, Pop_Back) {
    Vector<int> a(6, 1, 3, 7, 8, 9, 5);

    a.Pop_Back();
    EXPECT_EQ(a.size(), 5);
    a.Pop_Back();
    EXPECT_EQ(a.size(), 4);
    a.Pop_Back();
    EXPECT_EQ(a.size(), 3);
    a.Pop_Back();
    EXPECT_EQ(a.size(), 2);
    a.Pop_Back();
    EXPECT_EQ(a.size(), 1);
    EXPECT_EQ(a[0], 1);
}

TEST(Vector, Copy_Constructor) {
    Vector<int> b(6, 1, 3, 7, 8, 9, 5);
    Vector<int> a(b);

    a.Pop_Back();
    EXPECT_EQ(a.size(), 5);
    a.Pop_Back();
    EXPECT_EQ(a.size(), 4);
    a.Pop_Back();
    EXPECT_EQ(a.size(), 3);
    a.Pop_Back();
    EXPECT_EQ(a.size(), 2);
    a.Pop_Back();
    EXPECT_EQ(a.size(), 1);
    EXPECT_EQ(a[0], 1);
}

TEST(Vector, Another_Tests) {
    Vector<int> b(6, 1, 3, 7, 8, 9, 5);
    Vector<int> a = b;
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(a[0], 1);

    b[0] = 7;
    EXPECT_EQ(b[0], 7);
    EXPECT_EQ(a[0], 1);

    a[1] = 8;
    EXPECT_EQ(b[1], 3);
    EXPECT_EQ(a[1], 8);
}

TEST(Vector, Expect_Throws) {
    Vector<int> b(6, 1, 3, 7, 8, 9, 5);
    EXPECT_THROW(b[-1], std::out_of_range);
    EXPECT_THROW(Vector<int>(-1,1), std::logic_error);
    EXPECT_NO_THROW(b[0]);
}

// Heap tests

TEST(Heap, EmptySizeConstructor_IsEmpty) {
    Heap<int> a;
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(a.is_empty(), true);
}

TEST(Heap, Insert_GetMin) {
    Heap<int> a;
    a.insert(3);
    a.insert(2);
    a.insert(1);
    a.insert(3);
    a.insert(2);
    EXPECT_EQ(a.get_min(), 1);
}

TEST(Heap, Extract_Min) {
    Heap<int> a = Heap<int>();
    EXPECT_EQ(a.is_empty(), true);
    a.insert(3);
    EXPECT_EQ(a.is_empty(), false);
    a.insert(2);
    a.insert(1);
    EXPECT_EQ(a.extract_min(), 1);
    EXPECT_EQ(a.extract_min(), 2);
    EXPECT_EQ(a.extract_min(), 3);
    EXPECT_EQ(a.is_empty(), true);
}

TEST(Heap, Heap_Constructor) {
    Vector<int> b(5, 5, 4, 3, 2, 1);
    Heap<int> a(b);
    EXPECT_EQ(a.size(), b.size());
    EXPECT_EQ(a.size(), 5);
    EXPECT_EQ(a.extract_min(), 1);
    EXPECT_EQ(a.get_min(), 2);
    EXPECT_EQ(a.size(), 4);
    EXPECT_EQ(a.extract_min(), 2);
    EXPECT_EQ(a.extract_min(), 3);
    EXPECT_EQ(a.extract_min(), 4);
    EXPECT_EQ(a.extract_min(), 5);

}

TEST(Heap, ExtractMin_BigTest1) {
    Heap<int> a = Heap<int>();
    std::multiset<int> Q;
    for (int i = 0; i < 10000; ++i) {
        int elem = std::rand();
        Q.insert(elem);
        a.insert(elem);
    }
    while (!a.is_empty()) {
        EXPECT_EQ(a.extract_min(), *(Q.begin()));
        Q.erase(Q.begin());
    }
    EXPECT_EQ(a.is_empty(), Q.empty());
    a.insert(5);
    a.extract_min();
}

TEST(Heap, ExtractMin_BigTest2) {
    Vector<int> a;
    std::multiset<int> Q;
    for (int i = 0; i < 10000; ++i) {
        int elem = std::rand();
        Q.insert(elem);
        a.Push_Back(elem);
    }
    Heap<int> H(a);
    while (!H.is_empty()) {
        EXPECT_EQ(H.extract_min(), *(Q.begin()));
        Q.erase(Q.begin());
    }
    EXPECT_EQ(H.is_empty(), Q.empty());
}

TEST(Heap, DeleteChange_Pointer1) {
    Heap<int> a;
    Heap<int>::Pointer Pointer_3 = a.insert(3);
    Heap<int>::Pointer Pointer_2 = a.insert(2);
    Heap<int>::Pointer Pointer_1 = a.insert(1);
    EXPECT_EQ(a.get_min(), 1);
    a.change(Pointer_1, 7);
    EXPECT_EQ(a.get_min(), 2);
    a.change(Pointer_3, 0);
    EXPECT_EQ(a.get_min(), 0);
    a.Delete(Pointer_3);
    EXPECT_EQ(a.get_min(), 2);
}

TEST(Heap, DeleteChange_Pointer2) {
    Heap<int> H;
    std::multiset<int> Q;
    Vector<int> b;
    Vector<Heap<int>::Pointer> Pointers;
    for (int i = 0; i < 10000; i++) {
        int elem = std::rand();
        Q.insert(elem);
        b.Push_Back(elem);
        Pointers.Push_Back(H.insert(elem));
        EXPECT_EQ(Pointers[i].get_elem(), b[i]);
    }
    for (int i = 0; i < 10000; i++) {
        EXPECT_EQ(Pointers[i].get_elem(), b[i]);
    }
    for (int i = 0; i < 10000; i++) {
        int j = (std::rand() % 10000);
        int NewElem = std::rand();
        int elem = Pointers[j].get_elem();
        Q.erase(Q.find(elem));
        Q.insert(NewElem);
        H.change(Pointers[j], NewElem);
        b[j] = NewElem;
        EXPECT_EQ(Pointers[j].get_elem(), b[j]);
        EXPECT_EQ(H.get_min(), (*Q.begin()));
    }
    for (int i = 0; i < 10000; i++) {
        EXPECT_EQ(H.get_min(), (*Q.begin()));
        int elem = Pointers[i].get_elem();
        H.Delete(Pointers[i]);
        Q.erase(Q.find(elem));
    }
    EXPECT_TRUE(H.is_empty());
}


TEST(Heap, Expect_Throws) {
    Heap<int> a;
    EXPECT_THROW(a.get_min(), std::logic_error);
    EXPECT_THROW(a.extract_min(), std::logic_error);
}

// BinomialHeap tests

TEST(BinomialHeap, EmptySizeConstructor_IsEmpty) {
    BinomialHeap<int> a;
    EXPECT_EQ(a.is_empty(), true);
    BinomialHeap<int> b(5);
    EXPECT_EQ(b.is_empty(), false);
}

TEST(BinomialHeap, Insert) {
    BinomialHeap<int> a;
    EXPECT_EQ(a.is_empty(), true);
    a.insert(5);
    a.insert(1);
    a.insert(3);
    a.insert(7);
    a.insert(4);
    EXPECT_EQ(a.is_empty(), false);
}

TEST(BinomialHeap, Get_Min) {
    BinomialHeap<int> a;
    a.insert(5);
    a.insert(3);
    a.insert(4);
    a.insert(7);
    a.insert(1);
    EXPECT_EQ(a.get_min(), 1);
}

TEST(BinomialHeap, GetMin_BigTest) {
    BinomialHeap<int> a;
    std::multiset<int> Q;
    for (int i = 0; i < 10000; ++i) {
        int elem = std::rand();
        Q.insert(elem);
        a.insert(elem);
        EXPECT_EQ(a.get_min(), *(Q.begin()));
    }
}

TEST(BinomialHeap, Extract_Min) {
    BinomialHeap<int> a;
    a.insert(5);
    a.insert(3);
    a.insert(4);
    a.insert(7);
    a.insert(1);
    EXPECT_EQ(a.extract_min(), 1);
    EXPECT_EQ(a.extract_min(), 3);
    EXPECT_EQ(a.extract_min(), 4);
    EXPECT_EQ(a.extract_min(), 5);
    EXPECT_EQ(a.extract_min(), 7);
}

TEST(BinomialHeap, ExtractMin_BigTest) {
    BinomialHeap<int> a;
    std::multiset<int> Q;
    for (int i = 0; i < 10000; ++i) {
        int elem = std::rand();
        Q.insert(elem);
        a.insert(elem);
    }
    for (int i = 0; i < 10000; ++i) {
        int elem = *(Q.begin());
        Q.erase(Q.begin());
        EXPECT_EQ(elem, a.extract_min());
    }
    EXPECT_EQ(a.is_empty(), true);
}

TEST(BinomialHeap, MergeBinomialHeaps) {
    Vector<BinomialHeap<int>> a(10, BinomialHeap<int>());
    Vector<std::multiset<int>> b(10, std::multiset<int>());
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 200; ++j) {
            int elem = std::rand();
            a[i].insert(elem);
            b[i].insert(elem);
        }
    }
    BinomialHeap<int> BinomHeap;
    std::multiset<int> Q;
    for (int i = 0; i < 10; ++i) {
        BinomHeap.merge(a[i]);
        for (int j = 0; j < 200; ++j) {
            Q.insert(*b[i].begin());
            b[i].erase(b[i].begin());
        }
    }

    while (!BinomHeap.is_empty()) {
        EXPECT_EQ(*Q.begin(), BinomHeap.extract_min());
        Q.erase(Q.begin());
    }

    EXPECT_EQ(Q.empty(), true);
}

TEST(BinomialHeap, InsertDelete) {
    BinomialHeap<int> a;
    BinomialHeap<int>::Pointer Pointer1_ = a.insert(5);
    BinomialHeap<int>::Pointer Pointer2_ = a.insert(3);
    BinomialHeap<int>::Pointer Pointer3_ = a.insert(4);
    BinomialHeap<int>::Pointer Pointer4_ = a.insert(7);
    BinomialHeap<int>::Pointer Pointer5_ = a.insert(1);
    EXPECT_EQ(a.get_min(), 1);
    EXPECT_NO_THROW(a.Delete(Pointer5_));
    EXPECT_EQ(a.get_min(), 3);
    EXPECT_NO_THROW(a.change(Pointer1_,1));
    EXPECT_EQ(a.get_min(), 1);
    EXPECT_NO_THROW(a.Delete(Pointer1_));
    EXPECT_EQ(a.get_min(), 3);
    EXPECT_NO_THROW(a.Delete(Pointer2_));
    EXPECT_EQ(a.get_min(), 4);
    EXPECT_NO_THROW(a.Delete(Pointer3_));
    EXPECT_EQ(a.get_min(), 7);
    EXPECT_NO_THROW(a.Delete(Pointer4_));
    EXPECT_EQ(a.is_empty(), true);
}

TEST(BinomialHeap, InsertDelete2) {
    BinomialHeap<int> H;
    std::multiset<int> Q;
    Vector<int> b;
    Vector<BinomialHeap<int>::Pointer> Pointers;
    for (int i = 0; i < 10000; i++) {
        int elem = std::rand();
        Q.insert(elem);
        b.Push_Back(elem);
        Pointers.Push_Back(H.insert(elem));
        EXPECT_EQ(Pointers[i].get_elem(), b[i]);
    }
    for (int i = 0; i < 10000; i++) {
        EXPECT_EQ(Pointers[i].get_elem(), b[i]);
    }
    for (int i = 0; i < 10000; i++) {
        int j = (std::rand() % 10000);
        int NewElem = std::rand();
        int elem = Pointers[j].get_elem();
        Q.erase(Q.find(elem));
        Q.insert(NewElem);
        H.change(Pointers[j], NewElem);
        b[j] = NewElem;
        EXPECT_EQ(Pointers[j].get_elem(), b[j]);
        EXPECT_EQ(H.get_min(), (*Q.begin()));
    }
    for (int i = 0; i < 10000; i++) {
        EXPECT_EQ(H.get_min(), (*Q.begin()));
        int elem = Pointers[i].get_elem();
        H.Delete(Pointers[i]);
        Q.erase(Q.find(elem));
    }
    EXPECT_TRUE(H.is_empty());
}

TEST(BinomialHeap, Expect_Throws) {
    BinomialHeap<int> a;
    EXPECT_THROW(a.get_min(), std::logic_error);
    EXPECT_THROW(a.extract_min(), std::logic_error);
    a.insert(5);
    EXPECT_EQ(a.is_empty(), false);
    EXPECT_NO_THROW(a.extract_min());
    EXPECT_THROW(a.extract_min(), std::logic_error);
}

// Fibonacci Heap

TEST(FibonacciHeap, EmptySizeConstructor_IsEmpty) {
    FibonacciHeap<int> a;
    EXPECT_EQ(a.is_empty(), true);
}

TEST(FibonacciHeap, Insert_GetMin) {
    FibonacciHeap<int> a;
    a.insert(5);
    a.insert(7);
    a.insert(9);
    EXPECT_EQ(a.get_min(), 5);
    a.insert(3);
    a.insert(4);
    EXPECT_EQ(a.get_min(), 3);
    a.insert(1);
    EXPECT_EQ(a.get_min(), 1);
    EXPECT_EQ(a.is_empty(), false);
}

TEST(FibonacciHeap, Insert_ExtractMin) {
    FibonacciHeap<int> a;
    a.insert(7);
    a.insert(5);
    a.insert(6);
    a.insert(3);
    a.insert(1);
    a.insert(9);
    a.insert(2);
    a.insert(10);
    EXPECT_EQ(a.extract_min(), 1);
    EXPECT_EQ(a.extract_min(), 2);
    EXPECT_EQ(a.extract_min(), 3);
    EXPECT_EQ(a.extract_min(), 5);
    EXPECT_EQ(a.extract_min(), 6);
    EXPECT_EQ(a.extract_min(), 7);
}

TEST(FibonacciHeap, ExtractMin_BigTest) {
    FibonacciHeap<int> a;
    std::multiset<int> Q;
    for (int i = 0; i < 10000; ++i) {
        int elem = std::rand();
        Q.insert(elem);
        a.insert(elem);
    }
    for (int i = 0; i < 10000; ++i) {
        int elem = *(Q.begin());
        Q.erase(Q.begin());
        EXPECT_EQ(elem, a.extract_min());
    }
    EXPECT_EQ(a.is_empty(), true);
}

TEST(FibonacciHeap, MergeFibonacciHeaps) {
    Vector<FibonacciHeap<int>> a(10, FibonacciHeap<int>());
    Vector<std::multiset<int>> b(10, std::multiset<int>());
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 200; ++j) {
            int elem = std::rand();
            a[i].insert(elem);
            b[i].insert(elem);
        }
    }
    FibonacciHeap<int> FibHeap;
    std::multiset<int> Q;
    for (int i = 0; i < 10; ++i) {
        FibHeap.merge(a[i]);
        for (int j = 0; j < 200; ++j) {
            Q.insert(*b[i].begin());
            b[i].erase(b[i].begin());
        }
    }

    while (!FibHeap.is_empty()) {
        EXPECT_EQ(*Q.begin(), FibHeap.extract_min());
        Q.erase(Q.begin());
    }

    EXPECT_EQ(Q.empty(), true);
}

TEST(FibonacciHeap, Decrease_Key1) {
    FibonacciHeap<int> a;
    a.insert(7);
    a.insert(5);
    a.insert(6);
    FibonacciHeap<int>::Pointer b = a.insert(3);
    a.insert(1);
    FibonacciHeap<int>::Pointer c = a.insert(9);
    a.insert(2);
    a.insert(10);
    a.decreaseKey(b,0);
    EXPECT_EQ(a.get_min(), 0);
    EXPECT_EQ(a.extract_min(), 0);
    a.decreaseKey(c,2);
    EXPECT_EQ(a.extract_min(), 1);
    EXPECT_EQ(a.extract_min(), 2);
    EXPECT_EQ(a.extract_min(), 2);
}

TEST(FibonacciHeap, Decrease_Key2) {
    FibonacciHeap<int> a;
    std::multiset<int> Q;
    Vector<FibonacciHeap<int>::Pointer> b;
    Vector<int> c;
    for (int i = 0; i < 10000; ++i) {
        int elem = std::rand();
        Q.insert(elem);
        b.Push_Back(a.insert(elem));
        c.Push_Back(elem);
    }
    for (int i = 0; i < 10000; ++i) {
        Q.erase(Q.find(c[i]));
        Q.insert(c[i]/2);
        a.decreaseKey(b[i],c[i]/2);
    }
    for (int i = 0; i < 10000; ++i) {
        int elem = *(Q.begin());
        Q.erase(Q.begin());
        EXPECT_EQ(a.extract_min(),elem);
    }
    EXPECT_EQ(a.is_empty(), true);
}

TEST(FibonacciHeap, Expect_Throws) {
    FibonacciHeap<int> a;
    EXPECT_THROW(a.get_min(), std::logic_error);
    EXPECT_THROW(a.extract_min(), std::logic_error);
    a.insert(5);
    EXPECT_EQ(a.is_empty(), false);
    EXPECT_NO_THROW(a.extract_min());
    EXPECT_THROW(a.extract_min(), std::logic_error);
}