# Contributing

## Development Standards

### C++23 & Modern Practices

We use the **C++23** standard. All code must reflect modern C++ paradigms.
Key principles include:

* **RAII:** Resource Acquisition Is Initialization (RAII) is mandatory. Avoid manual
  memory management (`new`/`delete`).
* **Smart Pointers:** Use `std::unique_ptr` and `std::shared_ptr` exclusively.
* **Type Safety:** Prefer `std::expected`, `std::optional`, and strong typing
  over error codes or `void*`.
* **STL & Ranges:** Use the `<ranges>` and `<algorithm>` libraries to write
  expressive, declarative code.
* **Const-Correctness:** Mark variables and methods as `const` whenever
  possible to ensure immutability.
* **Structured Bindings:** Use structured bindings for clarity when unpacking
  tuples or pairs.
* **Modern Features:** Leverage C++23 features to improve code clarity and
  safety. Use `std::string_view` for read-only string parameters, `std::span`
  for array views, and `std::format` for string formatting.

### Coding Style

We follow the **Google C++ Style Guide** with specific **Epitech-style tweaks**:

* **Naming:** `PascalCase` for classes, `camelCase` for methods/functions, and
  `camelCase` for private members with a leading underscore (e.g.,
  `_variableName`).
* **Indentation:** four spaces rather than Google’s two spaces.
* **Header Guards:** Use `#pragma once` guards.
* **Clang-Format:** Always run `clang-format` before committing. A
  `.clang-format` file is provided in the root directory.

---

## Branching Model (Gitflow)

We strictly follow the **Gitflow** workflow. Please ensure you are working on
the correct branch.

| Branch     | Purpose                                                                     |
|:-----------|:----------------------------------------------------------------------------|
| `main`     | Production-ready code only. This reflects the current stable release.       |
| `develop`  | Integration branch for features. This is where the "latest" dev code lives. |
| `feature/` | Used for specific features (e.g., `feature/modern-ui`). Base off `develop`. |
| `release/` | Preparation for a new submission/milestone.                                 |
| `hotfix/`  | Urgent fixes for the `main` branch.                                         |

---

## Commit Message Format

We follow the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/)
specification.

**Format:** `<type>(<scope>): <description>`

### Allowed Types

* `feat`: A new feature (e.g., adding a new material type).
* `fix`: A bug fix (e.g., fixing a ray-intersection math error).
* `build`: Changes to the build system (CMake, dependencies).
* `chore`: Maintenance tasks (updating `.gitignore`, CI scripts).
* `docs`: Documentation only changes.
* `style`: Code style/formatting (non-functional changes).
* `refactor`: Code change that neither fixes a bug nor adds a feature.
* `perf`: Performance optimizations (e.g., optimizing the BVH traversal).
* `test`: Adding or correcting tests (Criterion, GoogleTest).

**Example:**
`feat(ui): add layout editor panel`

---

> [!NOTE]
> Any code that breaks the build or fails to meet the C++23 standard
> requirements will be requested for revision before merging.
