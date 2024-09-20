#include <iterator>

template <typename T>
concept IsContainer = requires(T container) {
    container.begin();
    container.end();
    requires std::derived_from<typename T::const_iterator::iterator_category, std::forward_iterator_tag>;
};

template <typename T>
concept IsAssociativeContainer = requires {
    requires IsContainer<T>;
    requires std::same_as<std::pair<typename T::value_type::first_type, typename T::value_type::second_type>, typename T::value_type>;
};

template <typename D, typename Pred>
class filter {
public:
    static_assert(IsContainer<D>, "There should be a container at the entrance.");
    class const_iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using iterator_category = D::const_iterator::iterator_category;

        const_iterator(typename D::const_iterator it, Pred pred, typename D::const_iterator end, typename D::const_iterator begin) :
                it_(it), pred_(pred), end_(end), begin_(begin) {};
        const_iterator(const const_iterator&) = default;
        const_iterator& operator=(const const_iterator&) = default;
        bool operator==(const const_iterator& other) const = default;
        bool operator!=(const const_iterator& other) const = default;

        auto operator*() const {
            return *it_;
        }

        auto operator->() const {
            return &(*it_);
        }

        const_iterator& operator++() {
            ++it_;
            while (it_ != end_ && !pred_(*it_)) {
                ++it_;
            }
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator copy = *this;
            ++(*this);
            return copy;
        }

        const_iterator& operator--() {
            --it_;
            while (it_ != begin_ && !pred_(*it_)) {
                --it_;
            }
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator copy = *this;
            --(*this);
            return copy;
        }

    private:
        Pred pred_;
        typename D::const_iterator it_;
        typename D::const_iterator end_;
        typename D::const_iterator begin_;
    };

    using value_type = D::value_type;
    explicit constexpr filter(D& base, Pred pred) : begin_(base.begin()), end_(base.end()), pred_(pred) {};

    constexpr const_iterator begin() {
        auto it = begin_;
        while (it != end_ && !pred_(*it)) {
            ++it;
        }
        return const_iterator(it, pred_, end_, begin_);
    }

    constexpr const_iterator end() {
        return const_iterator(end_, pred_, end_, begin_);
    }

private:
    typename D::const_iterator begin_;
    typename D::const_iterator end_;
    Pred pred_;
};

template <typename T, typename D>
filter<T, D> filter_view(T& cont, D argument) {
    return filter<T, D>(cont, argument);
}

template <typename D>
class take {
public:
    static_assert(IsContainer<D>, "There should be a container at the entrance.");
    class const_iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using iterator_category = D::const_iterator::iterator_category;

        const_iterator(typename D::const_iterator it) : it_(it) {};
        const_iterator(const const_iterator&) = default;
        const_iterator& operator=(const const_iterator&) = default;
        bool operator==(const const_iterator& other) const = default;
        bool operator!=(const const_iterator& other) const = default;

        auto operator*() const {
            return *it_;
        }

        auto operator->() const {
            return &(*it_);
        }

        const_iterator& operator++() {
            ++it_;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator copy = *this;
            ++(*this);
            return copy;
        }

        const_iterator& operator--() {
            --it_;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator copy = *this;
            --(*this);
            return copy;
        }

    private:
        typename D::const_iterator it_;
    };

    using value_type = D::value_type;
    explicit constexpr take(D& base, size_t count) : begin_(base.begin()), end_(base.end()), count_(count) {};

    constexpr const_iterator begin() {
        return const_iterator(begin_);
    }

    constexpr const_iterator end() {
        auto it = begin();
        size_t count = 0;
        while (count < count_ && it != end_) {
            ++count;
            ++it;
        }
        return it;
    }

private:
    typename D::const_iterator begin_;
    typename D::const_iterator end_;
    size_t count_;
};

template <typename T>
take<T> take_view(T& cont, size_t count) {
    return take<T>(cont, count);
}

template <typename D, typename Pred>
class transform {
public:
    static_assert(IsContainer<D>, "There should be a container at the entrance.");
    class const_iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using iterator_category = D::const_iterator::iterator_category;

        const_iterator(typename D::const_iterator it, Pred pred) : it_(it), pred_(pred) {};
        const_iterator(const const_iterator&) = default;
        const_iterator& operator=(const const_iterator&) = default;
        bool operator==(const const_iterator& other) const = default;
        bool operator!=(const const_iterator& other) const = default;

        auto operator*() const {
            return pred_(*it_);
        }

        auto operator->() const {
            return &(*it_);
        }

        const_iterator& operator++() {
            ++it_;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator copy = *this;
            ++(*this);
            return copy;
        }

        const_iterator& operator--() {
            --it_;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator copy = *this;
            --(*this);
            return copy;
        }

    private:
        Pred pred_;
        typename D::const_iterator it_;
    };

    using value_type = D::value_type;
    explicit constexpr transform(D& base, Pred pred) : begin_(base.begin()), end_(base.end()), pred_(pred) {};

    constexpr const_iterator begin() {
        return const_iterator(begin_, pred_);
    }

    constexpr const_iterator end() {
        return const_iterator(end_, pred_);
    }

private:
    typename D::const_iterator begin_;
    typename D::const_iterator end_;
    Pred pred_;
};

template <typename T, typename D>
transform<T, D> transform_view(T& cont, D argument) {
    return transform<T, D>(cont, argument);
}

template <typename D>
class drop {
public:
    static_assert(IsContainer<D>, "There should be a container at the entrance.");
    class const_iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using iterator_category = D::const_iterator::iterator_category;

        const_iterator(typename D::const_iterator it) : it_(it) {};
        const_iterator(const const_iterator&) = default;
        const_iterator& operator=(const const_iterator&) = default;
        bool operator==(const const_iterator& other) const = default;
        bool operator!=(const const_iterator& other) const = default;

        auto operator*() const {
            return *it_;
        }

        auto operator->() const {
            return &(*it_);
        }

        const_iterator& operator++() {
            ++it_;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator copy = *this;
            ++(*this);
            return copy;
        }

        const_iterator& operator--() {
            --it_;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator copy = *this;
            --(*this);
            return copy;
        }

    private:
        typename D::const_iterator it_;
    };

    using value_type = D::value_type;
    explicit constexpr drop(D& base, size_t count) : begin_(base.begin()), end_(base.end()), count_(count) {};

    constexpr const_iterator begin() {
        auto it = begin_;
        for (size_t i = 0; i < count_; ++i) {
            if (it == end_) {
                break;
            }
            ++it;
        }
        return it;
    }

    constexpr const_iterator end() {
        return const_iterator(end_);
    }

private:
    typename D::const_iterator begin_;
    typename D::const_iterator end_;
    size_t count_;
};

template <typename T>
drop<T> drop_view(T& cont, size_t count) {
    return drop<T>(cont, count);
}

template <typename D>
class keys {
public:
    static_assert(IsAssociativeContainer<D>, "There should be an associative container at the entrance.");
    class const_iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using iterator_category = D::const_iterator::iterator_category;

        const_iterator(typename D::const_iterator it) : it_(it) {};
        const_iterator(const const_iterator&) = default;
        const_iterator& operator=(const const_iterator&) = default;
        bool operator==(const const_iterator& other) const = default;
        bool operator!=(const const_iterator& other) const = default;

        auto operator*() const {
            return (*it_).first;
        }

        auto operator->() const {
            return &((*it_).first);
        }

        const_iterator& operator++() {
            ++it_;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator copy = *this;
            ++(*this);
            return copy;
        }

        const_iterator& operator--() {
            --it_;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator copy = *this;
            --(*this);
            return copy;
        }

    private:
        typename D::const_iterator it_;
    };

    using value_type = D::value_type;
    explicit constexpr keys(D& base) : begin_(base.begin()), end_(base.end()) {};

    constexpr const_iterator begin() {
        return const_iterator(begin_);
    }

    constexpr const_iterator end() {
        return const_iterator(end_);
    }

private:
    typename D::const_iterator begin_;
    typename D::const_iterator end_;
};

template <typename T>
keys<T> keys_view(T& cont) {
    return keys<T>(cont);
}

template <typename D>
class values {
public:
    static_assert(IsAssociativeContainer<D>, "There should be an associative container at the entrance.");
    class const_iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using iterator_category = D::const_iterator::iterator_category;

        const_iterator(typename D::const_iterator it) : it_(it) {};
        const_iterator(const const_iterator&) = default;
        const_iterator& operator=(const const_iterator&) = default;
        bool operator==(const const_iterator& other) const = default;
        bool operator!=(const const_iterator& other) const = default;

        auto operator*() const {
            return (*it_).second;
        }

        auto operator->() const {
            return &((*it_).second);
        }

        const_iterator& operator++() {
            ++it_;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator copy = *this;
            ++(*this);
            return copy;
        }

        const_iterator& operator--() {
            --it_;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator copy = *this;
            --(*this);
            return copy;
        }

    private:
        typename D::const_iterator it_;
    };

    using value_type = D::value_type;
    explicit constexpr values(D& base) : begin_(base.begin()), end_(base.end()) {};

    constexpr const_iterator begin() {
        return const_iterator(begin_);
    }

    constexpr const_iterator end() {
        return const_iterator(end_);
    }

private:
    typename D::const_iterator begin_;
    typename D::const_iterator end_;
};

template <typename T>
values<T> values_view(T& cont) {
    return values<T>(cont);
}

template <typename D>
class reverse {
public:
    static_assert(IsContainer<D>, "There should be a container at the entrance.");
    static_assert(std::derived_from<typename D::const_iterator::iterator_category, std::bidirectional_iterator_tag>,
            "Reverse need a bidirectional iterator.");
    class const_iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using iterator_category = D::const_iterator::iterator_category;

        const_iterator(typename D::const_iterator it) : it_(it) {};
        const_iterator(const const_iterator&) = default;
        const_iterator& operator=(const const_iterator&) = default;
        bool operator==(const const_iterator& other) const = default;
        bool operator!=(const const_iterator& other) const = default;

        auto operator*() const {
            auto it = it_;
            return *(--it);
        }

        auto operator->() const {
            auto it = it_;
            return &(*(--it));
        }

        const_iterator& operator++() {
            --it_;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator copy = *this;
            --(*this);
            return copy;
        }

        const_iterator& operator--() {
            ++it_;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator copy = *this;
            ++(*this);
            return copy;
        }

    private:
        typename D::const_iterator it_;
    };

    using value_type = D::value_type;
    explicit constexpr reverse(D& base) : begin_(base.begin()), end_(base.end()) {};

    constexpr const_iterator begin() {
        return const_iterator(end_);
    }

    constexpr const_iterator end() {
        return const_iterator(begin_);
    }

private:
    typename D::const_iterator begin_;
    typename D::const_iterator end_;
};

template <typename T>
reverse<T> reverse_view(T& cont) {
    return reverse<T>(cont);
}

struct TakeHelper {
    TakeHelper(size_t argument) : argument_(argument) {};
    size_t argument_;
};

TakeHelper take_view(size_t argument) {
    return TakeHelper(argument);
}

template <typename T>
auto operator|(T&& left, TakeHelper right) {
    return take<std::remove_reference_t<T>>(left, right.argument_);
}

struct DropHelper {
    DropHelper(size_t argument) : argument_(argument) {};
    size_t argument_;
};

DropHelper drop_view(size_t argument) {
    return DropHelper(argument);
}

template <typename T>
auto operator|(T&& left, DropHelper right) {
    return drop<std::remove_reference_t<T>>(left, right.argument_);
}

template <typename T>
struct TransformHelper {
    TransformHelper(T argument) : argument_(argument) {};
    T argument_;
};

template <typename T>
TransformHelper<T> transform_view(T argument) {
    return TransformHelper(argument);
}

template <typename T, typename D>
auto operator|(T&& left, TransformHelper<D> right) {
    return transform<std::remove_reference_t<T>, D>(left, right.argument_);
}

template <typename T>
struct FilterHelper {
    FilterHelper(T argument) : argument_(argument) {};
    T argument_;
};

template <typename T>
FilterHelper<T> filter_view(T argument) {
    return FilterHelper(argument);
}

template <typename T, typename D>
auto operator|(T&& left, FilterHelper<D> right) {
    return filter<std::remove_reference_t<T>, D>(left, right.argument_);
}

struct ReverseHelper {};

ReverseHelper reverse_view() {
    return ReverseHelper();
}

template <typename T>
auto operator|(T&& left, ReverseHelper right) {
    return reverse<std::remove_reference_t<T>>(left);
}

struct KeysHelper {};

KeysHelper keys_view() {
    return KeysHelper();
}

template <typename T>
auto operator|(T&& left, KeysHelper right) {
    return keys<std::remove_reference_t<T>>(left);
}

struct ValuesHelper {};

ValuesHelper values_view() {
    return ValuesHelper();
}

template <typename T>
auto operator|(T&& left, ValuesHelper right) {
    return values<std::remove_reference_t<T>>(left);
}