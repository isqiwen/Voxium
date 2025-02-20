#pragma once

#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <thread>
#include <mutex>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Voxium::Core::Math::Rand {
    inline thread_local std::mt19937 rng{ std::random_device{}() };

    inline void Init(unsigned int seed) {
        rng.seed(seed);
    }

    constexpr int ANGLE_STEP_COUNT = 256;
    constexpr int NORMALISED_VECTOR_COUNT = ANGLE_STEP_COUNT * ANGLE_STEP_COUNT;
    constexpr float PITCH_STEP = glm::pi<float>() / ANGLE_STEP_COUNT;
    constexpr float YAW_STEP = glm::two_pi<float>() / ANGLE_STEP_COUNT;

    inline static std::vector<glm::vec3> NormalizedRandoms = [](){
        std::vector<glm::vec3> vecs;
        vecs.reserve(NORMALISED_VECTOR_COUNT);
        float pitch = -glm::half_pi<float>();
        for (int i = 0; i < ANGLE_STEP_COUNT; ++i) {
            float yaw = 0.0f;
            for (int j = 0; j < ANGLE_STEP_COUNT; ++j) {
                float x = std::cos(pitch) * std::cos(yaw);
                float y = std::sin(pitch);
                float z = std::cos(pitch) * std::sin(yaw);
                vecs.push_back(glm::vec3(x, y, z));
                yaw += YAW_STEP;
            }
            pitch += PITCH_STEP;
        }
        return vecs;
    }();

    inline uint8_t Next(uint8_t max) {
        std::uniform_int_distribution<uint16_t> dist(0, max - 1);
        return static_cast<uint8_t>(dist(rng));
    }

    inline int Next(int max) {
        std::uniform_int_distribution<int> dist(0, max - 1);
        return dist(rng);
    }

    inline int Next(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max - 1);
        return dist(rng);
    }

    inline glm::ivec3 NextI3(int min, int max) {
        return glm::ivec3(Next(min, max), Next(min, max), Next(min, max));
    }

    inline float Next(float max) {
        std::uniform_real_distribution<float> dist(0.0f, max);
        return dist(rng);
    }

    inline float Next(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }

    inline glm::vec3 NextV3Normalised() {
        int idx = Next(NORMALISED_VECTOR_COUNT);
        return NormalizedRandoms[idx];
    }

    inline glm::vec3 NextV3Normalised(float magnitude) {
        return NextV3Normalised() * magnitude;
    }

    inline glm::vec3 NextV3(float min, float max) {
        float diff = max - min;
        return glm::vec3(NextFloat() * diff + min, NextFloat() * diff + min, NextFloat() * diff + min);
    }

    inline double Next(double max) {
        std::uniform_real_distribution<double> dist(0.0, max);
        return dist(rng);
    }

    inline double Next(double min, double max) {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(rng);
    }

    inline glm::vec3 Angle(double magnitude) {
        double pitch = Next(-glm::pi<double>(), glm::pi<double>());
        double yaw   = Next(-glm::pi<double>(), glm::pi<double>());
        float x = static_cast<float>(std::cos(pitch) * std::cos(yaw));
        float y = static_cast<float>(std::sin(pitch));
        float z = static_cast<float>(std::cos(pitch) * std::sin(yaw));
        return glm::vec3(x, y, z) * static_cast<float>(magnitude);
    }

    inline glm::vec3 NextV3(double min, double max) {
        double diff = max - min;
        float x = static_cast<float>(Next(min, max));
        float y = static_cast<float>(Next(min, max));
        float z = static_cast<float>(Next(min, max));
        return glm::vec3(x, y, z);
    }

    template<typename Container>
    inline auto Next(const Container& container) -> const typename Container::value_type& {
        std::size_t count = container.size();
        std::uniform_int_distribution<std::size_t> dist(0, count - 1);
        return container[dist(rng)];
    }

    inline char NextChar(const std::string& s) {
        std::uniform_int_distribution<std::size_t> dist(0, s.size() - 1);
        return s[dist(rng)];
    }

    inline uint8_t NextByte() {
        std::uniform_int_distribution<int> dist(0, 254);
        return static_cast<uint8_t>(dist(rng));
    }

    inline double NextDouble() {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rng);
    }

    inline float NextFloat() {
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(rng);
    }

    inline std::vector<int> CreateOrder(int max) {
        std::vector<int> result(max);
        for (int i = 0; i < max; ++i)
            result[i] = i;
        std::shuffle(result.begin(), result.end(), rng);
        return result;
    }

} // namespace Voxium::Core::Math::Rand
