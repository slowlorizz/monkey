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
            bool _dynamic_mem_policy = true; // if the memory-policy is set dynamicly (default: true)

            int _mem_policy_strict = 10; // until which length the memory policy is 'strict'
            int _mem_policy_buffer = this->_mem_policy_strict + 30; // until which length the memory policy is 'buffer'

            T* _array;

            int _cap; // Array Capacity (How much space is allocated, "reserved in memory")
            int _len = 0; // length | (how many objects are currently stored)


            // Capacity Management ************************************************************************************************
            
            // sets the memory-policy based on capacity and length();
            ArrayList<T>& set_memory_policy()
            {
                if(this->_dynamic_mem_policy){
                    if (this->_len < this->_mem_policy_strict) {
                        this->_mem_policy = MemoryPolicy::strict;
                    }
                    else if (this->_len < this->_mem_policy_buffer) {
                        this->_mem_policy = MemoryPolicy::buffer;
                    }
                    else {
                        this->_mem_policy = MemoryPolicy::exponential;
                    }
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
                    return this->alloc((this->_cap - n));
                }

                return *this;
            }

            // used for adding 1 element, Allocates more space according to memory-policy
            ArrayList<T>& grow(int n = 1) 
            {
                if (n < 0) {
                    throw "Negative growth!!";
                }
                else if (n > 0) {
                    if (int(this->_len + n) > this->_cap) {
                        if (this->_mem_policy == MemoryPolicy::strict ) {
                            this->alloc(this->_cap + n);
                        }
                        else if(this->_mem_policy == MemoryPolicy::buffer) {
                            this->alloc(int(this->_buffer_size * int(int(int(int(this->_len + n) - int(int(this->_len + n) % this->_buffer_size)) / this->_buffer_size) + 1)));
                        }
                        else if(this->_mem_policy == MemoryPolicy::exponential) {
                            int c = this->_cap;

                            while(c < int(this->_len + n)) {
                                c *= 2;
                            }

                            this->alloc(c);
                        }
                    }
                }

                return *this;
            }

            // Deallocates N space
            ArrayList<T>& shrink(int n = 1) 
            {
                // todo check if lentgth is smaller then the resulting capacity

                if (n < 0) {
                    throw "Negative shrink!!";
                }
                else if(n == this->_cap) {
                    this->dealloc();
                }

                if (this->_mem_policy == MemoryPolicy::strict) {
                    this->alloc(this->_cap - n);
                }
                else if(this->_mem_policy == MemoryPolicy::buffer && int(this->_cap - int(this->_len - n)) >= this->_buffer_size) {
                    this->alloc(int(this->_buffer_size * int(int(int(int(this->_len - n) - int(int(this->_len - n) % this->_buffer_size)) / this->_buffer_size) + 1)));
                }
                else {
                    int c = this->_cap;

                    while (int(c / 2) > int(this->_len - n)) {
                        c /= 2;
                    }

                    this->alloc(c);
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
            

            ArrayList()
            {
                this->_len = 0;
                this->_cap = 1;

                this->alloc(1);
            }

            ArrayList(int cap)
            {
                if (cap < 1) {
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

            T& operator[](int I) {
                this->_idx(&I);
                return this->_array[I];
            }

            ArrayList<T>& operator<<(T v) {
                return this->push(v);
            }

            ArrayList<T>& operator<<(ArrayList<T>& li) {
                return this->push(li);
            }

            // Configures the capacity buffer (for memory-policy 'buffer')
            ArrayList<T>& set_buffer_size(int size) 
            {
                if (size < 1) {
                    throw "buffer_size must be at least 1";
                }

                this->_buffer_size = size;
                return *this;
            }

            // Gets the current Buffer size (for memory-policy 'buffer')
            int get_buffer_size()
            {
                return this->_buffer_size;
            }

            // Sets memory Policy to specified Policy, and stays at that policy (disables dynamic memory policy switching)
            ArrayList<T>& set_mem_policy(ArrayList<T>::MemoryPolicy mp) 
            {
                this->_mem_policy = mp;
                this->_dynamic_mem_policy = false;

                return *this;
            }

            // Sets memory-policy management to dynamic (mostly used when set to specific memory policy to reactivate the dynamic mode)
            ArrayList<T>& enable_dynamic_mem_policy() 
            {
                this->_dynamic_mem_policy = true;
                this->set_mem_policy();

                return *this;
            }

            // returns current length
            int len() 
            {
                return this->_len;
            }

            // returns current capacity
            int cap() 
            {
                return this->_cap;
            }

            ArrayList<T>& clear() 
            {
                return this->dealloc();
            }

            // Adds one Element to the back of the array
            ArrayList<T>& push(T v) 
            {
                this->grow();
                this->_array[int(this->_len)] = v;
                this->_len += 1;

                return *this;
            }

            // Appends another ArrayList at the end of this ArrayList (makes a copy!)
            ArrayList<T>& push(ArrayList<T>& li) 
            {
                this->grow(li.len());

                for(int i = this->_len; i < int(this->_len + li.len()); ++i)
                {
                    this->_array[i] = li[int(i - this->_len)];
                }

                this->_len += li.len();

                return *this;
            }

            // Adds one Element at a specified index (pushes all later elements one index back)
            ArrayList<T>& insert(T v, int I)
            {
                if(I == this->_len) {
                    this->push(v);
                }

                this->_idx(&I);

                this->grow();

                for(int i = int(this->_len - 1); i >= I; --i) {
                    this->_array[i + 1] = this->_array[i];
                }

                this->_array[I] = v;
                this->_len += 1;

                return *this;
            }

            T pop(int I = -1) 
            {
                this->_idx(&I);

                T v = this->_array[I];
                this->shrink(1);
                this->_len -= 1;

                return v;
            }
    };
};

#endif //MONKEY_ARRAYLIST_HPP
