#ifndef SHARED_PTR
#define SHARED_PTR

#include "Unique_ptr.hpp" // Include UniquePtr definition
// When submitting to ACMOJ, please replace this with the content inside Unique_ptr.hpp

template <typename T>
class SharedPtr {
  private:
    // Add whatever you want to add.
  public:
    // Constructors
    SharedPtr();                         // Default constructor
    SharedPtr(T* p);                     // Constructor from raw pointer
    SharedPtr(const SharedPtr& tmp);     // Copy constructor
    SharedPtr(SharedPtr&& tmp);          // Move constructor
    // How about constructor from WeakPtr?

    // Destructor
    ~SharedPtr();

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