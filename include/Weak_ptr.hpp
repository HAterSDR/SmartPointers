#ifndef WEAK_PTR
#define WEAK_PTR

#include "Shared_ptr.hpp"
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
