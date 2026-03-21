#ifndef OVERLOADED_OPPS_HPP
#define OVERLOADED_OPPS_HPP

#include <vector>
#include <type_traits>

// This entity represents a 2D vector with declared 
// and defined operators
template<typename T>
class GenericMatrix
{
public:
    /*
    * Default constructor
    */
    GenericMatrix();

    /*
    * Default destructor
    */
    ~GenericMatrix();

    /*
    * Copy Constructor
    */
    GenericMatrix(const GenericMatrix<T>& other);

    /*
    * Move Constructor
    */
    GenericMatrix(GenericMatrix<T> &&matrix) noexcept;

    /*
    * Move assignment 
    */
    GenericMatrix<T>&& operator=(GenericMatrix<T> &&other) noexcept;

    /*
    * Assignment operator
    */
    GenericMatrix<T> operator=(GenericMatrix<T> &other);

    /*
    * Initializer List Constructor
    */
    GenericMatrix(const std::initializer_list<std::initializer_list<T>> &list);
    
    size_t getRows();

    size_t getCols();

    bool empty();
/// Accessor operators
public:
    T operator[](int i);

/// Arithmetic operators
public:
    /*
    * Addition operator 
    */
    GenericMatrix<T> operator+ (GenericMatrix<T> &other);

    /*
    * Subtraction operator
    */
    GenericMatrix<T> operator- (GenericMatrix<T> &other);

    /*
    * Multiplication operator
    */
    GenericMatrix<T> operator* (GenericMatrix<T> &other);
    
    /*
    * Division operator 
    */
    GenericMatrix<T> operator/ (GenericMatrix<T> &other);

    /*
    * Addition assignemnt operator
    */
    void operator+= (GenericMatrix<T> &other);

    /*
    * Minus assignment operator
    */
    void operator-= (GenericMatrix<T> &other);

    /*
    * Multiplication assignemnt operator
    */
    void operator *= (GenericMatrix<T> &other);

    /*
    * Division assignment operator
    */
    void operator /= (GenericMatrix<T> &other);

    void operator++ (int) = delete;
    void operator-- (int) = delete;

/// Comparison operators
public:

    /*
    * Equality operator
    */
    bool operator== (GenericMatrix<T> &other); 

    /*
    * Not equal operator
    */
    bool operator!= (GenericMatrix<T> &other);
    /*
    * Less than operator
    */
    bool operator< (GenericMatrix<T> &other);

    /*
    * Greater than operator
    */
    bool operator> (GenericMatrix<T> &other);

    /*
    * Less than equal to operator
    */
    bool operator<= (GenericMatrix<T> &other);

    /*
    * Greater than equal to operator
    */
    bool operator>= (GenericMatrix<T> &other);

private:
    template<typename U>
    bool equalLength(GenericMatrix<U> &other);

private:

    /// Two dementional vector
    std::vector<std::vector<T>> matrix_;
    
    /// Matrix attributes
    size_t rows_;
    size_t cols_;
};
#include "GenericMatrix.tpp" /// Implementation

#endif
