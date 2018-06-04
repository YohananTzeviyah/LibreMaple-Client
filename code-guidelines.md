# C++ coding style

## Syntax

* **Just use clang-format**. There is a provided `.clang-format` file which
  will provide all of the information needed for clang-format to prettify any
  C++ code.
* Don't omit curly braces, even when they are optional (clang-format
  unfortunately cannot do this for you):
    * Bad:

        ```cpp
        if (condition)
            do_this;
        else
            do_this_instead;
        ```

    * Good:

        ```cpp
        if (condition) {
            do_this;
        } else {
            do_this_instead;
        }
        ```

* Tabs are 4 characters, and thus indentations are also 4 characters. There are
  heretic movements that try to make indentations 8 (or even 2!) characters
  deep, and that is akin to trying to define the value of PI to be 3.
* Use spaces. Never tabs.
* The preferred way to ease multiple indentation levels in a `switch` statement
  is to align the `switch` and its subordinate `case` labels in the same column
  instead of double-indenting the `case` labels:

    ```cpp
    switch (suffix) {
    case 'G':
    case 'g':
        mem <<= 30;
        break;
    case 'M':
    case 'm':
        mem <<= 20;
        break;
    case 'K':
    case 'k':
        mem <<= 10;
        /* fall through */
    default:
        break;
    }
    ```

* Don't put multiple statements on a single line unless you have something to
  hide:
    * Bad:

        ```cpp
        if (condition) do_this;
            do_something_everytime;
        ```

    * Good:

        ```cpp
        if (condition) {
            do_this;
        }

        do_something_everytime;
        ```

* Don't put multiple assignments on a single line either. Keep your coding
  style super simple. Avoid tricky expressions.
* Get a decent editor and don't leave whitespace at the end of lines.
* Use spaces. Never tabs.
* The maximum length (in terms of character widths) for each line is 79, and
  this is a strongly preferred limit.
* Statements longer than 79 columns will be broken into sensible chunks, unless
  exceeding 79 columns **significantly** increases readability and does not
  hide information. Descendants are indented an additional 4 spaces to the
  right. The same applies to function headers with a long argument list.
  However, never break user-visible strings, because that breaks the ability
  to grep for them.
* The other issue that always comes up in C++/C styling is the placement of
  braces. Unlike the indent size, there are few technical reasons to choose one
  placement strategy over the other, but the preferred way, as shown to us by
  the prophets Kernighan and Ritchie, is to put the opening brace last on the
  line, and put the closing brace first, thusly:

    ```cpp
    if (x) {
        do_y();
    }
    ```

  This applies to all non-function statement blocks (`if`, `switch`, `for`,
  `while`, `do`, `try`, `catch`):

    ```cpp
    switch (action) {
    case KOBJ_ADD:
        return "add";
    case KOBJ_REMOVE:
        return "remove";
    case KOBJ_CHANGE:
        return "change";
    default:
        return "none";
    }
    ```

  However, there is one special case, namely functions: they have the opening
  brace at the beginning of the next line, thus:

    ```cpp
    int function(int x)
    {
        // body of function
    }
    ```

  Heretic people all over the world have claimed that this inconsistency is
  ...well... inconsistent, but all right-thinking people know that (a) K&R are
  right and (b) K&R are right. Besides, functions are special anyway (you can't
  nest them in C++/C).

  Note that the closing brace is empty on a line of its own, except in the
  cases where it is followed by a continuation of the same statement, i.e. a
  `while` in a `do`-statement or an `else` in an `if`-statement, like this:

    ```cpp
    do {
        // body of do-loop
    } while (condition);

    // and

    if (x == y) {
        // ...
    } else if (x > y) {
        // ...
    } else {
        // ...
    }
    ```

  Also, note that this brace-placement also minimizes the number of empty (or
  almost empty) lines, without any loss of readability. Thus, as the supply of
  new-lines on your screen is not a renewable resource (think 25-line terminal
  screens here), you have more empty lines to put comments on.
* Use a space after these keywords: `if`, `switch`, `case`, `for`, `do`,
  `while`, `try`, `catch`, but **not** with `sizeof`, `typeid`, or `alignof`.
  E.g.,

    ```cpp
    s = sizeof(struct file);
    ```

  Do not add spaces around (inside) parenthesized expressions. This example is
  **bad**:

    ```cpp
    s = sizeof( struct file );
    ```

* When declaring reference/pointer data or a function that returns a
  reference/pointer type, the preferred use of `&`/`*` is adjacent to the type
  name and not adjacent to the data name or function name. Examples:

    ```cpp
    char* linux_banner;
    std::uint64_t memparse(std::string& ptr, std::string&& retptr);
    char* match_strdup(substring_t* s);
    ```

* Use one space around (on each side of) most binary and ternary operators,
  such as any of these:

    ```cpp
    =  +  -  <  >  *  /  %  |  &  ^  <=  >=  ==  !=  ?  :
    ```

  but no space after unary operators:

    ```cpp
    &  *  +  -  ~  !  sizeof  typeid  alignof
    ```

  no space before **nor** after the postfix increment and decrement unary
  operators:

    ```cpp
    ++  --
    ```

  and no space around the `.` and `->` structure member operators.

* For naming local variables, fields, functions/methods, `enum class` variants,
  etc., generally stick to all-lowercase snake case (underscores). Encoding the
  type of a function or variable into the name (so-called Hungarian notation)
  is brain damaged --- the compiler knows the types anyway and can check those,
  and it only confuses the programmer. No wonder Microsoft makes buggy
  programs.
* For naming `struct`ures and `class`es, use CamelCase.
* Comments are good, but there is also a danger of over-commenting. **Never**
  try to explain **how** your code works in a comment: it's much better to
  write the code so that the working is obvious, and it's a waste of time to
  explain badly written code.
* Generally, you want your comments to tell **what** your code does, not
  **how**. Also, try to avoid putting comments inside a function body: if the
  function is so complex that you need to separately comment parts of it, you
  should probably reduce its complexity by refactoring. You can make small
  comments to note or warn about something particularly clever (or ugly), but
  try to avoid excess. Instead, put the comments at the head of the function,
  telling people what it does, and possibly **why** it does it.
* Single-line comments are written in plain english, starting with two forward
  slashes and a space, and contain proper capitalization and punctuation.
* Multi-line comments are similar, and look like this:

    ```cpp
    /*
     * This is a multi-line comment.
     * It's got multiple lines.
     * It's readable.
     */
    ```

* Documentation comments (which should, with very few exceptions, be at the
  direct top of every function declaration and every `class`/`struct`
  definition) follow [Doxygen](https://www.stack.nl/~dimitri/doxygen/) style.
  That means starting the first line of a multiline comment with `/*!` or
  `/**`, or using a tight stack of single-line comments that start with `///`.
  Additionally, such comments are written in
  [Markdown](https://daringfireball.net/projects/markdown/) syntax:

    ```cpp
    /*!
     * This function does X, Y, and then Z.
     *
     * # Arguments
     *
     * - `foo` : Should satisfy `!foo.isBarred()`.
     * - `baz` : Byte to add to the end of the new `foo`.
     */
    void some_function(const Foo& foo, std::uint8_t baz) {
        // ...
    }
    ```

* Doc comments (which are on every function and method declaration) should
  indicate both what the function does, and also indicate if the function is
  pure or not.
* Try to avoid the use of macros (`constexpr` and templates should eliminate
  the need for them), but when you do have to use them, make sure that their
  names are in SHOUTING\_SNAKE\_CASE:

    ```cpp
    #define MY_MACRO "don't use macros, ya silly goose."
    ```

* Use brace initialization and initializer lists, and use them like array
  literals:

    ```cpp
    const std::tuple<std::uint64_t, std::string, bool> my_tuple{137, "1/137"s, true};
    ```

## Method

* **[The ISO C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines)**
  **rule supreme**. All hail.
* **Use static analysis tools**. These include things like Cppcheck,
  clang-tidy, Oink, etc.
* Heed compiler warnings, including all those generated by the warning flags
  passed in automatically at compile time.
* **Use runtime analysis tools**. These include things like AddressSanitizer,
  LeakSanitizer, ThreadSanitizer, MemorySanitizer, UndefinedBehaviorSanitizer,
  valgrind/cachegrind/callgrind, etc.
* Do not use bare pointers (`*`). Like, really, don't. Instead, use one of the
  following, **preferrably the closest one to the top of the list**:
    * References (an r-value reference `&&` if it makes sense at all to use,
      otherwise just an ordinary l-value reference `&`).
    * Optionals, viz. `std::optional<T>`. This is for when you may or may *not*
      have a value, **strongly** preferable to using a nullable pointer.
    * Spans, viz. `gsl::span<T>`. This only makes sense when you have a pointer
      into some data *and* a length associated with it.
    * Unique pointers, viz. `std::unique_ptr<T>`.
    * Shared pointers, viz. `std::shared_ptr<T>` (or `std::weak_ptr<T>`, as the
      case may be). Only use these if you are definitely sharing this pointer
      and *require* the reference counting, since it comes with considerable
      overhead.
    * Actual raw pointers `*`, **ONLY if you really have to and you know**
      **exactly what you're doing**. In the case that you **have to** resort to
      this, use `gsl::not_null<T>` and `gsl::owner<T>` whenever appropriate.
* Do not use bare invocations of `new` and `delete`. Like, no, really, *don't*.
  See the guidelines above on the use of pointer-like objects.
* When using "smart pointers" (`std::unique_ptr<T>`, `std::shared_ptr<T>`, and
  `std::weak_ptr<T>`), wrap them as being guaranteed non-null
  (e.g. `gsl::not_null<std::unique_ptr<T>>`), and if nullability is required,
  further wrap that into an optional
  (e.g. `std::optional<gsl::not_null<std::unique_ptr<T>>>`). You may want to
  create `using` aliases for these as necessary.
* When dealing with values that you may or may not actually have (especially
  when returning such things from functions), prefer the use of
  `std::optional<T>`.
* Use `gsl::span<T>` (respectively, `gsl::span<const T>`) instead of a
  free-standing `T*` (respectively `const T*`) for which you have the length
  value. So, replace functions like:

    ```cpp
    void read_into(int* buffer, size_t buffer_size);
    ```

  with:

    ```cpp
    void read_into(gsl::span<int> buffer);
    ```

* **Do not use C-style strings**. Use `std::string`, and `std::string_view`
  when appropriate.
* Use the `s` suffix for string literals that are meant to be `std::string`s:

    ```cpp
    // Unless you really meant `const char[19]`, this is wrong:
    const auto my_string = "string-a-ring-ding";
    // Good, assuming you wanted a `std::string`:
    const auto my_string = "string-a-ring-ding"s;
    ```

* When required to deal with C-style strings (interfacing with other code), use
  `gsl::not_null<gsl::zstring>`.
* Use these diagrams when considering how to pass parameters (the first is just
  a simplified version of the second):

    ![Param passing (simple)](https://raw.githubusercontent.com/isocpp/CppCoreGuidelines/master/param-passing-normal.png)

    ![Param passing (advanced)](https://raw.githubusercontent.com/isocpp/CppCoreGuidelines/master/param-passing-advanced.png)

* Do not transfer ownership using references or --- god forbid --- raw
  pointers. This most often applies when returning something from a
  function/method. Instead, transfer *by value* (forcing move semantics using
  `std::move` when necessary), or use smart pointers for that purpose:

    ```cpp
    // BAD!!!
    &vector<double> compute(/* args... */)
    {
        vector<double> res(10000);
        // ...
        return res;
    }

    // Ok.
    vector<double> compute(/* args... */)
    {
        vector<double> res(10000);
        // ...
        return res;
    }
    ```

* **Use the standard library**. Use the standard algorithms. Use the standard
  data structures. Use the standard... etc. Also use the GSL.
* Use `std::array` for sequences of data. If not, use `std::vector`. Under no
  circumstances should you resort to the use of `std::forward_list` or
  `std::list`, unless you **REALLY** know what you're doing.
* **Use the `unordered_` versions of set/map/multiset/multimap containers**,
  unless you know what you're doing (e.g. you do need to persist a sorted
  order).
* Make things pure whenever possible, and when a function/method is pure, it
  should (of course) be `const` and also indicate that it is pure within its
  doc comment. Functions/methods that are not pure should indicate that they
  are not pure in their doc comments.
* **Use `const`**. Use it. Use it again. Wherever you can. Of course only use
  it when applicable, and **respect const-correctness**. Use it some more. Use
  it a lot. Make sure you're not accidentally preventing possible moves (that
  would be desirable).
* Use `constexpr` *wherever applicable*, including on functions.
* All functions/methods that are incapable of throwing an exception should be
  marked as `noexcept`, and even more importantly, *functions/methods that*
  *make use of exceptions for error handling at all must be marked*
  `noexcept(false)`.
* Do **not** use exceptions in situations where you know that the
  exception-throwing case is actually not that rare (i.e. it is not an
  exceptional case at all).
* Avoid using exceptions, just in general. If you are forced to interface with
  code that *does* use exceptions, there are three approaches that can be
  suitable depending on the context:
    1. Check all preconditions of the exception-throwing function(s) that you
       are calling and make sure that none can be violated before you call the
       function(s). This is usually(?) an acceptable approach so long as the
       preconditions are more than rarely violated and the
       precondition-checking is relatively cheap. This has the advantage of
       allowing your function to be `noexcept` while also not using any `try`
       machinery yourself.
    2. Wrap exception-throwing functions with `try` blocks and handle the
       exceptions yourself. This is acceptable whenever you expect that you
       will rarely or never actually have to `catch` anything (as the catching
       is rather expensive). Such an approach also allows for `noexcept` to be
       specified for your function, assuming of course that you do not
       re-throw or anything like that.
    3. Consider the exceptions (possibly) thrown by the exception-throwing
       interface to be part of the exception contract of your function. Such
       an approach requires your function to be `noexcept(false)`, and
       description of exception-throwing contracts should of course appear in
       the doc comment of your function.
* Instead of using exceptions, you can implement "failure modes" for your
  functions/methods. Things that make this easy are `std::optional<T>` and
  `std::variant<T...>`. NOTE: It may be wise to adopt the `expected` library
  [from here](https://github.com/TartanLlama/expected). Optionals can
  represent a success (useful value) or failure (nothing at all). Variants can
  be used to represent a success (the first type parameter, a useful value) or
  failure (the second type parameter, a useful bit of info about what went
  wrong):

    ```cpp
    // Using an idiom from Rust.
    using result<T, E> = std::variant<T, E>;

    enum class FooError {
        bad_file,
        no_such_player,
        not_yet_initialized,
        incorrect_challenge
    }

    result<Foo, FooError> get_foo_may_fail() const noexcept
    {
        const auto foo = // ...
        // ...
        if (getting_foo_failed) {
            return {relevant_foo_error};
        } else {
            return {foo};
        }
    }

    result<Foo, std::string> get_foo_may_fail_with_err_msg() const noexcept
    {
        const auto foo = // ...
        // ...
        if (getting_foo_failed) {
            return {relevant_err_msg};
        } else {
            return {foo};
        }
    }

    std::optional<Foo> get_foo_maybe() const noexcept
    {
        const auto foo = // ...
        // ...
        if (getting_foo_failed) {
            return {};
        } else {
            return {foo};
        }
    }
    ```

* Avoid using bare `for` and `while` blocks; use iterator-style `for` blocks
  (`for (const auto item : container) { ... }`) instead, or *even better*, use
  standard algorithms and container methods like `for_each`, `find_if`, etc.
* Limit the scope of your variables to be *as small as possible*. Even better,
  don't declare variables at all and simply place the value where it's needed
  (when applicable).
* Use `if`-statement init statements, when applicable, to limit the scope of
  your variables as much as possible (also applies to `switch`):

    ```cpp
    if (auto foo = get_foo(); foo.isBarred()) {
        // ...
    }
    ```

* Be tidy with resources (memory, file handles, ...). Always clean up after
  yourself, and do so as soon as is applicable. Make sure that you do this
  automatically as often as possible through the use of destructors, so that
  no one can forget.
* Performance matters.
* In general avoid out params. Use `std::tuple`, `std::variant`, etc. or your
  own structure/class instead. If you do use out params, refer to the relevant
  diagrams for parameter passing.
* Keep all code as type-safe as possible, and when forced to interact with
  poor/weak typing in some other code, try to encapsulate it so that you don't
  shoot anyone (including yourself) in the foot.
* Avoid using casts. This is part of type-safety. Templates can help.
* To convert from one numeric type to another that doesn't lose precision, use
  brace initialization like so (`static_cast` is also acceptable):

    ```cpp
    const std::int32_t x = 15;
    const std::int64_t y{x};
    ```

* To convert from one numeric type to another that possibly loses
  precision, consider using `gsl::narrow_cast`, or even
  `math::saturating_cast` if saturation is the appropriate semantics for
  "overflow".
* If you *have* to use a cast that is not one of the aforementioned, use
  `static_cast`, but be wary of possible undefined behavior when using it on
  numeric types.
* **Do not use `const_cast` to cast away const-ness**.
* Do not use `reinterpret_cast` nor C-style casts (e.g. `(int) some_float`)
  unless you are a super wizard whom'st knows **exactly** what they are doing.
* If you find that for some reason you need some kind of class
  hierarchy/subtyping, **keep hierarchies as shallow as you possibly can**, and
  **use pure virtual/abstract classes as interfaces rather than extending**
  **other, less general kinds of classes**. Use pure virt class/interfaces as
  if they are typeclasses/traits.
* If you do find yourself writing a pure virt/abstract class, include a
  (not-pure-)virtual destructor, and then do *not* include a virtual destructor
  in implementing/derived classes; those classes should inherit the virtual
  destructor. Also, make sure that as many of the other methods as possible are
  *pure* virtual, not just virtual.
* Try to avoid the use of macros; `constexpr` and templates should eliminate
  the need for them.
* Use `auto`. Unless the type name is *particularly* short, or the type matters
  a **lot** to readers of the code and is non-obvious, `auto` should be
  preferred:

    ```cpp
    for (auto&& [k, v] : consumable_map) {
        do_thing(k);
    }
    ```

* When dealing with integral types, prefer to use fixed width integer types
  like `std::uint64_t`, `std::int32_t`, etc.
* Use `std::variant`, not `union`s. NOTE: It may be wise to adopt the scelta
  library, [from here](https://github.com/SuperV1234/scelta).
* Use structured bindings, when applicable.
* Don't ask the compiler to inline your functions. It knows how to do that. If
  you really think that you know what you're doing, profile it.
* Don't give the compiler other "optimization hints" (primarily through the use
  of attributes). The compiler knows how to optimize things.
* Use `enum class`, not `enum`.
* If you require the use of bare null pointers (you shouldn't since you
  shouldn't be using raw pointers at all, and even if you had a reason, you
  should use `nullable_ptr<T>`, but it's possible that you're interfacing with
  someone else's code that uses nasty raw nullable pointers), use `nullptr`.
* Use `if constexpr` for conditional compilation when applicable.
* What cannot be checked at compile time should be checkable at run time.
* Keep the number of function/method parameters low. If you find yourself
  writing a function with lots of parameters, you probably just need some kind
  of abstraction (a structure/class, perhaps?).
* A function should perform a single logical operation. Keep functions short
  and simple.
* **Only** ever return a `T*` to indicate a position, unless you **really**
  know what you're doing, as explained above. If you want to return a view,
  use a reference (`&`) or special type like `std::string_view`, and if you
  want to transfer ownership, use `std::unique_ptr<T>`.
* Use lambdas for complex initialization, especially of `const` variables (this
  is sometimes called an "IIFE", immediately invoked function expression, due
  to Javascript). This allows more liberal use of `const`, allows for shorter
  lifetimes of variables, and can generate more efficient code:

    ```cpp
    // BAD!!!
    widget x;  // Should be const, but:
    for (auto i = 2; i <= N; ++i) {         // This could be some
        x += some_obj.do_something_with(i); // arbitrarily long code
    }                                       // needed to initialize `x`.
    // From here onward, `x` *should* be `const`, but we can't say so in code
    // in this style.

    // Good.
    const widget x = [&]{
        widget val;                               // Assuming that widget has a default constructor.
        for (auto i = 2; i <= N; ++i) {           // This could be some
            val += some_obj.do_something_with(i); // arbitrarily long code
        }                                         // needed to initialize `x`.
        return val;
    }();
    ```

* Use a lambda when a function won't do (to capture local variables, or to
  write a local function).
* Explicitly list exactly those and only those captures that your lambda
  needs, when possible. I.e., generally avoid `[&]` and `[=]`.
* Prefer capturing by reference in lambdas that will be used locally.
* Avoid capturing by reference in lambdas that will be used nonlocally,
  including returned, stored on the heap, or passed to another thread.
* Where there is a choice, prefer default arguments over overloading.
* Use `struct` whenever all members are `public`. Otherwise, use `class`.
* Don't return a `T&&`.
* `int` is the return type for `main()`. Not `void`. Not anything else. `int`.
* Return `T&` from assignment operators.
* Don't use var-args for parameters. Instead, use overloading, or variadic
  templates, or variant arguments, or `initializer_list` (for homogeneous
  args).
* Never use `malloc` or `free` (unless some archaic API forces your hand, in
  which case you can encapsulate it).
* Minimize allocations in general. Use the stack (don't overflow it, though,
  of course).
* `std::regex` is good, but please don't use regex for simple things or when
  you really just need a more complex hand-written parser or a parser library.
* Use `gsl::string_span` for when you need something *like* a
  `std::string_view`, but want to mutate the string being viewed.
* When working with time, use `std::chrono`.
* `using namespace std;` is bad, and you should feel bad. "Use" as needed.
  Things that originate from the standard library should have the marker
  `std::`, and likewise for other foreign namespaces.
* Pure functions should have the `[[nodiscard]]` attribute. Other functions may
  have a use for the attribute as well; use your judgement.
* Don't use `std::endl`. Use `'\n'` instead.
* RAII.
