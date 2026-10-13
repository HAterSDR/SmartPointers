#ifndef WEAK_PTR
#define WEAK_PTR

#include "Shared_ptr.hpp" // Include SharedPtr definition
// When submitting to ACMOJ, please replace this with the content inside Shared_ptr.hpp

template <typename T>
class WeakPtr {
  private:
    // Add whatever you want to add.
  public:
    // Constructors
    WeakPtr();                         // Default constructor
    WeakPtr(const SharedPtr<T>& temp); // Constructor from SharedPtr
    WeakPtr(const WeakPtr& temp);      // Copy constructor
    WeakPtr(WeakPtr&& temp);           // Move constructor

    // Destructor
    ~WeakPtr();

    // Assignment operators
    WeakPtr& operator=(const WeakPtr& temp);      // Copy assignment operator
    WeakPtr& operator=(const SharedPtr<T>& temp); // Assignment from SharedPtr
    WeakPtr& operator=(WeakPtr&& temp);           // Move assignment operator

    // Basic operations
    int use_count();     // Get the number of shared owners
    SharedPtr<T> lock(); // Get a SharedPtr to the managed object
    bool expired();      // Check if the managed object was deleted
    void reset();        // Reset to empty state

    // Utility functions
    void swap(WeakPtr& other); // Swap with another WeakPtr
};

// Non-member swap function
template <typename T>
void swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs);

#endif // WEAK_PTR
