#include "ScriptInstance.h"

#include "api/Components.h"
#include "api/Physics.h"

namespace Engine
{
    ScriptInstance::ScriptInstance(ScriptingApi::Entity entity, ScriptClass* scriptClass,
                                   const sol::table& luaTable)
        : m_entity{entity}, m_luaTable{luaTable}, m_scriptClass{scriptClass}
    {
        m_luaTable["entity"] = m_entity;
    }

    void ScriptInstance::SetAttribute(std::string_view name, Variant value)
    {
        switch (value.type) {
        case Variant::Type::tInteger:
            m_luaTable[name] = value.asInteger;
            break;
        case Variant::Type::tFloat:
            m_luaTable[name] = value.asFloat;
            break;
        case Variant::Type::tBool:
            m_luaTable[name] = value.asBool;
            break;
        case Variant::Type::tStringId:
            m_luaTable[name] = std::string{StringId::GetString(value.asStringId)};
        default:
            break;
        }
    }

    void ScriptInstance::InvokeOnStart() { InvokeFunction("OnStart"); }

    void ScriptInstance::InvokeOnUpdate(float timeStep) { InvokeFunction("OnUpdate", timeStep); }

    void ScriptInstance::InvokeOnLateUpdate(float timeStep) { InvokeFunction("OnLateUpdate", timeStep); }

    void ScriptInstance::InvokeOnCollisionEnter(Entity& otherEntity, const CollisionManifold2D& manifold,
                                                Shape2DId otherShapeId)
    {
        ScriptingApi::Collision2DData collisionData{};
        for (int i{0}; i < manifold.pointCount; ++i) {
            const auto manifoldPoint{manifold.points[i]};
            collisionData.points[static_cast<std::size_t>(i)] =
                ScriptingApi::CollisionPoint2D{glm::vec2{manifoldPoint.anchorA.x, manifoldPoint.anchorA.y},
                                               glm::vec2{manifoldPoint.anchorB.x, manifoldPoint.anchorB.y},
                                               manifoldPoint.normalImpulse,
                                               manifoldPoint.normalVelocity,
                                               manifoldPoint.separation,
                                               manifoldPoint.tangentImpulse,
                                               manifoldPoint.totalNormalImpulse,
                                               manifoldPoint.id,
                                               manifoldPoint.persisted};
        }
        collisionData.normal = glm::vec2{manifold.normal.x, manifold.normal.y};
        collisionData.pointCount = manifold.pointCount;
        ScriptingApi::Collider2D collider{&otherEntity};
        collider.SetShapeId(otherShapeId);
        InvokeFunction("OnCollisionEnter", collider, collisionData);
    }

    void ScriptInstance::InvokeOnCollisionExit(Entity& otherEntity, Shape2DId otherShapeId)
    {
        ScriptingApi::Collider2D collider{&otherEntity};
        collider.SetShapeId(otherShapeId);
        InvokeFunction("OnCollisionExit", collider);
    }

    void ScriptInstance::InvokeOnTriggerEnter(Entity& otherEntity, Shape2DId otherShapeId)
    {
        ScriptingApi::Collider2D collider{&otherEntity};
        collider.SetShapeId(otherShapeId);
        InvokeFunction("OnTriggerEnter", collider);
    }

    void ScriptInstance::InvokeOnTriggerExit(Entity& otherEntity, Shape2DId otherShapeId)
    {
        ScriptingApi::Collider2D collider{&otherEntity};
        collider.SetShapeId(otherShapeId);
        InvokeFunction("OnTriggerExit", collider);
    }
} // namespace Engine
