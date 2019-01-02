//
// Created by User on 11.11.2018.
//

#ifndef LAB1_HEAP_H
#define LAB1_HEAP_H

#include <cassert>
#include "Vector.h"

template <typename Key>
class Heap {

struct Node;

public:

    class Pointer;

    explicit Heap() {
        array_ = Vector<Node>();
        array_memory = 0;
    }

    explicit Heap(const Vector<Key>& init_values) {
        array_.Clear(); 
        for (int i = 0; i < init_values.size(); i++) {
            array_.Push_Back(Node(init_values[i]));
            array_[i].NumberInArray = i;
        }
        for (int i = array_.size() / 2; i >= 0 && !init_values.is_empty(); i--) {
               SiftDown(i);
        }

        if (init_values.size() == 0) {
            array_memory = 0;
        } else {
            array_memory = 1;
            while (array_memory < init_values.size()) {
                array_memory *= 2;
            }
        }
    }

    ~Heap() { };

    size_t size() const {
        return array_.size();
    }

    bool is_empty() const {
        return (array_.size() == 0);
    }

    Pointer insert(Key key) {
        Node* son = new(Node);
        Node* father = new(Node);
        Push_in_array(*son);
        son = &(array_[array_.size() - 1]);
        son->brother_ = father;
        son->value = key;
        son->NumberInArray = array_.size() - 1;
        father->brother_ = son;
        Pointer* Pointer_ = new Pointer;
        Pointer_->Node_ = father;
        SiftUp(array_.size() - 1);
        return (*Pointer_);
    }

    Key get_min() const {
        if (array_.size() <= 0) {
            throw std::logic_error("get minimum element from an empty heap");
        }
        return array_[0].value;
    }

    Key extract_min() {
        if (array_.size() <= 0) {
            throw std::logic_error("extract minimum element from an empty heap");
        }
        Key ans = array_[0].value;
        SwapArrayElements(array_[0], array_[array_.size() - 1]);
        delete(array_[array_.size() - 1].brother_);
        Remove_from_array();
        SiftDown(0);
        return ans;
    }

    void Delete(Pointer Pointer_) {
        if (Pointer_.Node_ == nullptr) {
            throw std::logic_error("deleting a nonexistent element from the heap");
        }
        int index = Pointer_.Node_->brother_->NumberInArray;
        SwapArrayElements(array_[index], array_[array_.size() - 1]);
        delete(array_[array_.size() - 1].brother_);
        Remove_from_array();
        SiftDown(index);
        SiftUp(index);
    }

    void change(Pointer Pointer_, Key key) {
        if (Pointer_.Node_ == nullptr) {
            throw std::logic_error("changing a nonexistent element from the heap");
        }
        int index = Pointer_.Node_->brother_->NumberInArray;
        array_[index].value = key;
        SiftDown(index);
        SiftUp(index);
    }

    struct Pointer {
        friend class Heap<Key>;

        explicit Pointer() {
            Node_ = nullptr;
        }

        ~Pointer() {}

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

    Vector<Node> array_;
    int array_memory = 0;

    struct Node {

        explicit Node() {
            value = 0;
            brother_ = nullptr;
        }

        explicit Node(Key key) {
            value = key;
            brother_ = nullptr;
        }

        Node& operator = (const Node& a) {
            value = a.value;
            NumberInArray = a.NumberInArray;
            brother_  = a.brother_;
            return (*this);
        }

        ~Node() {};

        Key value;
        int NumberInArray;
        Node* brother_;
    };

    template <typename T>
    void swap(T& a, T& b){
        T tmp = a;
        a = b;
        b = tmp;
    }

    bool Update_Push(int a) {
        if (a == 0) {
            array_memory = 1;
            return true;
        } else if (a == 1) {
            array_memory *= 2;
            return true;
        } else if (a % 2 == 1) {
            return false;
        } else {
            return Update_Push(a / 2);
        }
    }

    void Remove_from_array() {
        if ((array_.size() - 1) <= array_memory / 4) {
            Vector<Node*> VectorFathers(array_.size() - 1, nullptr);
            for (int i = 0; i < array_.size() - 1; i++) {
                VectorFathers[i] = array_[i].brother_;
            }
            array_.Pop_Back();
            for (int i = 0; i < array_.size(); i++) {
                array_[i].brother_ = VectorFathers[i];
                if (VectorFathers[i] != nullptr) {
                    VectorFathers[i]->brother_ = &(array_[i]);
                }
            }
            array_memory /= 2;
        } else {
            array_.Pop_Back();
        }
    }

    void Push_in_array(Node& son) {
        if (Update_Push(array_.size())) {
            Vector<Node*> VectorFathers(array_.size(), nullptr);
            for (int i = 0; i < array_.size(); i++) {
                VectorFathers[i] = array_[i].brother_;
            }
            array_.Push_Back(son);
            for (int i = 0; i < array_.size() - 1; i++) {
                array_[i].brother_ = VectorFathers[i];
                if (VectorFathers[i] != nullptr) {
                    VectorFathers[i]->brother_ = &(array_[i]);
                }
            }
        } else {
            array_.Push_Back(son);
        }
    }

    void SwapArrayElements(Node& son1, Node& son2) {
        if (son1.brother_ == nullptr && son2.brother_ == nullptr) {
            swap(son1.value, son2.value);
        } else if (son2.brother_ == nullptr) {
            son2.brother_ = son1.brother_;
            son2.brother_->brother_ = &son2;
            son1.brother_ = nullptr;
            swap(son1.value, son2.value);
        } else if (son1.brother_ == nullptr) {
            son1.brother_ = son2.brother_;
            son1.brother_->brother_ = &son1;
            son2.brother_ = nullptr;
            swap(son1.value, son2.value);
        } else {
            Node* father1 = son1.brother_;
            Node* father2 = son2.brother_;
            father1->brother_ = &son2;
            father2->brother_ = &son1;
            son1.brother_ = father2;
            son2.brother_ = father1;
            swap(son1.value, son2.value);
        }
    }

    void SiftDown(int i) {
        while (2 * i + 1 < array_.size()){
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int j = left;
            if (right < array_.size() && array_[right].value < array_[left].value) {
                j = right;
            }
            if (array_[i].value <= array_[j].value) {
                break;
            }
            SwapArrayElements(array_[i], array_[j]);
            i = j;
        }
    }

    void SiftUp(int i) {
        if (i == array_.size()) {
            return ;
        }
        while (i != 0 && array_[i].value < array_[(i - 1) / 2].value) {
            SwapArrayElements(array_[i], array_[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }
};

#endif //LAB1_HEAP_H
