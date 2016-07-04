// @date 2016/07/01
// @author Mao Jingkai(oammix@gmail.com)

INLINE Transform::iterator& Transform::iterator::operator ++ ()
{
    if( _cusor == nullptr )
        return *this;

    if( _mode == iterator_mode::ANCESTORS )
    {
        _cusor = _cusor->_parent;
    }
    else if( _mode == iterator_mode::CHILDREN )
    {
        _cusor = _cusor->_next_sibling;
    }
    else if( _mode == iterator_mode::CHILDREN_RECURSIVE )
    {
        if( _cusor->_first_child )
            _cusor = _cusor->_first_child;
        else if( _cusor->_next_sibling )
            _cusor = _cusor->_next_sibling;
        else
        {
            bool found_available_node = false;
            while( _cusor->_parent != _start )
            {
                if( _cusor->_parent->_next_sibling )
                {
                    _cusor = _cusor->_parent->_next_sibling;
                    found_available_node = true;
                    break;
                }
                else
                    _cusor = _cusor->_parent;
            }

            if( !found_available_node )
                _cusor = nullptr;
        }
    }

    return *this;
}

INLINE bool Transform::iterator::operator == (const Transform::iterator& rh) const
{
    return _cusor == rh._cusor && _mode == rh._mode;
}

INLINE bool Transform::iterator::operator != (const Transform::iterator& rh) const
{
    return !(*this == rh);
}

INLINE Transform& Transform::iterator::operator * () const
{
    return *_cusor;
}

INLINE Transform::iterator Transform::view::begin() const
{
    return Transform::iterator(_start, _mode);
}

INLINE Transform::iterator Transform::view::end() const
{
    return Transform::iterator(nullptr, _mode);
}