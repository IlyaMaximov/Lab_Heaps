//
// Created by User on 23.11.2018.
//

#ifndef LAB1_BINOMIAL_HEAP_H
#define LAB1_BINOMIAL_HEAP_H

#include <cassert>
#include <stdexcept>

template <typename Key>
class BinomialHeap {

    struct Node;

public:

    struct Pointer;

    explicit BinomialHeap() {
        head_ = nullptr;
        MinElemNode_ = nullptr;
    }

    explicit BinomialHeap(Key a) {
        head_ = new Node(a);
        MinElemNode_ = head_;
    }

    ~BinomialHeap() {
        if (head_ != nullptr)
            delete(head_);
    }

    bool is_empty() const {
        return (head_ == nullptr);
    }

    void merge(BinomialHeap<Key>& OtherHeap) {
        if (OtherHeap.is_empty()) {
            UpdateMinElemNode_();
            return ;
        }
        Node* OtherHeapNode_ = OtherHeap.head_;
        Node* MyHeapNode_ =  head_;
        Node* NewHeapNode_ = nullptr;

        //Adding OtherHeap to MyHeap
        while (OtherHeapNode_ != nullptr || MyHeapNode_ != nullptr) {
            Node* Node_;
            if (OtherHeapNode_ == nullptr || MyHeapNode_ != nullptr &&
                OtherHeapNode_ != nullptr && MyHeapNode_->degree <= OtherHeapNode_->degree) {
                 Node_ = MyHeapNode_;
                MyHeapNode_ = MyHeapNode_->sibling_;
            } else {
                Node_ = OtherHeapNode_;
                OtherHeapNode_ = OtherHeapNode_->sibling_;
            }
            if (NewHeapNode_ == nullptr) {
                NewHeapNode_ = Node_;
                head_ = NewHeapNode_;
            } else {
                NewHeapNode_->sibling_ = Node_;
                NewHeapNode_ = NewHeapNode_->sibling_;
            }
        }
        assert(NewHeapNode_->sibling_ == nullptr);

        //Union sibligs in MyHeap
        Node* FatherNode_ = nullptr;
        MyHeapNode_ = head_;
        while (MyHeapNode_->sibling_ != nullptr) {
            if (MyHeapNode_->degree == MyHeapNode_->sibling_->degree) {
                Node* tmp_ = MyHeapNode_->sibling_;
                if (MyHeapNode_->sibling_->value >= MyHeapNode_->value) {
                    tmp_->parent_ = MyHeapNode_;
                    MyHeapNode_->sibling_ = MyHeapNode_->sibling_->sibling_;
                    tmp_->sibling_ = MyHeapNode_->child_;
                    MyHeapNode_->child_ = tmp_;
                    MyHeapNode_->degree ++;
                } else {
                    if (FatherNode_ == nullptr) {
                        head_ = tmp_;
                    } else {
                        FatherNode_->sibling_ = tmp_;
                    }
                    MyHeapNode_->parent_ = tmp_;
                    MyHeapNode_->sibling_ = tmp_->child_;
                    tmp_->child_ = MyHeapNode_;
                    tmp_->degree++;
                    MyHeapNode_ = tmp_;
                }
            } else if (MyHeapNode_->degree > MyHeapNode_->sibling_->degree) {
                Node* tmp_ = MyHeapNode_->sibling_;
                if (FatherNode_ == nullptr) {
                    head_ = tmp_;
                } else {
                    FatherNode_->sibling_ = tmp_;
                }
                MyHeapNode_->sibling_ = tmp_->sibling_;
                tmp_->sibling_ = MyHeapNode_;
                MyHeapNode_ = tmp_;
            } else {
                FatherNode_ = MyHeapNode_;
                MyHeapNode_ = MyHeapNode_->sibling_;
            }
        }
        OtherHeap.head_ = nullptr;
        UpdateMinElemNode_();
    }

    Pointer insert(Key key) {
        BinomialHeap<Key>* b = new BinomialHeap<Key>(key);
        Node* NewNode_ = new Node;
        NewNode_->brother_ = b->head_;
        b->head_->brother_ = NewNode_;
        Pointer* NewPointer_ = new Pointer(NewNode_);
        merge(*b);
        return (*NewPointer_);
    }

    Key get_min() const {
        if (MinElemNode_ == nullptr) {
            throw std::logic_error("get minimum element from an empty binomial heap");
        }
        return (MinElemNode_->value);
    }

    Key extract_min() {
        if (MinElemNode_ == nullptr) {
            throw std::logic_error("extract minimum element from an empty binomial heap");
        }
        Key MinElem = get_min();
        if (MinElemNode_ == head_) {
            head_ = MinElemNode_->sibling_;
        } else {
            Node* Node_ = head_;
            while (Node_->sibling_ != MinElemNode_) {
                Node_ = Node_->sibling_;
            }
            Node_->sibling_ = MinElemNode_->sibling_;
        }

        BinomialHeap<int> TmpHeap;
        if (MinElemNode_->child_ != nullptr) {
            Node* Node_;
            Node* HeapNode_ = RecursiveBuildBinomialHeap(MinElemNode_->child_,Node_);
            HeapNode_->sibling_ = nullptr;
            TmpHeap.head_ = Node_;
        }
        MinElemNode_->child_ = nullptr;
        MinElemNode_->sibling_ = nullptr;
        if (MinElemNode_->brother_ != nullptr)
            delete(MinElemNode_->brother_);
        delete (MinElemNode_);

        merge(TmpHeap);
        return (MinElem);
      }

    void change(Pointer Pointer_, int NewValue) { // it is a renamed function change
        if (Pointer_.Node_ == nullptr) {
            throw std::logic_error("change a nonexistent element");
        }
        Node* Node_ = Pointer_.Node_->brother_;
        assert(Node_ != nullptr);
        if (NewValue <= Pointer_.Node_->brother_->value) {
            Node_->value = NewValue;
            while (Node_->parent_ != nullptr && Node_->parent_->value > Node_->value) {
                Swap(Node_->value, Node_->parent_->value);
                Swap_Brothers(Node_, Node_->parent_);
                Node_ = Node_->parent_;
            }
            UpdateMinElemNode_();
        }
        else {
            Node_->brother_ = nullptr;
            Delete(Pointer_);
            Pointer TmpPointer_ = insert(NewValue);
            Node_ = TmpPointer_.Node_->brother_;
            Pointer_.Node_->brother_ = Node_;
            Node_->brother_ = Pointer_.Node_;
        }
    }

    void Delete(Pointer Pointer_) {
        if (Pointer_.Node_ == nullptr) {
            throw std::logic_error("delete a nonexistent element");
        }
        change(Pointer_, -1e9);
        extract_min();
    }

    struct Pointer {
        friend class BinomialHeap<Key>;

        explicit Pointer() {
            Node_ = nullptr;
        }

        explicit Pointer(Node* NewNode) {
            Node_ = NewNode;
        }

        ~Pointer() { }

        Key get_elem() const {
            if (Node_ == nullptr) {
                throw std::logic_error("invalid pointer");
            } else {
                return Node_->brother_->value;
            }
        }

    private:
        Node* Node_;
    };

private:

    Node* head_;
    Node* MinElemNode_;

    struct Node {
        Key value;
        size_t degree;
        Node* child_;
        Node* sibling_;
        Node* parent_;
        Node* brother_;

        explicit Node() {
            degree = 0;
            child_ = sibling_ = parent_ = brother_ = nullptr;
        }

        explicit Node(Key a) {
            value = a;
            degree = 0;
            child_ = sibling_ = parent_ = brother_ =  nullptr;
        }

        ~Node() {
            if (child_ != nullptr) {
                delete(child_);
            }
            if (sibling_ != nullptr) {
                delete(sibling_);
            }
        }
    };

    template <typename T>
    void Swap(T& a, T& b) {
        T tmp = a;
        a = b;
        b = tmp;
    }

    void Swap_Brothers(Node* Node1_, Node* Node2_) {
        if (Node1_->brother_ != nullptr) {
            Node1_->brother_->brother_ = Node2_;
        }
        if (Node2_->brother_ != nullptr) {
            Node2_->brother_->brother_ = Node1_;
        }
        Swap(Node1_->brother_, Node2_->brother_);
    }

    void UpdateMinElemNode_() {
        Node* tmp_ = head_;
        MinElemNode_ = head_;
        while (tmp_ != nullptr) {
            if (tmp_->value < MinElemNode_->value) {
                MinElemNode_ = tmp_;
            }
            tmp_ = tmp_->sibling_;
        }
    }

    Node* RecursiveBuildBinomialHeap(Node* HeapNode_, Node*& HeapHead_) {
        assert(HeapNode_ != nullptr);
        HeapNode_->parent_ = nullptr;
        if (HeapNode_->sibling_ == nullptr) {
            return (HeapHead_ = HeapNode_);
        } else {
            Node* tmp_ = RecursiveBuildBinomialHeap(HeapNode_->sibling_, HeapHead_);
            tmp_->sibling_ = HeapNode_;
            return HeapNode_;
        }
    }
};


#endif //LAB1_BINOMIAL_HEAP_H
