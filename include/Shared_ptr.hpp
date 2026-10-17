#ifndef SHARED_PTR
#define SHARED_PTR

#include "Unique_ptr.hpp" // Include UniquePtr definition
// When submitting to ACMOJ, please replace this with the content inside Unique_ptr.hpp
template <typename T>
struct ControlBlock{
  T* ptr = nullptr;
  int shared_cnt = 1;
  int weak_cnt = 0;
  void release_shared(){
    shared_cnt--;
    if(shared_cnt == 0){
      delete ptr;
      ptr = nullptr;
      release_weak();
    }
  }
  void release_weak(){
    //to do
  }
};
template <typename T>
class SharedPtr {
  private:
  T* ptr = nullptr;
  ControlBlock<T>* cb_ = nullptr;
    // Add whatever you want to add.
  public:
    // Constructors
    SharedPtr(){
      cb_{new ControlBlock<T>()};
    };                         // Default constructor
    explicit SharedPtr(T* p){
      if(p != nullptr) {
        cb_{new ControlBlock<T>()};
        cb_->ptr = p;
      }
      ptr = p;
    };                     // Constructor from raw pointer
    SharedPtr(const SharedPtr& tmp){
      ptr = tmp.ptr;
      cb_ = tmp.cb_;
      cb_->shared_cnt++;
    };     // Copy constructor
    SharedPtr(SharedPtr&& tmp) noexcept{
      ptr = tmp.ptr;
      tmp.ptr = nullptr;
      cb_ = tmp.cb_;
      tmp.cb_ = nullptr;
    };          // Move constructor
    // How about constructor from WeakPtr?

    // Destructor
    ~SharedPtr(){
      cb_->release_shared();
      cb_ = nullptr;
    };

    // Assignment operators
    SharedPtr& operator=(const SharedPtr& temp);     // Copy assignment operator
    SharedPtr& operator=(SharedPtr&& temp); // Move assignment operator

    // Basic operations
    int use_count();       // Get the number of shared owners
    T* get();              // Get the raw pointer
    operator bool();       // Check if the pointer is not null
    T& operator*();        // Dereference operator
    T* operator->();       // Arrow operator
    void reset();          // Reset to empty state
    void reset(T* p);      // Reset with a new raw pointer
};

// Then implement the make_shared function
// I think I'd better leave you to design the prototype :)

// template <?>
//? make_shared(?);

#endif // SHARED_PTR