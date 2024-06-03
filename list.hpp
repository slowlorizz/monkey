#ifndef MONKEY_LIST_HPP
#define MONKEY_LIST_HPP

namespace monkey {
    template<typename T>
    class List {
        /*
        Dynamic Container
        --> is a double linked list in background
        */

        struct Item {
            T value;
            monkey::List<T>::Item* prev = nullptr;
            monkey::List<T>::Item* next = nullptr;

            Item(T v, monkey::List<T>::Item* p = nullptr, monkey::List<T>::Item* n = nullptr) {
                this->value = v;
                this->prev = p;
                this->next = n;
            }

            void destroy_chain() {
                if (this->next != nullptr) {
                    this->next->destroy_chain();
                    delete this->next;
                }
            }

            void operator=(T v) {
                this->value = v;
            }
        };

        

        private:
            monkey::List<T>::Item* _root = nullptr;
            int _len = 0;

            void _destroy() {
                if(this->_root != nullptr) {
                    this->_root->destroy_chain();
                    delete this->_root;
                }
            }

            void _idx(int* I) {
                if (*I < 0) {
                    *I = int(this->_len - int(0 - *I));
                }

                if(*I < 0 || *I > int(this->_len - 1)) {
                    throw "index out of bounds!";
                }
            }

            List<T>::Item* _get_tail() {
                if (this->_root == nullptr) {
                    return nullptr;
                }

                List<T>::Item* _tail = this->_root;

                while (_tail->next != nullptr) {
                    _tail = _tail->next;
                }

                return _tail;
            }

            List<T>::Item* _at(int I) {
                if (this->_root == nullptr) {
                    return nullptr;
                }

                this->_idx(&I);

                List<T>::Item* itm = this->_root;

                for (int i = 1; i <= I; ++i) {
                    itm = itm->next;
                }

                return itm;
            }

        public:
            class Iterator {
                private:
                    List<T>::Item* itm;

                    void _set_value() {
                        if (this->itm == nullptr) {
                            this->v = nullptr;
                        }
                        else {
                            this->v = &this->itm->value;
                        }
                    }
                
                public:
                    T* v;

                    Iterator(List<T>* lst) {
                        this->itm = lst->itm(0);
                        this->_set_value();
                    }

                    Iterator(int I, List<T>* lst) {
                        this->itm = lst->itm(I);
                        this->_set_value();
                    }

                    Iterator(List<T>::Item* itm) {
                        this->itm = itm;
                        this->_set_value();
                    }

                    void next(int n = 1) {
                        for (int i = 0; i < n && this->itm != nullptr; ++i) {
                            this->itm = this->itm->next;
                        }

                        this->_set_value();
                    }

                    void prev(int n = 1) {
                        for (int i = 0; i < n && this->itm != nullptr; ++i) {
                            this->itm = this->itm->prev;
                        }

                        this->_set_value();
                    }

                    bool is_lost() {
                        return bool(this->itm == nullptr);
                    }

                    bool at_start() {
                        return bool(this->itm->prev == nullptr);
                    }

                    bool at_end() {
                        return bool(this->itm->next == nullptr);
                    }

                    void operator++() {
                        this->next(1);
                    }

                    void operator--() {
                        this->prev(1);
                    }

                    void operator+=(int n) {
                        this->next(n);
                    }

                    void operator-=(int n) {
                        this->prev(n);
                    }
            };

            List() {
                this->_root = nullptr;
                this->_len = 0;
            }

            List(int len, T* arr) {
                this->_root = nullptr;
                this->_len = 0;

                this->push(len, arr);
            }

            ~List() {
                this->_destroy();
            }

            T& operator[](int I) {
                this->_idx(&I);

                T& v = this->_at(I)->value;

                return v;
            }

            List<T>& operator<<(T v) {
                this->push(v);
                return *this;
            }

            List<T>& operator<<(List<T>& l) {
                this->push(l);
                return *this;
            }

            int len() {
                return this->_len;
            }

            List<T>::Item* itm(int I) {
                // get item pointer

                this->_idx(&I);

                return this->_at(I);
            }

            List<T>::Iterator itr(int I = 0) {
                // get iterator
                this->_idx(&I);

                List<T>::Iterator it = List<T>::Iterator(this->_at(I));

                return it;
            }

            List<T>& clear() {
                this->_destroy();

                return *this;
            }

            T get(int I) {
                this->_idx(&I);

                return this->_at(I)->value;
            }

            List<T>& set(int I, T v) {
                this->_idx(&I);

                this->_at(I)->value = v;

                return *this;
            }

            List<T>& push(T v) {
                if (this->_root == nullptr) {
                    this->_root = new List<T>::Item(v);
                }
                else {
                    List<T>::Item* _tail = this->_get_tail();
                    _tail->next = new List<T>::Item(v, _tail);
                }

                this->_len += 1;

                return *this;
            }

            List<T>& push(int len, T* arr) {
                // pushing an array

                for (int i = 0; i < len; ++i) {
                    this->push(arr[i]);
                }

                return *this;
            }

            List<T>& push(List<T>& l) {
                // pushing a list

                for (int i = 0; i < l.len(); ++i) {
                    this->push(l.get(i));
                }

                return *this;
            }

            List<T>& push_front(T v) {
                if (this->_root == nullptr) {
                    this->_root = new List<T>::Item(v);
                }
                else {
                    this->_root->prev = new List<T>::Item(v, nullptr, this->_root);
                    this->_root = this->_root->prev;
                }

                this->_len += 1;

                return *this;
            }

            List<T>& insert(int I, T v) {
                if (I == this->_len) {
                    return this->push(v);
                }
                else if(I == 0){
                    return this->push_front(v);
                }

                this->_idx(&I);

                List<T>::Item* itm = this->_at(I);

                itm->prev->next = new List<T>::Item(v, itm->prev, itm);
                itm->prev = itm->prev->next;

                this->_len += 1;

                return *this;
            }

            T pop(int I = -1) {
                this->_idx(&I);

                List<T>::Item* itm = this->_at(I);

                T v = itm->value;

                if (itm->prev != nullptr) {
                    itm->prev->next = itm->next;
                }

                if (itm->next != nullptr) {
                    itm->next->prev = itm->prev;
                }

                delete itm;

                this->_len -= 1;

                return v;
            }

            List<T>& foreach(void (*func)(List<T>::Iterator*)) {
                for (List<T>::Iterator it = this->itr(0); !it.is_lost(); ++it) {
                    func(&it);
                }

                return *this;
            }

            List<T>& map(T (*func)(List<T>::Iterator*)) {
                for (List<T>::Iterator it = this->itr(0); !it.is_lost(); ++it) {
                    *it.v = func(&it);
                }

                return *this;
            }
    };
}

#endif //MONKEY_LIST_HPP