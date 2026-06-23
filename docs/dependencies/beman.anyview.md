# beman.any_view

* [GitHub Repository](https://github.com/bemanproject/any_view/)

## TL;DR

This allows us to write cleaner and safer code.

## Description

``beman.any_view`` is a C++ library that provides a generic, type-erased view
interface for containers.

While the standard library's ``std::ranges`` provides powerful algorithms, it
relies heavily on complex, auto-generated types under the hood. This library
fixes a major structural gap in standard C++20 by providing a concrete,
definable type for views.

## Why do we need it?

Using ``any_view`` is highly beneficial for architecting clean, maintainable
systems. It solves several core limitations of standard ranges:

* **Type Erasure:** It allows us to provide strictly typed views without
  relying on ``auto``.
* **Branching Logic:** It enables returning completely different underlying
  view types (or empty views) from the same function, which is normally
  forbidden by the compiler when using ``auto``.
* **API Boundaries & Compilation:** Because the return type is explicitly
  known, you no longer have to define your complex view transformations
  entirely in header files. You can hide the implementation in ``.cpp`` files,
  reducing compilation times across the project.

## Why beman.any_view specifically?

This specific implementation strictly
follows [P3411R6](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/p3411r6.html),
which is the proposed standard for introducing type-erased views into C++26.
Using it ensures our codebase is forward-compatible and adheres to modern C++
design philosophies.

---

## Examples

### 1. Return Type Encapsulation

**The Standard C++20 Approach**

```c++
template <IsEntity T>
auto EntityDatabase::viewAll() const {
    const auto it = _entitiesByType.find(typeid(T));
    static const std::remove_cvref_t<decltype(it->second)> emptyMap;
    const auto& entities = it != _entitiesByType.end() ? it->second : emptyMap;

    return entities | std::views::values |
           std::views::transform([](IEntity* entity) { return static_cast<T*>(entity); });
}

```

**Current Pain Points:**

* The developer consuming the API has no idea what specific type this function
  returns without reading the implementation.
* Returning an "empty" state requires awkward workarounds, like a static helper
  map allocation.
* The visual noise makes the function's core intent hard to read.

**The ``beman.any_view`` Approach**

```c++
template <IsEntity T>
using EntityView = beman::any_view::any_view<T*, beman::any_view::any_view_options::forward, T*>;

template <IsEntity T>
EntityDatabase::EntityView<T> EntityDatabase::viewAll() const {
    const auto it = _entitiesByType.find(typeid(T));

    if (it == _entitiesByType.end()) {
        return EntityView<T>{};
    }
    return it->second | std::views::values |
           std::views::transform([](IEntity* entity) { return static_cast<T*>(entity); });
}

```

**Advantages:**

* **Explicit Contracts:** The return type ``EntityView<T>`` makes the API
  instantly understandable.
* **Type Safety:** The compiler strictly enforces that only an
  ``EntityView<T>``
  can be returned.
* **Clean Logic:** Returning an empty view is as simple as returning a
  default-constructed object.
* **Readability:** The business logic is isolated and easy to parse.

---

### 2. Argument Type Safety

**The Standard C++20 Approach**

```c++
void displayNumbers(std::ranges::view auto numbers) {
    for (int n : numbers) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
}

```

**Current Pain Points:**

* The function signature cannot enforce the exact type contained *inside* the
  view (e.g., ``int`` vs ``float``).
* Developers lose immediate context on what data structure the function expects
  to consume.

**The ``beman.any_view`` Approach**

```c++
void displayNumbers(beman::any_view::any_view<const int> numbers) {
    for (const int n : numbers) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
}

```

**Advantages:**

* **Immediate Clarity:** The argument type leaves no ambiguity about what data
  is being processed.
* **Strict Enforcement:** The function is completely type-safe; attempting to
  pass a view of a mismatched type will fail at compile time.
