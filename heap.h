// Copyright[2017] <Burlaciuc Mihai>
#ifndef _HEAP_H_
#define _HEAP_H_
#include <vector>
#include <iostream>
#include <algorithm>
template <typename T>
class Heap {
    std::vector<T> elements;
 public:


    int get_parent(int pos) {
        return (pos - 1) / 2;
    }

    int get_left_subtree(int pos) {
        return 2 * pos + 1;
    }

    int get_right_subtree(int pos) {
        return 2 * pos + 2;
    }

    void pushUp(int pos) {
        int parent = this->get_parent(pos);
        T aux;
        while (this->elements[parent] < this->elements[pos]) {
            if (pos > 0) {
                // Se face interschimbarea
                aux = this->elements[parent];
                this->elements[parent] = this->elements[pos];
                this->elements[pos] = aux;

                pos = parent;
                parent = this->get_parent(pos);
            }
        }

    }

    void pushDown(int pos) {
        T aux;
        while (1) {
            unsigned int leftPos = this->get_left_subtree(pos);
            unsigned int rightPos = this->get_right_subtree(pos);
            if (rightPos > elements.size() - 1) {
                if (leftPos > elements.size() - 1) {
                    break;
                } else if(this->elements[leftPos] > this->elements[pos]) {
                    aux = this->elements[leftPos];
                    this->elements[leftPos] = this->elements[pos];
                    this->elements[pos] = aux;

                    pos = leftPos;
                } else {
                    break;
                }
            } else {
                if (this->elements[leftPos] >= this->elements[rightPos] &&
                this->elements[leftPos] > this->elements[pos]) {
                    aux = this->elements[leftPos];
                    this->elements[leftPos] = this->elements[pos];
                    this->elements[pos] = aux;

                    pos = leftPos;
                } else if (this->elements[rightPos] > this->elements[leftPos]
                && this->elements[rightPos] > this->elements[pos]) {
                    aux = this->elements[rightPos];
                    this->elements[rightPos] = this->elements[pos];
                    this->elements[pos] = aux;

                    pos = rightPos;
                } else {
                    break;
                }
            }
        }
    }

    void insert(T value) {
        elements.push_back(value);
        pushUp(elements.size() - 1);
    }

    void replace(T oldvalue, T value) {
        typename std::vector<T>::iterator it;
        it = find(elements.begin(), elements.end(), oldvalue);
        elements.erase(it);
        elements.push_back(value);
        pushUp(elements.size() - 1);
    }

    T peek() {
        if (elements.size()) {
            return elements[0];
        } else {
            return T();
        }
    }

    bool isEmpty() {
        if (elements.size() == 0) {
            return true;
        }
        return false;
    }
    T extractMax() {
        if (elements.size() == 0) {
            T x;
            return x;
        }
        T maxValue = elements[0];
        elements[0] = elements[elements.size() - 1];
        elements.pop_back();
        if (elements.size() > 0)
            pushDown(0);
        return maxValue;
    }


    void print() {
        for (unsigned int i = 0; i < elements.size(); i++) {
            this->elements[i].a();
            std::cout<< '\n';
        }
    }
};
#endif //  _HEAP_H_
