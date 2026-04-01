// Implementation of pylist per problem 031 requirements
#ifndef PYLIST_H
#define PYLIST_H

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_set>

class pylist {
public:
    // Value kind: either an integer or a shared list
    enum class Kind { INT, LIST };

private:
    Kind kind_;
    int  iv_ = 0;
    std::shared_ptr<std::vector<pylist>> vec_;

    static void print_impl(std::ostream &os, const pylist &v,
                           std::unordered_set<const void*> &stack) {
        if (v.kind_ == Kind::INT) {
            os << v.iv_;
            return;
        }
        // v is a list
        const void *id = v.vec_.get();
        if (stack.find(id) != stack.end()) {
            os << "[...]";
            return;
        }
        stack.insert(id);
        os << '[';
        if (v.vec_ && !v.vec_->empty()) {
            const auto &arr = *v.vec_;
            for (std::size_t i = 0; i < arr.size(); ++i) {
                print_impl(os, arr[i], stack);
                if (i + 1 < arr.size()) os << ", ";
            }
        }
        os << ']';
        stack.erase(id);
    }

public:
    // Default constructs an empty list
    pylist() : kind_(Kind::LIST), iv_(0), vec_(std::make_shared<std::vector<pylist>>()) {}

    // Construct from integer (implicit)
    pylist(int v) : kind_(Kind::INT), iv_(v), vec_(nullptr) {}

    // Copy/move defaulted: preserve reference semantics for lists
    pylist(const pylist &) = default;
    pylist(pylist &&) noexcept = default;
    pylist &operator=(const pylist &) = default;
    pylist &operator=(pylist &&) noexcept = default;

    // Allow assignment from int
    pylist &operator=(int v) {
        kind_ = Kind::INT;
        iv_ = v;
        vec_.reset();
        return *this;
    }

    // Append element to list (amortized O(1))
    void append(const pylist &x) {
        // Assume correct usage: appending only on list kinds
        if (kind_ != Kind::LIST) {
            // Transform this into an empty list first if needed
            kind_ = Kind::LIST;
            vec_ = std::make_shared<std::vector<pylist>>();
        }
        vec_->push_back(x);
    }

    // Pop element from list and return it (amortized O(1))
    pylist pop() {
        // Assume non-empty when used
        pylist ret = vec_->back();
        vec_->pop_back();
        return ret;
    }

    // Indexing (O(1))
    pylist &operator[](std::size_t i) {
        return (*vec_)[i];
    }
    const pylist &operator[](std::size_t i) const {
        return (*vec_)[i];
    }

    // Implicit conversion to int for arithmetic/bitwise operations in tests
    operator int() const {
        return iv_;
    }

    friend std::ostream &operator<<(std::ostream &os, const pylist &ls) {
        std::unordered_set<const void*> stack;
        print_impl(os, ls, stack);
        return os;
    }
};

#endif // PYLIST_H

