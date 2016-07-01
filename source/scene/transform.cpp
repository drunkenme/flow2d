// @date 2016/05/26
// @author Mao Jingkai(oammix@gmail.com)

#include <scene/transform.hpp>
#include <scene/scene.hpp>

NS_FLOW2D_BEGIN

void NSScene::visit_children(EntityManager& world, Transform& t, const visitor& cb, bool recursive)
{
    for( auto cursor = t._first_child; cursor != nullptr; cursor = cursor->_next_sibling )
    {
        cb(t, *cursor);
        if( recursive )
            NSScene::visit_children(world, *cursor, cb, recursive);
    }
}

void NSScene::update_children(EntityManager& world, Transform& t)
{
    NSScene::visit_children(world, t, [](const Transform& parent, Transform& c)
    {
        c._worldspace = parent._worldspace * c._localspace;
    }, true);
}

void NSScene::set_scale(EntityManager& world, Transform& t, const Vector2f& scale, TransformSpace space)
{
    if( TransformSpace::SELF == space )
    {
        t._localspace.scale = scale;
        if( t._parent )
            t._worldspace = t._parent->_worldspace * t._localspace;
    }
    else
    {
        t._worldspace.scale = scale;
        if( t._parent )
            t._localspace = t._worldspace / t._parent->_worldspace;
    }

    NSScene::update_children(world, t);
}

void NSScene::set_position(EntityManager& world, Transform& t, const Vector2f& position, TransformSpace space)
{
    if( TransformSpace::SELF == space )
    {
        t._localspace.position = position;
        if( t._parent )
            t._worldspace = t._parent->_worldspace * t._localspace;
    }
    else
    {
        t._worldspace.position = position;
        if( t._parent )
            t._localspace = t._worldspace / t._parent->_worldspace;
    }

    NSScene::update_children(world, t);
}

void NSScene::set_rotation(EntityManager& world, Transform& t, float rotation, TransformSpace space)
{
    if( TransformSpace::SELF == space )
    {
        t._localspace.rotation = rotation;
        if( t._parent )
            t._worldspace = t._parent->_worldspace * t._localspace;
    }
    else
    {
        t._worldspace.rotation = rotation;
        if( t._parent )
            t._localspace = t._worldspace / t._parent->_worldspace;
    }

    NSScene::update_children(world, t);
}

Vector2f NSScene::get_scale(EntityManager& world, const Transform& t, TransformSpace space)
{
    if( TransformSpace::SELF == space )
        return t._localspace.scale;
    else
        return t._worldspace.scale;
}

Vector2f NSScene::get_position(EntityManager& world, const Transform& t, TransformSpace space)
{
    if( TransformSpace::SELF == space )
        return t._localspace.position;
    else
        return t._worldspace.position;
}

float NSScene::get_rotation(EntityManager& world, const Transform& t, TransformSpace space)
{
    if( TransformSpace::SELF == space )
        return t._localspace.rotation;
    else
        return t._worldspace.rotation;
}

void NSScene::append_child(EntityManager& world, Transform& pt, Transform& ct, bool keep_world_pose)
{
    if( ct._parent != nullptr )
        NSScene::remove_from_parent(world, ct);

    if( pt._first_child != nullptr )
    {
        ENSURE( pt._first_child->_prev_sibling == nullptr );
        pt._first_child->_prev_sibling = &ct;
        ct._next_sibling = pt._first_child;
    }

    pt._first_child = &ct;
    ct._parent = &pt;

    if( !keep_world_pose )
        ct._worldspace = pt._worldspace * ct._localspace;
    else
        ct._localspace = ct._worldspace / pt._worldspace;

    NSScene::update_children(world, ct);
}

void NSScene::remove_from_parent(EntityManager& world, Transform& t)
{
    if( t._parent == nullptr )
        return;

    if( t._parent->_first_child == &t )
    {
        t._parent->_first_child = t._next_sibling;
        if( t._next_sibling )
            t._next_sibling->_prev_sibling = nullptr;
    }
    else
    {
        ENSURE( t._prev_sibling != nullptr );
        t._prev_sibling->_next_sibling = t._next_sibling;
        if( t._next_sibling )
            t._next_sibling->_prev_sibling = t._prev_sibling;
    }

    t._parent = nullptr;
    t._prev_sibling = nullptr;
    t._next_sibling = nullptr;
}

bool NSScene::is_root(EntityManager& world, const Transform& t)
{
    return t._parent == nullptr;
}

bool NSScene::is_leaf(EntityManager& world, const Transform& t)
{
    return t._first_child == nullptr;
}

Entity NSScene::get_object(EntityManager& world, const Transform& t)
{
    return t._object;
}

Entity NSScene::get_parent(EntityManager& world, const Transform& t)
{
    if( t._parent )
        return t._parent->_object;
    return Entity();
}

size_t NSScene::get_children_count(EntityManager& world, const Transform& t, bool recursive)
{
    size_t result = 0;
    NSScene::visit_children(world, const_cast<Transform&>(t), [&](const Transform&, Transform&) { result ++; }, recursive);
    return result;
}

/// MEMBER METHODS
void Transform::on_spawn(EntityManager& world, Entity object)
{
    _object = object;
}

void Transform::on_dispose(EntityManager& world, Entity object)
{
    if( _parent )
        NSScene::remove_from_parent(world, *this);
}

NS_FLOW2D_END