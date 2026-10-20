#ifndef SHARED_PTR
#define SHARED_PTR

#include "Unique_ptr.hpp" // Include UniquePtr definition
// When submitting to ACMOJ, please replace this with the content inside Unique_ptr.hpp
template <typename T>
struct ControlBlock{
  T* ptr = nullptr;
  int shared_cnt = 1;
  int weak_cnt = 1;
  void release_shared(){
    shared_cnt--;
    if(shared_cnt == 0){
      delete ptr;
      ptr = nullptr;
      release_weak();
    }
  }
  void release_weak(){
    weak_cnt--;
    if(weak_cnt == 0||shared_cnt == 0){
      delete this;
    }
  }
};
template <typename T>
class SharedPtr {
  template <typename U>
  friend class WeakPtr;
  private:
  T* ptr = nullptr;
  ControlBlock<T>* cb_ = nullptr;
    // Add whatever you want to add.
  public:
    // Constructors
    SharedPtr() = default;                // Default constructor
    explicit SharedPtr(T* p){
      if(p != nullptr) {
        cb_ = new ControlBlock<T>();
        cb_->ptr = p;
      }
      ptr = p;
    };                     // Constructor from raw pointer
    SharedPtr(const SharedPtr& tmp){
      ptr = tmp.ptr;
      cb_ = tmp.cb_;
      if(cb_) cb_->shared_cnt++;
    };     // Copy constructor
    SharedPtr(SharedPtr&& tmp) noexcept{
      ptr = tmp.ptr;
      tmp.ptr = nullptr;
      cb_ = tmp.cb_;
      tmp.cb_ = nullptr;
    };          // Move constructor
    SharedPtr(UniquePtr<T>& tmp) : SharedPtr(tmp.release()){};
    SharedPtr(UniquePtr<T>&& tmp) : SharedPtr(tmp.release()){};
    // How about constructor from WeakPtr?
    // Destructor
    ~SharedPtr(){
      if(cb_){
        cb_->release_shared();
        cb_ = nullptr;
      }
    };

    // Assignment operators
    SharedPtr& operator=(const SharedPtr& temp){
      if(ptr == temp.ptr){
        return *this;
      }
      else{
        if(cb_) cb_->release_shared();
        ptr = temp.ptr;
        cb_ = temp.cb_;
        if(cb_) cb_->shared_cnt++;
        return *this;
      }
    };     // Copy assignment operator
    SharedPtr& operator=(SharedPtr&& temp) noexcept{
      if(this == &temp){
        return *this;
      }
      if(cb_) cb_->release_shared();
      ptr = temp.ptr;
      temp.ptr = nullptr;
      cb_ = temp.cb_;
      temp.cb_ = nullptr;
      return *this;
    }; // Move assignment operator

    // Basic operations
    int use_count() const{
      if(cb_) return cb_->shared_cnt;
      return 0;
    };       // Get the number of shared owners
    T* get() const{
      return ptr;
    };              // Get the raw pointer
    explicit operator bool() const{
      return ptr != nullptr;
    };       // Check if the pointer is not null
    T& operator*() const{
      return *ptr;
    };        // Dereference operator
    T* operator->() const{
      return ptr;
    };       // Arrow operator
    void reset(){

      ptr = nullptr;
      if(cb_) cb_->release_shared();
      cb_ = nullptr;
    };          // Reset to empty state
    void reset(T* p){
      if(cb_ && ptr == p) return;
      *this = std::move(SharedPtr(p));
    };      // Reset with a new raw pointer
};

// Then implement the make_shared function
// I think I'd better leave you to design the prototype :)

// template <?>
//? make_shared(?);
template<typename T,typename ...Args>
SharedPtr<T> make_shared(Args&&...args){
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

#endif // SHARED_PTR