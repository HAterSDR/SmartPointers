#ifndef WEAK_PTR
#define WEAK_PTR

#ifndef SHARED_PTR
#define SHARED_PTR

#ifndef UNIQUE_PTR
#define UNIQUE_PTR

#include <utility>

template <typename T>

class UniquePtr {
  private:
    // Add whatever you want to add.
    T* ptr = nullptr;
  public:
    // Constructors
    UniquePtr() = default;                          // Default constructor
    explicit UniquePtr(T* p){
      ptr = p;
    };             // Constructor from raw pointer
    UniquePtr(UniquePtr& other) = delete; // Copy constructor is deleted
    UniquePtr(UniquePtr&& other) noexcept{
      ptr = other.ptr;
      other.ptr = nullptr;
    };         // Move constructor

    // Destructor
    ~UniquePtr(){
      delete ptr;
      ptr = nullptr;
    };

    // Assignment operators
    UniquePtr& operator=(UniquePtr& other) = delete; // Copy assignment operator is deleted
    UniquePtr& operator=(UniquePtr&& other) noexcept{
      if(this != &other){
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
      }
      return *this;
      
    }; // Move assignment operator

    // Basic operations
    T* get() const{
      return ptr;
    };                 // Get the raw pointer
    explicit operator bool() const{
      if(ptr == nullptr){
        return false;
      }
      else return true;
    }; // Check if the pointer is not null
    T& operator*() const{
      return *ptr;
    };           // Dereference operator
    T* operator->() const{
      return ptr;
    };          // Arrow operator
    void reset(){
      delete ptr;
      ptr = nullptr;
    };                   // Reset to empty state
    void reset(T* p){
      if(ptr == p) return;
      delete ptr;
      ptr = p;
    };               // Reset with a new raw pointer
    T* release(){
      T* now_ptr = ptr;
      ptr = nullptr;
      return now_ptr;
    };                   // Release ownership of the managed object
};
template<typename T, typename...Args>
UniquePtr<T> make_unique(Args&&...args){
  return UniquePtr<T>(new T(std::forward<Args>(args)...));
}
// make_unique function

#endif // UNIQUE_PTR

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
    if(weak_cnt == 0){
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

template <typename T>
class WeakPtr {
  private:
    ControlBlock<T>* cb_ = nullptr;
    // Add whatever you want to add.
  public:
    // Constructors
    WeakPtr() = default;                         // Default constructor
    WeakPtr(const SharedPtr<T>& temp){
      cb_ = temp.cb_;
      if(cb_) cb_->weak_cnt++;
    }; // Constructor from SharedPtr
    WeakPtr(const WeakPtr& temp){
      cb_ = temp.cb_;
      if(cb_) cb_->weak_cnt++;

    };      // Copy constructor
    WeakPtr(WeakPtr&& temp) noexcept{
      cb_ = temp.cb_;
      temp.cb_ = nullptr;
    };           // Move constructor

    // Destructor
    ~WeakPtr(){
      if(cb_){
        cb_->release_weak();
        cb_ = nullptr;
      }
    };

    // Assignment operators
    WeakPtr& operator=(const WeakPtr& temp){
      if(temp.cb_ == cb_){
        return *this;
      }
      if(cb_) cb_->release_weak();
      cb_ = temp.cb_;
      if(cb_) cb_->weak_cnt++;
      return *this;
    };      // Copy assignment operator
    WeakPtr& operator=(const SharedPtr<T>& temp){
      if(temp.cb_ == cb_){
        return *this;
      }
      if(cb_) cb_->release_weak();
      cb_ = temp.cb_;
      if(cb_) cb_->weak_cnt++;
      return *this;
    }; // Assignment from SharedPtr
    WeakPtr& operator=(WeakPtr&& temp) noexcept{
      if(this == &temp){
        return *this;
      }
      if(cb_) cb_->release_weak();
      cb_ = temp.cb_;
      temp.cb_ = nullptr;
      return *this;
    };           // Move assignment operator
    explicit operator bool() const{
      return !expired();
    }
    // Basic operations
    int use_count() const{
      if(cb_) return cb_->shared_cnt;
      return 0;
    };     // Get the number of shared owners
    SharedPtr<T> lock() const{
      if(expired()){
        return SharedPtr<T>();
      }
      SharedPtr<T> shared_ptr;
      shared_ptr.cb_ = cb_;
      shared_ptr.ptr = cb_->ptr;
      cb_->shared_cnt++;
      return shared_ptr;
    }; // Get a SharedPtr to the managed object
    bool expired() const{
      if(cb_ == nullptr) return true;
      return cb_->ptr == nullptr; 
    };      // Check if the managed object was deleted
    void reset(){
      if(cb_) cb_->release_weak();
      cb_ = nullptr;
    };        // Reset to empty state

    // Utility functions
    void swap(WeakPtr& other){
      ControlBlock<T>* tmp = other.cb_;
      other.cb_ = cb_;
      cb_ = tmp;
    }; // Swap with another WeakPtr

};

// Non-member swap function
template <typename T>
void swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs){
  lhs.swap(rhs);
};

#endif // WEAK_PTR
