#ifndef MONKEY_ARRAYLIST_HPP
#define MONKEY_ARRAYLIST_HPP

namespace monkey {
    template<typename T>
    class ArrayList {
        

        private:
            /* how memory allocation is managed
                memory gets managed like that:
                    1. 'strict', at small sizes, as the array grows it switches to 'buffer'
                    2. 'buffer', at medium sizes, as the array grows it switches to 'exponential'
                    3. 'exponential', at large sizes, since it can expect even more it is prepared
            */
            enum MemoryPolicy 
            {
                strict = 0, // capacity equals length (not recommended for objects with frequent push's or pop's, since it needs to reallocate the space every time) { capacity = capacity + amount_elements_pushed }
                buffer = 1, // a specified amount of space additionally allocated, the same amount gets removed if [(capacity - length) > buffer_size] (gets added again if full) { capacity = capacity + buffer }
                exponential = 2 // the additionally allocated memory is always as long as the current (gets added if full) --> { capacity = capacity * 2 }
            };

            int _buffer_size = 10; // the buffer size (for mem-policy 'buffer')
            ArrayList<T>::MemoryPolicy _mem_policy = ArrayList<T>::MemoryPolicy::strict; // Current configured memory-policy

            int _mem_policy_strict = 10; // until which length the memory policy is 'strict'
            unsigned int _mem_policy_buffer = this->_mem_policy_strict_cap + 30 // until which length the memory policy is 'buffer'

            T* _array;

            int _cap; // Array Capacity (How much space is allocated, "reserved in memory")
            int _len = 0; // length | (how many objects are currently stored)


            // Capacity Management ************************************************************************************************
            
            // sets the memory-policy based on capacity and length();
            ArrayList<T>& set_memory_policy()
            {

                if (this->_len < this->_mem_policy_strict) {
                    this->_mem_policy = MemoryPolicy::strict;
                }
                else if (this->_len < this->_mem_policy_buffer) {
                    this->_mem_policy = MemoryPolicy::buffer;
                }
                else {
                    this->_mem_policy = MemoryPolicy::exponential;
                }

                return *this;
            }


            // Allocates and sets a defined capacity
            ArrayList<T>& alloc(int cap) 
            {
                

                if (cap <= 0) {
                    this->_len = 0;
                    this->_cap = 1;
                }
                else {
                    if (this->_len > cap) {
                        this->_len = cap;
                    }

                    this->_cap = cap;
                }

                if (this->_len > 0) {
                    T* tmp = new T[this->_cap];

                    for (int i = 0; i < this->_len; ++i) {
                        tmp[i] = this->_array[i];
                    }

                    this->del();
                    this->_array = tmp;
                }
                else {
                    this->del();
                    this->_array = new T[this->_cap];
                }

                this->set_memory_policy();

                return *this;
            }

            // Allocates the current capacity `this->_cap`
            ArrayList<T>& alloc() 
            {
                this->alloc(this->_cap);
                return *this;
            }

            // Deletes array (but does not reallocate)
            ArrayList<T>& del() 
            {
                delete[] this->_array;

                return *this;
            }

            // Deallocates the entire array (delete, and realocates array with size 1)
            ArrayList<T>& dealloc() 
            {
                this->alloc(0);

                return *this;
            }

            // Deallocates n 
            ArrayList<T>& dealloc(int n) 
            {
                if (n < 0) {
                    throw "negative numbers not allowed for dealloc";
                }
                else if (n == 0) {
                    return *this;
                }

                if (n >= this->_cap) {
                    return this->alloc(0);
                }
                else {
                    return this->alloc((this->_cap - n))
                }

                return *this;
            }

            // used for at adding 1 element, Allocates more space according to memory-policy
            ArrayList<T>& grow() 
            {
                if (this->_len == this->_cap) {
                    if (this->_mem_policy == MemoryPolicy::strict) {
                        this->alloc(this->_cap + 1);
                    }
                    else if(this->_mem_policy == MemoryPolicy::buffer) {
                        this->alloc(this->_cap + this->_buffer_size);
                    }
                    else {
                        this->alloc(this->_cap * 2);
                    }
                }

                return *this;
            }

            // Deallocates N space
            ArrayList<T>& shrink() 
            {
                // todo check if lentgth is smaller then the resulting capacity

                if (this->_mem_policy == MemoryPolicy::strict) {
                    this->alloc(this->_cap - 1);
                }
                else if(this->_mem_policy == MemoryPolicy::buffer) {
                    this->alloc(this->_cap - this->_buffer_size);
                }
                else {
                    this->alloc(this->_cap / 2);
                }

                return *this;
            }

            // Optimizes the allocated space, to be just as much is needed by the current length
            ArrayList<T>& optimize() 
            {
                this->alloc(this->_len);

                return *this;
            }
            

            // Index Management ****************************************************************************************************************************

            // standardizes Indexes
            int _norm_idx(int I) {
                if (I < 0) {
                    I = this->_len - (0 - I);
                }

                return I;
            }

            // standardizes Indexes
            void _norm_idx(int* I) {
                if (*I < 0) {
                    *I = this->_len - (0 - *I);
                }
            }

            // norms and validates index
            int _idx(int I) {
                this->_norm_idx(&I);

                if (I < 0 || I  > int(this->_len - 1)) {
                    throw "Index out of bounds";
                }

                return I;
            }

            // norms and validates index
            void _idx(int* I) {
                this->_norm_idx(I);

                if (*I < 0 || *I  > int(this->_len - 1)) {
                    throw "Index out of bounds";
                }
            }

        public:
            

            ArrayList(int cap = 1)
            {
                if (n < 1) {
                    throw "negative or zero numbers not allowed for ArrayList constructor";
                }

                this->_len = 0;
                this->_cap = cap;

                this->alloc(1);
            }

            ~ArrayList()
            {
                this->del();
            }

            // Configures the capacity buffer (for memory-policy 'buffer')
            ArrayList<T>& set_buffer_size(int size) 
            {
                if (n < 1) {
                    throw "buffer_size must be at least 1";
                }

                this->_buffer_size = size;
                return *this;
            }

            // Gets the current Buffer size (for memory-policy 'buffer')
            unsigned int get_buffer_size()
            {
                return this->_buffer_size;
            }

            // returns current length
            int len() {
                return this->_len;
            }

            // returns current capacity
            int cap() {
                return this->_cap;
            }

            ArrayList<T>& clear() {
                return this->dealloc();
            }

            ArrayList<T>& push(T v) {
                if (this->_len == this->_cap) {

                }
            }
    };
};

#endif //MONKEY_ARRAYLIST_HPP
