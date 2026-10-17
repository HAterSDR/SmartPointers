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
