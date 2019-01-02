//
// Created by User on 01.12.2018.
//

#ifndef LAB1_FIBONACCIHEAP_H
#define LAB1_FIBONACCIHEAP_H


#include <cassert>
#include <stdexcept>
#include "Vector.h"

const int VectorNodeSize = 70;
//Size for constant array we use in consolidate

template <typename Key>
class FibonacciHeap {

    struct Node;

public:

    struct Pointer;

    FibonacciHeap() {
        size_ = 0;
        MinElemNode_ = nullptr;
    }

    FibonacciHeap(Node *Node_) {
        size_ = 1;
        MinElemNode_ = Node_;
    }

    ~FibonacciHeap() {
        if (MinElemNode_ != nullptr) {
            DeleteFibonacciHeap(MinElemNode_);
        }
    }

    bool is_empty() const {
        return (size_ == 0);
    }

    Key get_min() const {
        if (size_ == 0) {
            throw std::logic_error("get minimum element from an empty fibonacci heap");
        }
        return (MinElemNode_->key);
    }

    Pointer insert(Key key) {
        Node *NewNode_ = new(Node);
        NewNode_->key = key;
        if (size_ == 0) {
            MinElemNode_ = NewNode_;
            MinElemNode_->left_ = MinElemNode_;
            MinElemNode_->right_ = MinElemNode_;
        } else {
            Node *RightMinElemNode_ = MinElemNode_->right_;
            MinElemNode_->right_ = NewNode_;
            NewNode_->left_ = MinElemNode_;
            NewNode_->right_ = RightMinElemNode_;
            RightMinElemNode_->left_ = MinElemNode_;
        }
        if (NewNode_->key < MinElemNode_->key) {
            MinElemNode_ = NewNode_;
        }
        size_++;
        Pointer* Pointer_ = new (Pointer);
        Pointer_->Node_ = NewNode_;
        return (*Pointer_);
    }

    void merge(FibonacciHeap<Key> &OtherHeap) {
        if (OtherHeap.is_empty()) {
            return;
        } else if (MinElemNode_ == nullptr) {
            MinElemNode_ = OtherHeap.MinElemNode_;
            size_ = OtherHeap.size_;
        } else {
            UnionLists(MinElemNode_, OtherHeap.MinElemNode_);
            size_ += OtherHeap.size_;
            if (OtherHeap.MinElemNode_->key < MinElemNode_->key) {
                MinElemNode_ = OtherHeap.MinElemNode_;
            }
        }
        OtherHeap.MinElemNode_ = nullptr;
        OtherHeap.size_ = 0;
    }

    Key extract_min() {
        if (size_ == 0) {
            throw std::logic_error("get minimum element from an empty fibonacci heap");
        }
        Key ans;
        size_t SavedSize = size_;
        Node *SavedMinElemNode_ = MinElemNode_;
        if (size_ == 1) {
            ans = MinElemNode_->key;
            MinElemNode_ = nullptr;
            delete (SavedMinElemNode_);
        } else {
            if (MinElemNode_->child_ != nullptr) {
                Node *MinElemNodeChild_ = MinElemNode_->child_;
                MinElemNode_->child_->parent_ = nullptr;
                MinElemNode_->child_ = nullptr;
                UnionLists(MinElemNode_, MinElemNodeChild_);
            }
            Node *LeftMinElemNode_ = MinElemNode_->left_;
            Node *RightMinElemNode_ = MinElemNode_->right_;
            LeftMinElemNode_->right_ = RightMinElemNode_;
            RightMinElemNode_->left_ = LeftMinElemNode_;
            MinElemNode_ = LeftMinElemNode_;
            consolidate();
            ans = SavedMinElemNode_->key;
            delete (SavedMinElemNode_);
        }
        size_ = SavedSize - 1;
        return ans;
    }

    void decreaseKey(Pointer Pointer_, Key key) {
        if (Pointer_.Node_->parent_ == nullptr) {
            Pointer_.Node_->key = key;
            if (Pointer_.Node_->key < MinElemNode_->key) {
                MinElemNode_ = Pointer_.Node_;
            }
            return;
        } else if (Pointer_.Node_->parent_->key <= key) {
            Pointer_.Node_->key = key;
            return;
        } else {
            Pointer_.Node_->key = key;
            Node *ParentPointerNode_ = Pointer_.Node_->parent_;
            Cut(Pointer_.Node_);
            CascadingCut(ParentPointerNode_);
        }
    }

    struct Pointer {
        friend class FibonacciHeap<Key>;

        Pointer() {
            Node_ = nullptr;
        }

        ~Pointer() {
            Node_ = nullptr;
        }

    private:
        Node* Node_;
    };

private:

    size_t size_;
    Node *MinElemNode_;

    struct Node {
        Key key;
        size_t degree;
        bool mark;
        Node *child_;
        Node *left_;
        Node *right_;
        Node *parent_;

        Node() {
            degree = mark = 0;
            child_ = left_ = right_ = parent_ = nullptr;
        }

        ~Node() {}

    };

    template<typename T>
    void swap(T &a, T &b) {
        T tmp = a;
        a = b;
        b = tmp;
    }

    void DeleteFibonacciHeap(Node* Node_) {
        assert(Node_ != nullptr);
        Node_->left_->right_ = nullptr;
        while(Node_ != nullptr) {
            Node* tmp_ = Node_;
            Node_ = Node_->right_;
            if (tmp_->child_ != nullptr) {
                DeleteFibonacciHeap(tmp_->child_);
            }
            delete(tmp_);
        }
    }

    void ClearNode(Node *Node_) {
        if (Node_ != nullptr) {
            Node_->left_ = Node_;
            Node_->right_ = Node_;
            Node_->parent_ = nullptr;
        }
    }

    Node *UnionLists(Node *FirstNode_, Node *SecondNode_) {
        assert (FirstNode_ != nullptr || SecondNode_ != nullptr);
        if (FirstNode_ == nullptr)
            return SecondNode_;
        if (SecondNode_ == nullptr)
            return FirstNode_;
        Node *LeftFirstNode_ = FirstNode_->left_;
        Node *RightSecondNode_ = SecondNode_->right_;
        SecondNode_->right_ = FirstNode_;
        FirstNode_->left_ = SecondNode_;
        LeftFirstNode_->right_ = RightSecondNode_;
        RightSecondNode_->left_ = LeftFirstNode_;
        return FirstNode_;
    }

    void consolidate() {
        assert(size_ > 0 && MinElemNode_ != nullptr);
        Vector<Node *> Roots(VectorNodeSize, nullptr);
        Node *tmp_ = MinElemNode_->right_;
        ClearNode(MinElemNode_);
        Roots[MinElemNode_->degree] = MinElemNode_;
        while (tmp_ != MinElemNode_) {
            Node *FirstNode_ = tmp_;
            tmp_ = tmp_->right_;
            ClearNode(FirstNode_);
            while (Roots[FirstNode_->degree] != nullptr) {
                Node *SecondNode_ = Roots[FirstNode_->degree];
                Roots[FirstNode_->degree] = nullptr;
                if (SecondNode_->key < FirstNode_->key)
                    swap(FirstNode_, SecondNode_);
                FirstNode_->child_ = UnionLists(FirstNode_->child_, SecondNode_);
                SecondNode_->parent_ = FirstNode_;
                (FirstNode_->degree)++;
            }
            Roots[FirstNode_->degree] = FirstNode_;
        }
        int i = 0;
        while (Roots[i] == nullptr) {
            i++;
        }
        MinElemNode_ = Roots[i];
        i++;
        while (i < VectorNodeSize) {
            if (Roots[i] != nullptr) {
                FibonacciHeap<Key> OtherHeap(Roots[i]);
                merge(OtherHeap);
                OtherHeap.MinElemNode_ = nullptr;
            }
            i++;
        }
    }

    void Cut(Node *Node_) {
        assert(Node_ != nullptr);
        assert(Node_->parent_ != nullptr);
        Node_->left_->right_ = Node_->right_;
        Node_->right_->left_ = Node_->left_;
        if (Node_->parent_->child_ == Node_) {
            if (Node_ == Node_->right_) {
                Node_->parent_->child_ = nullptr;
            } else {
                Node_->parent_->child_ = Node_->right_;
            }
        }
        Node_->left_ = Node_->right_ = Node_;
        (Node_->parent_->degree)--;
        Node_->parent_ = nullptr;
        FibonacciHeap<Key> OtherHeap(Node_);
        merge(OtherHeap);
        OtherHeap.MinElemNode_ = nullptr;
    }

    void CascadingCut(Node *Node_) {
        while (Node_->mark && Node_->parent_ != nullptr) {
            Cut(Node_);
            CascadingCut(Node_->parent_);
        }
        Node_->mark = true;
    }
};

#endif //LAB1_FIBONACCIHEAP_H
