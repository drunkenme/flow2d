// @date 2016/05/24
// @author Mao Jingkai(oammix@gmail.com)

// INCLUDED METHODS OF MATRIX

template<size_t R, size_t C, typename T>
Matrix<R, C, T>::Matrix() {}

template<size_t R, size_t C, typename T>
Matrix<R, C, T>::Matrix(std::initializer_list<T> values)
{
    auto size = values.size();
    auto cursor = values.begin();

    size_t r, c, i;
    for( r = 0, i = 0; r < R; r++ )
    {
        for( c = 0; c < C; c++, i++ )
        {
            if( i < size ) m_values[r][c] = *iter++;
            else break;
        }

        if( c < C )
        {
            for( ; c < C; c++ )
                m_values[r][c] = (T)0;
            r++;
            break;
        }
    }

    if( r < R )
    {
        for( ; r < R; r++ )
            for( c = 0; c < C; c++ )
                m_values[r][c] = (T)0;
    }
}

template<size_t R, size_t C, typename T>
INLINE const T& Matrix<R, C, T>::operator() (size_t r, size_t c) const
{
    return m_values[r][c];
}

template<size_t R, size_t C, typename T>
INLINE T& Matrix<R, C, T>::operator() (size_t r, size_t c)
{
    return m_values[r][c];
}

template<size_t R, size_t C, typename T>
INLINE const T& Matrix<R, C, T>::operator[] (size_t index) const
{
    const T* elements = &m_values;
    return elements[index];
}

template<size_t R, size_t C, typename T>
INLINE T& Matrix<R, C, T>::operator[] (size_t index)
{
    T* elements = &m_values;
    return elements[index];
}

template<size_t R, size_t C, typename T>
INLINE bool Matrix<R, C, T>::operator == (const Matrix<R, C, T>& rh) const
{
    return m_values == rh.m_values;
}

template<size_t R, size_t C, typename T>
INLINE bool Matrix<R, C, T>::operator != (const Matrix<R, C, T>& rh) const
{
    return m_values != rh.m_values;
}

template<size_t R, size_t C, typename T>
void Matrix<R, C, T>::zero() const
{
    auto zero = (T)0;
    for( auto i=0; i<R; i++ )
        for( auto j=0; j<C; j++ )
            m_values[i][j] = zero;
}

template<size_t R, size_t C, typename T>
void Matrix<R, C, T>::unit(size_t r, size_t c) const
{
    zero();
    if( 0 <= r && r < R && 0 <= c && c <= C )
        m_values[r][c] = (T)1;
}

template<size_t R, size_t C, typename T>
void Matrix<R, C, T>::identity() const
{
    zero();
    auto diagonal = R <= C ? R : C;
    for( auto i=0; i<diagonal; i++ )
        m_values[i][i] = (T)1;
}

// unary operations
template<size_t R, size_t C, typename T>
Matrix<R, C, T> operator + (const Matrix<R, C, T>& rh)
{
    return rh;
}

template<size_t R, size_t C, typename T>
Matrix<R, C, T> operator - (const Matrix<R, C, T>& rh)
{
    auto result = rh;
    for( auto i = 0; i < R*C; i++ ) result[i] = -result[i];
    return result;
}

// linear-algebraic operations
template<size_t R, size_t C, typename T>
Matrix<R, C, T> operator + (const Matrix<R, C, T>& M0, const Matrix<R, C, T>& M1)
{
    auto result = M0;
    return result += M1;
}

template<size_t R, size_t C, typename T>
Matrix<R, C, T> operator - (const Matrix<R, C, T>& M0, const Matrix<R, C, T>& M1)
{
    auto result = M0;
    return result -= M1;
}

template<size_t R, size_t C, typename T>
Matrix<R, C, T> operator * (const Matrix<R, C, T>& M0, T scalar)
{
    auto result = M0;
    return result *= scalar;
}

template<size_t R, size_t C, typename T>
Matrix<R, C, T> operator * (T scalar, const Matrix<R, C, T>& M0)
{
    auto result = M0;
    return result *= scalar;
}

template<size_t R, size_t C, typename T>
Matrix<R, C, T> operator / (const Matrix<R, C, T>&, T)
{
    auto result = M0;
    return result /= scalar;
}

template<size_t R, size_t C, typename T>
Matrix<R, C, T>& operator += (Matrix<R, C, T>& M0, const Matrix<R, C, T>& M1)
{
    for( auto i = 0; i < R*C; i++ ) M0[i] += M1[i];
    return M0;
}

template<size_t R, size_t C, typename T>
Matrix<R, C, T>& operator -= (Matrix<R, C, T>& M0, const Matrix<R, C, T>& M1)
{
    for( auto i = 0; i < R*C; i++ ) M0[i] -= M1[i];
    return M0;
}

template<size_t R, size_t C, typename T>
Matrix<R, C, T>& operator *= (Matrix<R, C, T>& M0, T scalar)
{
    for( auto i = 0; i < R*C; i++ ) M0[i] *= scalar;
    return M0;
}

template<size_t R, size_t C, typename T>
Matrix<R, C, T>& operator /= (Matrix<R, C, T>& M0, T scalar)
{
    for( auto i = 0; i < R*C; i++ ) M0[i] /= scalar;
    return M0;
}

// M*V
template<size_t R, size_t C, typename T>
Vector<R, T> operator * (const Matrix<R, C, T>& M, const Vector<C, T>& V)
{
    Vector<R, T> result;
    for( auto r = 0; r < R; r++ )
    {
        result[r] = (T)0;
        for( auto c = 0; c < C; c++ )
            result[r] += M(r, c) * V[c];
    }

    return result;
}

// V^T*M
template<size_t R, size_t C, typename T>
Vector<C, T> operator * (const Vector<R, T>& V, const Matrix<R, C, T>& M)
{
    Vector<C, T> result;
    for( auto c = 0; c < C; c++ )
    {
        result[c] = (T)0;
        for( auto r = 0; r < R; r++ )
            result[c] += V[c] * M(r, c);
    }
    return result;
}

// A*B
template<size_t R, size_t C, size_t S, typename T>
Matrix<R, C, T> operator * (const Matrix<R, S, T>& M0, const Matrix<S, C, T>& M1)
{
    Matrix<R, C, T> result;
    for( auto r = 0; r < R; r ++ )
    {
        for( auto c = 0; c < C; c ++ )
        {
            result(r, c) = (T)0;
            for( auto i = 0; i < S; i ++ )
                result(r, c) += M0(r, i) * M1(i, c);
        }
    }
    return result;
}

// M^T
template<size_t R, size_t C, typename T>
Matrix<C, R, T> Matrix<R, C, T>::transpose()
{
    Matrix<C, R, T> result;
    for( auto r = 0; r < R; r ++ )
        for( auto c = 0; c < C; c ++ )
            result(c, r) = m_values[r][c];
    return result;
}

template<size_t N, typename T>
Matrix<N-1, N-1, T> Matrix<R, C, T>::hproject()
{
    static_assert( N >= 2, "invalid matrix dimension." );

    Matrix<N-1, N-1, T> result;
    for( auto r = 0; r < N-1; r ++ )
        for( auto c = 0; c < N-1; c ++ )
            result(r, c) = m_values[r][c];
    return result;
}

template<size_t N, typename T>
Matrix<N+1, N+1, T> Matrix::<R, C, T>::hlift()
{
    Matrix<N+1, N+1, T> result;
    result.identity();
    for( auto r = 0; r < N; r ++ )
        for( auto c = 0; c < N; c ++ )
            result(r, c) = m_values[r][c];
    return result;
}