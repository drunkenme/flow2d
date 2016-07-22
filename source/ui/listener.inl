// @date 2016/07/05
// @author Mao Jingkai(oammix@gmail.com)

template<size_t N> EventListener<N>& EventListener<N>::operator += (const closure& c)
{
    for( size_t i = 0; i < N; i++ )
    {
        if( _callbacks[i].first == c.first )
        {
            _callbacks[i] = c;
            return *this;
        }
    }

    for( size_t i = 0; i < N; i++ )
    {
        if( _callbacks[i].first == nullptr )
        {
            _callbacks[i] = c;
            return *this;
        }
    }

    LOGW("failed to add event listener.");
    return *this;
}

template<size_t N> EventListener<N>& EventListener<N>::operator -= (void* owner)
{
    for( size_t i = 0; i < N; i++ )
    {
        if( _callbacks[i].first == owner )
        {
            _callbacks[i].first = nullptr;
            _callbacks[i].second = nullptr;
        }
    }
    return *this;
}

template<size_t N> void EventListener<N>::operator () (void* a1)
{
    for( size_t i = 0; i < N; i++ )
        if( _callbacks[i].first != nullptr )
            _callbacks[i].second(a1);
}

template<typename E, typename T>
void EventListenerGroup::subscribe(T& owner)
{
    static_assert( std::is_base_of<EvtBase, E>::value, "E is not subtype of EvtBase." );

    auto found = _listeners.find(TypeID::value<EvtBase, E>());
    if( found == _listeners.end() )
        _listeners[TypeID::value<EvtBase, E>()] = EventListener<kMaxEventListeners>();

    _listeners[TypeID::value<EvtBase, E>()] +=
        std::make_pair(static_cast<void*>(&owner), [&](void* c)
        {
            owner.receive(*static_cast<E*>(c));
        });
}

template<typename E, typename T>
void EventListenerGroup::subscribe(T& owner, const func<E>& f)
{
    static_assert( std::is_base_of<EvtBase, E>::value, "E is not subtype of EvtBase." );

    auto found = _listeners.find(TypeID::value<EvtBase, E>());
    if( found == _listeners.end() )
        _listeners[TypeID::value<EvtBase, E>()] = EventListener<kMaxEventListeners>();

    _listeners[TypeID::value<EvtBase, E>()] +=
        std::make_pair(static_cast<void*>(&owner), [&](void* c)
        {
            f(*static_cast<E*>(c));
        });
}

template<typename E, typename T>
void EventListenerGroup::unsubscribe(T& owner)
{
    static_assert( std::is_base_of<EvtBase, E>::value, "E is not subtype of EvtBase." );

    auto found = _listeners.find(TypeID::value<EvtBase, E>());
    if( found != _listeners.end() )
        found->second -= static_cast<void*>(&owner);
}

template<typename E> void EventListenerGroup::emit(E& evt)
{
    static_assert( std::is_base_of<EvtBase, E>::value, "E is not subtype of EvtBase." );

    auto found = _listeners.find(TypeID::value<EvtBase, E>());
    if( found != _listeners.end() )
        found->second(static_cast<void*>(&evt));
}

template<typename E, typename T>
void EventListenerGroup::subscribe(Transform& transform, T& owner, const func<E>& cb)
{
    if( !transform.has_component<EventListenerGroup>() )
        transform.add_component<EventListenerGroup>();

    transform.get_component<EventListenerGroup>()->subscribe(owner, cb);
}

template<typename T>
void EventListenerGroup::unsubscribe(Transform& transform, T& owner)
{
    if( transform.has_component<EventListenerGroup>() )
        transform.get_component<EventListenerGroup>()->unsubscribe(owner);
}