#pragma once

#include <cstdint>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

// 6.1 位向量集合（2026 秋正式课件第 9~19 页）：每个 16 位字从高位到低位表示 16 个成员。
// 课件漏写了模板声明；T 必须是整数类型，成员范围为 [0, setSize)。
template <class T = int>
class bitSet {
    static_assert(std::is_integral_v<T>);
    int setSize;
    int vectorSize;
    std::uint16_t* bitVector;

    int memberIndex(T member) const {
        if constexpr (std::is_signed_v<T>) {
            if (member < 0)
                throw std::out_of_range("bitSet member out of range");
        }
        if (static_cast<std::uintmax_t>(member) >= static_cast<std::uintmax_t>(setSize))
            throw std::out_of_range("bitSet member out of range");
        return static_cast<int>(member);
    }

    void requireSameUniverse(const bitSet& other) const {
        if (setSize != other.setSize)
            throw std::invalid_argument("bitSet universe sizes differ");
    }

public:
    explicit bitSet(int sz = 50) : setSize(sz), vectorSize(0), bitVector(nullptr) {
        if (sz < 0)
            throw std::invalid_argument("negative bitSet size");
        vectorSize = sz / 16 + (sz % 16 != 0);
        bitVector = new std::uint16_t[vectorSize]{};
    }

    bitSet(const bitSet& other) : bitSet(other.setSize) {
        for (int i = 0; i < vectorSize; ++i) bitVector[i] = other.bitVector[i];
    }

    ~bitSet() { delete[] bitVector; }

    bitSet& operator=(const bitSet& other) {
        if (this != &other) {
            bitSet copy(other);
            std::swap(setSize, copy.setSize);
            std::swap(vectorSize, copy.vectorSize);
            std::swap(bitVector, copy.bitVector);
        }
        return *this;
    }

    int getMember(T member) const {
        int index = memberIndex(member);
        return (bitVector[index / 16] >> (15 - index % 16)) & 1u;
    }

    void putMember(T member, int value) {
        int index = memberIndex(member);
        if (value != 0 && value != 1)
            throw std::invalid_argument("bit value must be 0 or 1");

        auto mask = static_cast<std::uint16_t>(1u << (15 - index % 16));
        if (value)
            bitVector[index / 16] |= mask;
        else
            bitVector[index / 16] &= static_cast<std::uint16_t>(~mask);
    }

    void makeEmpty() {
        for (int i = 0; i < vectorSize; ++i)
            bitVector[i] = 0;
    }
    bool Contains(T member) const { return getMember(member) != 0; }

    bool addMember(T member) {
        if (Contains(member))
            return false;
        putMember(member, 1);
        return true;
    }
    bool delMember(T member) {
        if (!Contains(member)) return false;
        putMember(member, 0);
        return true;
    }

    bitSet operator+(const bitSet& other) const {
        requireSameUniverse(other);
        bitSet result(setSize);
        for (int i = 0; i < vectorSize; ++i)
            result.bitVector[i] = bitVector[i] | other.bitVector[i];
        return result;
    }

    bitSet operator*(const bitSet& other) const {
        requireSameUniverse(other);
        bitSet result(setSize);
        for (int i = 0; i < vectorSize; ++i)
            result.bitVector[i] = bitVector[i] & other.bitVector[i];
        return result;
    }

    bitSet operator-(const bitSet& other) const {
        requireSameUniverse(other);
        bitSet result(setSize);
        for (int i = 0; i < vectorSize; ++i)
            result.bitVector[i] = bitVector[i] & ~other.bitVector[i];
        return result;
    }

    bool subSet(const bitSet& other) const {
        requireSameUniverse(other);
        for (int i = 0; i < vectorSize; ++i)
            if (bitVector[i] & ~other.bitVector[i])
                return false;
        return true;
    }

    bool operator==(const bitSet& other) const {
        if (setSize != other.setSize) return false;
        for (int i = 0; i < vectorSize; ++i)
            if (bitVector[i] != other.bitVector[i])
                return false;
        return true;
    }

    // 补充输入约定：成员数 count 后跟 count 个整数；非法输入不改变原集合。
    friend std::istream& operator>>(std::istream& input, bitSet& target) {
        int count;
        if (!(input >> count)) return input;
        if (count < 0) { input.setstate(std::ios::failbit); return input; }
        bitSet replacement(target.setSize);
        for (int i = 0; i < count; ++i) {
            T member;
            if (!(input >> member)) return input;
            try { replacement.addMember(member); }
            catch (const std::out_of_range&) {
                input.setstate(std::ios::failbit);
                return input;
            }
        }
        target = replacement;
        return input;
    }
    friend std::ostream& operator<<(std::ostream& output, const bitSet& target) {
        output << '{';
        bool firstMember = true;
        for (int index = 0; index < target.setSize; ++index) {
            if ((target.bitVector[index / 16] >> (15 - index % 16)) & 1u) {
                if (!firstMember) output << ',';
                output << index;
                firstMember = false;
            }
        }
        return output << '}';
    }
};
