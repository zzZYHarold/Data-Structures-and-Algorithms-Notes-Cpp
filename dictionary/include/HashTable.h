#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>

// 6.4 闭散列（开地址法），2026 秋完整课件第 110~129 页。
// 课件只给出规则与示意图，类接口为补充；两种平方增量序列分别保留。
enum class ProbingMethod { Linear, Quadratic, DoubleHashing, QuadraticAlternating };

template <class E, class K>
class HashTable {
    enum class State { Empty, Occupied, Deleted };
    struct Slot {
        E entry{};
        State state = State::Empty;
    };
    int tableSize;
    int count = 0;
    std::unique_ptr<Slot[]> slots;
    ProbingMethod method;
    std::function<int(const K&)> hash;
    std::function<int(const K&)> secondHash;

    static bool supportsAlternatingQuadratic(int size) {
        if (size % 4 != 3) return false;
        for (int divisor = 3; divisor <= size / divisor; divisor += 2)
            if (size % divisor == 0) return false;
        return true;
    }
    int normalize(int value) const {
        int remainder = value % tableSize;
        return remainder < 0 ? remainder + tableSize : remainder;
    }
    int stepFor(const K& key) const {
        if (method != ProbingMethod::DoubleHashing) return 1;
        int step = normalize(secondHash(key));
        if (step == 0) throw std::invalid_argument("double hash step must be nonzero");
        // 第 128、136 页含非互素步长，保留有限次探查；保证全表可达时应使用互素步长。
        return step;
    }
    int probeIndex(int home, int attempt, int step) const {
        long long offset = attempt;
        if (method == ProbingMethod::Quadratic) offset *= attempt;
        else if (method == ProbingMethod::QuadraticAlternating) {
            long long distance = (static_cast<long long>(attempt) + 1) / 2;
            offset = distance * distance;
            if (attempt % 2 == 0) offset = -offset; // 0,+1,-1,+4,-4,...
        } else if (method == ProbingMethod::DoubleHashing) offset *= step;
        long long index = (home + offset) % tableSize;
        if (index < 0) index += tableSize;
        return static_cast<int>(index);
    }
    void store(int index, const E& entry) {
        slots[index].entry = entry;
        slots[index].state = State::Occupied;
        ++count;
    }

public:
    HashTable(int sz, std::function<int(const K&)> hashFunction,
              ProbingMethod probing = ProbingMethod::Linear,
              std::function<int(const K&)> secondHashFunction = {})
        : tableSize(sz), method(probing), hash(std::move(hashFunction)),
          secondHash(std::move(secondHashFunction)) {
        if (sz <= 0 || !hash || (method == ProbingMethod::DoubleHashing && !secondHash))
            throw std::invalid_argument("invalid hash table configuration");
        if (method == ProbingMethod::QuadraticAlternating && !supportsAlternatingQuadratic(sz))
            throw std::invalid_argument("alternating quadratic table size must be a 4k+3 prime");
        slots = std::make_unique<Slot[]>(tableSize);
    }
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    // 0-based 散列地址，失败为 -1；probes 包含空槽/墓碑及重复地址的探查。
    int Search(const K& key, int* probes = nullptr) const {
        if (probes) *probes = 0;
        int home = normalize(hash(key));
        int step = stepFor(key);
        for (int attempt = 0; attempt < tableSize; ++attempt) {
            int index = probeIndex(home, attempt, step);
            if (probes) ++*probes;
            if (slots[index].state == State::Empty) return -1;
            if (slots[index].state == State::Occupied && slots[index].entry == key)
                return index;
        }
        return -1;
    }
    bool Insert(const K& key, const E& entry) {
        if (!(entry == key)) throw std::invalid_argument("entry key mismatch");
        int home = normalize(hash(key));
        int step = stepFor(key);
        int firstDeleted = -1;
        for (int attempt = 0; attempt < tableSize; ++attempt) {
            int index = probeIndex(home, attempt, step);
            if (slots[index].state == State::Occupied) {
                if (slots[index].entry == key) return false;
            } else if (slots[index].state == State::Deleted) {
                if (firstDeleted == -1) firstDeleted = index;
            } else {
                store(firstDeleted == -1 ? index : firstDeleted, entry);
                return true;
            }
        }
        if (firstDeleted != -1) {
            store(firstDeleted, entry);
            return true;
        }
        // 单向平方探查、非互素步长可能无法到达仍空闲的槽。
        return false;
    }
    // 补充删除：墓碑保持探查链，不能直接将槽标记为 Empty。
    bool Remove(const K& key, E& removedEntry) {
        int index = Search(key);
        if (index == -1) return false;
        removedEntry = slots[index].entry;
        slots[index].state = State::Deleted;
        --count;
        return true;
    }
    bool Update(const K& key, const E& entry) {
        if (!(entry == key)) throw std::invalid_argument("Update cannot change a key");
        int index = Search(key);
        if (index == -1) return false;
        slots[index].entry = entry;
        return true;
    }
    bool getData(int index, E& entry) const {
        if (index < 0 || index >= tableSize || slots[index].state != State::Occupied)
            return false;
        entry = slots[index].entry;
        return true;
    }
    void makeEmpty() {
        for (int index = 0; index < tableSize; ++index) slots[index].state = State::Empty;
        count = 0;
    }
    int Size() const { return tableSize; }
    int Length() const { return count; }
};
