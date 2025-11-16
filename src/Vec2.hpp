#pragma once

#include <cstdint>
#include <functional>

namespace sw
{
    // Структура для хранения 2D координат
    struct Vec2
    {
        int32_t x = 0;
        int32_t y = 0;

        Vec2() = default;
        Vec2(int32_t x, int32_t y) : x(x), y(y) {}

        // Операторы сравнения
        bool operator==(const Vec2& other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Vec2& other) const
        {
            return !(*this == other);
        }

        // Арифметические операторы
        Vec2 operator+(const Vec2& other) const
        {
            return Vec2(x + other.x, y + other.y);
        }

        Vec2 operator-(const Vec2& other) const
        {
            return Vec2(x - other.x, y - other.y);
        }

        Vec2& operator+=(const Vec2& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vec2& operator-=(const Vec2& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }
    };
}

// Хэш-функция для Vec2
namespace std
{
    template <>
    struct hash<sw::Vec2>
    {
        size_t operator()(const sw::Vec2& v) const noexcept
        {
            // Комбинируем хэши x и y с помощью простого алгоритма
            size_t h1 = std::hash<int32_t>{}(v.x);
            size_t h2 = std::hash<int32_t>{}(v.y);
            return h1 ^ (h2 << 1); // XOR с сдвигом для лучшего распределения
        }
    };
}
