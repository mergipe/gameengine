#pragma once

#include "core/Types.h"

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace YAML
{
    template <>
    struct convert<glm::vec2> {
        static Node encode(glm::vec2 vector)
        {
            Node node{};
            node["x"] = vector.x;
            node["y"] = vector.y;
            return node;
        }

        static bool decode(const Node& node, glm::vec2& vector)
        {
            if (!node["x"] || !node["y"]) {
                return false;
            }
            vector.x = node["x"].as<float>();
            vector.y = node["y"].as<float>();
            return true;
        }
    };

    template <>
    struct convert<glm::vec3> {
        static Node encode(glm::vec3 vector)
        {
            Node node{};
            node["x"] = vector.x;
            node["y"] = vector.y;
            node["z"] = vector.z;
            return node;
        }

        static bool decode(const Node& node, glm::vec3& vector)
        {
            if ((!node["x"] || !node["y"] || !node["z"]) && (!node["r"] || !node["g"] || !node["b"])) {
                return false;
            }
            if (node["x"]) {
                vector.x = node["x"].as<float>();
            } else if (node["r"]) {
                vector.r = node["r"].as<float>();
            }
            if (node["y"]) {
                vector.y = node["y"].as<float>();
            } else if (node["g"]) {
                vector.g = node["g"].as<float>();
            }
            if (node["z"]) {
                vector.z = node["z"].as<float>();
            } else if (node["b"]) {
                vector.b = node["b"].as<float>();
            }
            return true;
        }
    };

    template <>
    struct convert<Engine::RGBA8> {
        static Node encode(Engine::RGBA8 color)
        {
            Node node{};
            node["r"] = color.r;
            node["g"] = color.g;
            node["b"] = color.b;
            node["a"] = color.a;
            return node;
        }

        static bool decode(const Node& node, Engine::RGBA8& color)
        {
            if (node["r"]) {
                color.r = node["r"].as<Engine::U8>();
            }
            if (node["g"]) {
                color.g = node["g"].as<Engine::U8>();
            }
            if (node["b"]) {
                color.b = node["b"].as<Engine::U8>();
            }
            if (node["a"]) {
                color.a = node["a"].as<Engine::U8>();
            }
            return true;
        }
    };
} // namespace YAML
