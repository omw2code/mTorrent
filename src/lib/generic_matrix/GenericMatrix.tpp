template<typename T>
GenericMatrix<T>::GenericMatrix()
    : rows_(0)
    , cols_(0)
    , matrix_{}
{}

template<typename T>
GenericMatrix<T>::GenericMatrix(const GenericMatrix &other)
    : rows_(matrix_.rows_)
    , cols_(matrix_.cols_)
    , matrix_(other.matrix_)
{}

template<typename T>
GenericMatrix<T>::GenericMatrix(GenericMatrix &&other) noexcept
    : rows_(std::exchange(other.rows_, 0))
    , cols_(std::exchange(other.cols_, 0))
    , matrix_(std::exchange(other.matrix_, {}))
{}

template<typename T> 
GenericMatrix<T>&& GenericMatrix<T>::operator=(GenericMatrix<T> &&other) noexcept
{
    return std::move(other);
}

template<typename T>
GenericMatrix<T>::GenericMatrix(const std::initializer_list<std::initializer_list<T>> &list)
  : rows_{0}
  , cols_{0}
  , matrix_{}
{
    if (list.size() <= 0)
        return;

    // Check to make sure cols and rows are equal
    rows_ = list.size();
    cols_ = rows_ > 0 ? list.begin()->size() : 0;
    for (const auto &inner : list)
    {
        if (inner.size() != cols_)
            throw std::runtime_error("Unequal row and column size");
    }
    
    // copy over the data
    matrix_.resize(rows_, std::vector<T>(cols_, 0));
    int i{};
    for (const auto &inner : list)
    {
        std::copy(inner.begin(), inner.end(), matrix_[i].begin());
        ++i;
    }

}

template<typename T>
GenericMatrix<T>::~GenericMatrix()
{}

// TODO fix for [][] 
template<typename T>
T GenericMatrix<T>::operator[](int indx)
{
    return matrix_.at(indx);
}

template<typename T>
GenericMatrix<T> GenericMatrix<T>::operator+(GenericMatrix<T> &rhs)
{
    // Ensure this and T are the same type
    if constexpr ( !std::is_same<decltype(this), decltype(rhs)>::type )
        throw std::invalid_argument("Invalid binary operand using the + operator");
    
    int r_rows = rhs.size();
    if constexpr ( r_rows != rows_ )
        throw std::length_error("Invalid operand row size using + operator");

    int r_cols = rhs[0].size();
    if constexpr ( r_cols != cols_ )
        throw std::length_error("Invalid operand col size using the + operator");

    GenericMatrix<T> matrix;
    for (int i{}; i < rows_; ++i)
    {
        for (int j{}; j < cols_; ++j)
        {
            this[i][j] = this[i][j] + rhs[i][j];
        }
    }
    return matrix;
}




template<typename T>
 GenericMatrix<T> GenericMatrix<T>::operator*(GenericMatrix<T> &rhs)
{
    // Start matrix multiplication
    GenericMatrix<T> matrix;
    for (int i{}; i < rows_; ++i)
    {
        for(int j{}; j < cols_; ++j)
        {
            for(int k{}; k < rhs.size(); ++k)
            {
                matrix[i][j] = this[i][j] * rhs[i][j];
            }
        }
    }
    return matrix;
}


template<typename T>
bool GenericMatrix<T>::operator==(GenericMatrix<T> &other)
{
    if (equalLength(other))
    {
        for (int i{}; i < rows_; ++i)
        {
            for (int j{}; j < cols_; ++j)
            {
                if (this[i][j] != other[i][j])
                    return false;
            }
        }
    }
    return true;
}

template<typename T>
bool GenericMatrix<T>::operator!=(GenericMatrix<T> &other)
{
   if (equalLength(other))
   {
       for (int i{}; i < rows_; ++i)
       {
           for (int j{}; j < cols_; ++j)
           {
               if (this[i][j] != other[i][j])
                   return true;
           }
       }
   }
   return false;
}

template<typename T>
bool GenericMatrix<T>::operator>(GenericMatrix<T> &other)
{
    if (equalLength(other))
    {
        for (int i{}; i < rows_; ++i)
            for(int j{}; j < cols_; ++j)
                if ( !(this[i][j] > other[i][j]) )
                    return false;
    }
    return true;
}

template<typename T>
bool GenericMatrix<T>::operator<(GenericMatrix<T> &other)
{
    if (equalLength(other))
    {
        for (int i{}; i < rows_; ++i)
            for (int j{}; j <cols_; ++j)
                if ( !(this[i][j] < other[i][j]) )
                    return false;
    }
    return true;
}

template<typename T>
bool GenericMatrix<T>::operator>=(GenericMatrix<T> &other)
{
    if (equalLength(other))
    {
        for (int i{}; i < rows_; ++i)
            for (int j{}; j < cols_; ++j)
                if ( !(this[i][j] >= other[i][j]) )
                    return false;
    }
    return true;
}

template<typename T>
bool GenericMatrix<T>::operator<=(GenericMatrix &other)
{
    if (equalLength())
    {
        for (int i{}; i < rows_; ++i)
            for (int j{}; j < cols_; ++j)
                if ( !(this[i][j] <= other[i][j]) )
                    return false;
    }
    return true;
}

template<typename T>
bool GenericMatrix<T>::empty()
{
    return matrix_.empty();
}

template<typename T>
size_t GenericMatrix<T>::getRows()
{
    return rows_;
}

template<typename T>
size_t GenericMatrix<T>::getCols()
{
    return cols_;
}

template<typename T>
template<typename U>
bool GenericMatrix<T>::equalLength(GenericMatrix<U> &other)
{
    if (matrix_.size() != other.size())
        throw std::runtime_error("Other row size is not equal to this row size");

    for(const auto &col : other)
    {
        if (col.size() != cols_)
            throw std::runtime_error("Other col size is not equal ot this col size");
    }
    return true;
}

