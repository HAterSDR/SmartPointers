# Project 2: $\mathrm{Smart Pointer}$

> SJTU CS1958-01 2026Fall 第二次大作业

---

**前排提醒：本作业禁止使用 `<memory>` 库！**

开始项目前，建议先阅读[智能指针：从入门到入土](./Intro.md)

本项目要实现的内容是一个类模板，可能有类似于`template <typename T>`的语句，你可以在理解的时候将`T`视作某一种特殊的类型名称（或者视为`int`）。根据这一规则，被包装的原始指针的类型就是`T*`。

## `Unique_ptr`

你需要构造的`unique_ptr`类模板的要求包括：

- 默认构造函数，内部指针默认应当为`nullptr`：
  
  ```cpp
  UniquePtr<int> ptr;
  ```

- 能够支持将普通指针**显式**转换成`unique_ptr`的构造函数：
  
  ```cpp
  UniquePtr<int> ptr{new int{10}};
  ```

- `unique_ptr`间的移动构造函数，并禁止拷贝构造函数的存在，注意所有的移动构造/移动赋值函数都需要有 `noexcept` 声明 (why?)，后续所有实现中这句话也生效：
  
  ```cpp
  UniquePtr<int> ptr2{ptr1};  // compile error!
  ```

- 在类外编写类似于`std::make_unique` 的函数模版，要求能够支持对任意类型、任意数量的参数的`unique_ptr`构造，且能触发`UniquePtr`类的移动构造函数：
  
  ```cpp
  UniquePtr<int> ptr{make_unique<int>(10)};
  UniquePtr<std::vector<int>> ptr2{make_unique<std::vector<int>>(2,3)};
  ```

- 重载运算符`operator=`，实现`unique_ptr`间的移动赋值并禁止拷贝赋值：
  
  ```cpp
  UniquePtr<int> ptr1{new int{10}};
  UniquePtr<int> ptr2{new int{11}};
  ptr2 = ptr1;  // compile error!
  ```

- 析构函数 `~UniquePtr()` 删除动态指针并将其置为 `nullptr`（思考：为什么要置空）

- 实现 `get()` 函数用以返回存储在类中的原始指针：
  
  ```cpp
   UniquePtr<int> ptr{new int{10}};
   std::cout << ptr.get() << std::endl;  // output: raw pointer stored in the class
  ```

- 重载类型转换符 `bool()` 来判断指针是否为空，非空返回 true，空返回 false，注意这种判断不应支持隐式的转换：
  
  ```cpp
   UniquePtr<std::string> ptr{new std::string{"hello"}};
   std::cout << bool(ptr) << std::endl;  // output: true
  ```

- 重载解引用运算符 `operator*`，实现与普通指针相同的解引用功能：
  
  ```cpp
   UniquePtr<int> ptr{new int{10}};
   std::cout << *ptr << std::endl;  // output: 10
  ```

- 重载间接引用运算符 `operator->`，实现与普通指针相同的间接引用功能：
  
  ```cpp
   UniquePtr<std::string> ptr{new std::string{"hello"}};
   std::cout << ptr->length() << std::endl;  // output: 5
  ```

- 实现`reset()`函数，包括以下两种参数形式：
  
  1. `void reset()` 删除原有指针并分配 `nullptr` 给它
  2. `void reset(T* p)` 删除原有指针并分配 `p` 给它
  
  ```cpp
  UniquePtr<std::string> ptr{new std::string{"hello"}};
  ptr.reset(new std::string{"nice"});
  std::cout << *ptr << std::endl;  // output: nice
  
  // NOTE: It should also support:
  UniquePtr<int> p(new int(5));
  p.reset(p.get()); // with NO operation (which is defined in C++23)
  ```

- 实现`release`函数，让对象脱离`unique_ptr`的管理，并返回一个指向原对象的普通指针（交给使用者进行内存管理）。在此期间，`unique_ptr`类不删除原有指针。
  
  ```cpp
  UniquePtr<double> ptr{new double{1.567}};
  double *tmp{ptr.release()};
  std::cout << *tmp << std::endl;  // output: 1.567
  delete tmp;                      // manual deletion
  ```

## `Shared_ptr`

**在提交 `Shared_ptr.hpp` 时，一定要将你自己写的 `Class UniquePtr` 具体实现附在代码前面！！！**

> 在 `Shared_ptr` 及 `Weak_ptr` 的模板中，部分关键字（例如 `const`、`explicit`、`noexcept`）都已经被省略。请根据 `Unique_ptr` 的经验以及对函数效果的理解，自行补充这些关键字。

`shared_ptr` 中的要求包括：

- 默认构造函数，内部指针默认应当为`nullptr`：
  
  ```cpp
  SharedPtr<int> ptr;
  ```

- 实现能够支持将普通指针**显式**转换成 `shared_ptr` 的构造函数和将无论是**左值**还是**右值**的 `unique_ptr` 转换成 `shared_ptr` 的构造函数：
  
  ```cpp
  UniquePtr<int> ptr{new int{1}};
  SharedPtr<int> sp1{new int{10}};
  SharedPtr<int> sp2{ptr};
  ```

- 实现 `shared_ptr` 间的移动构造函数与拷贝构造函数：
  
  ```cpp
  SharedPtr<int> ptr2{ptr1};  // compile success
  ```

- 在类外编写类似于 `std::make_shared` 的函数模版，要求能够支持对任意类型、任意数量的参数的`shared_ptr`构造，且能触发`SharedPtr`类的移动构造函数：
  
  ```cpp
   SharedPtr<int> ptr{make_shared<int>(10)};
  ```

- 析构函数 `~SharedPtr()` 删除动态指针并将其置为 `nullptr`；

- 实现 `use_count()` 函数用以返回指向当前的对象的所有 `shared_ptr` 实例数：
  
  ```cpp
   SharedPtr<int> ptr1{make_shared<int>(10)};
   std::cout << ptr1.use_count() << std::endl;  // output: 1
   SharedPtr<int> ptr2{ptr1};
   std::cout << ptr1.use_count() << std::endl;  // output: 2
   std::cout << ptr2.use_count() << std::endl;  // output: 2
  ```

- 实现 `get()` 函数用以返回存储在类中的原始指针：
  
  ```cpp
    SharedPtr<int> ptr{new int{10}};
    std::cout << ptr.get() << std::endl;  // output: raw pointer of the class
  ```

- 重载类型转换符 `bool()` 来判断指针是否为空，非空返回 true，空返回 false：
  
  ```cpp
   SharedPtr<std::string> ptr{new std::string{"hello"}};
   std::cout << bool(ptr) << std::endl;  // output: 1 
  ```

- 重载解引用运算符 `operator*`，实现与普通指针相同的解引用功能：
  
  ```cpp
   SharedPtr<int> ptr{new int{10}};
   std::cout << *ptr << std::endl;  // output: 10
  ```

- 重载间接引用运算符 `operator->`，实现与普通指针相同的间接引用功能：
  
  ```cpp
   SharedPtr<std::string> ptr{new std::string{"hello"}};
   std::cout << ptr->length() << std::endl;  // output: 5
  ```

- 重载运算符 `operator=`，实现 `shared_ptr` 间的移动赋值与拷贝赋值：
  
  ```cpp
   SharedPtr<int> ptr1{new int{10}};
   SharedPtr<int> ptr2{new int{11}};
   ptr2 = ptr1;
  ```

- 实现 `reset()` 函数，包括实现以下两种参数形式：
  
  1. `void reset()` 删除原有指针并分配 `nullptr` 给它
  2. `void reset(T* p)` 删除原有指针并分配 `p` 给它
  
  ```cpp
   SharedPtr<std::string> ptr{new std::string{"hello"}};
   ptr.reset(new std::string{"nice"});
   std::cout << *ptr << std::endl;  // output: nice
  
   // NOTE: It should also support:
  SharedPtr<int> p(new int(5));
  p.reset(p.get()); // with NO operation (which is defined in C++23)
  ```

## `Weak_ptr`

**在提交 `Weak_ptr.hpp` 时，一定要将你自己写的`Class UniquePtr` 和 `Class SharedPtr` 具体实现附在代码前面！！！**

为此，你需要构造的 `weak_ptr` 模版类的要求包括：

- 默认构造函数，内部指针默认应当为`nullptr`：
  
  ```cpp
  WeakPtr<int> wp;
  ```

- 能够支持将 `shared_ptr` 转换成 `weak_ptr` 的构造函数：
  
  ```cpp
  SharedPtr<int> sp{new int{10}};
  WeakPtr<int> wp{sp};
  ```

- 移动构造函数与拷贝构造函数，注意在拷贝构造中可以通过 `shared_ptr` 构造 `weak_ptr`，自赋值/自移动必须安全，被移动构造和移动赋值的源对象都应当 `expired()` ，`expired()` 的要求详见下文：
  
  ```cpp
  WeakPtr<int> wp1{sp};
  WeakPtr<int> wp2{wp1};             // compile success
  WeakPtr<int> wp2{std::move(wp1)};  // compile success
  ```

- 拥有析构函数 `~WeakPtr()` 删除动态指针并将其置为 `nullptr`

- 重载运算符 `operator=`，实现移动赋值与拷贝赋值，注意在拷贝赋值中可以将 `shared_ptr` 赋给 `weak_ptr`
  
  ```cpp
  WeakPtr<int> wp1, wp2;
  SharedPtr<int> sp{new int{10}};
  wp1 = sp;   // compile success
  wp2 = wp1;  // compile success
  WeakPtr<int> wp1{sp}, wp2;
  wp2 = std::move(wp1);  // compile success
  ```

- 重载类型转换符 `bool()` 来判断指针是否为空，非空返回 true，空返回 false：
  
  ```cpp
   WeakPtr<int> wp;
   SharedPtr<int> sp{new int{10}};
   wp = sp;
   std::cout << bool(wp) << std::endl;  // output: 1
  ```

- 实现 `use_count()` 函数，用以返回指向当前的对象的所有 `shared_ptr` 实例数

- 实现`lock()`函数用以转换`weak_ptr`为`shared_ptr`，注意这会产生一个**新的** `shared_ptr`，引发计数器变化。如果对象已被删除，则返回一个空的 `shared_ptr`：
  
  ```cpp
   WeakPtr<int> wp{sp};
   SharedPtr<int> sp2 = wp.lock();
   if(sp2) {
       std::cout << *sp2 << std::endl;  // Use the object safely
   }
  ```

- 实现 `expired()` 函数用以检查 `weak_ptr` 所指向的**对象**是否已被删除，返回 `true` 表示被删除，返回 `false`表示没有被删除：
  
  ```cpp
   WeakPtr<int> wp{sp};
   if(!wp.expired()) {
       SharedPtr<int> sp2 = wp.lock();
       // Use sp2...
   }
  ```

- 实现 `reset()` 函数，删除原有指针并分配 `nullptr` 给它：
  
  ```cpp
  WeakPtr<int> wp{sp};
  wp.reset();  // wp no longer references anything
  ```

- 实现 `swap()` 函数，$O(1)$ 交换两个 `weak_ptr` 对象而不修改对象资源：
  
  ```cpp
  WeakPtr<int> wp1{sp1}, wp2{sp2};
  wp1.swap(wp2);  // wp1 now watches sp2's object, wp2 watches sp1's
  ```

---

## 注意事项

1. 请事先阅读下发的材料，里面包含了绝大多数本次大作业当中需要用到的知识点，以及对三种智能指针功能的简要介绍，能够对本次大作业的背景有一个初步的了解；
2. 开始写代码前一定要先进行程序设计，思考应当以什么结构框架实现各种功能，以避免不必要的代码堆砌或整体重构 （尤其是`shared_ptr`与`weak_ptr`）
3. 所有的自赋值和自移动赋值必须保证安全。
4. 注意函数实现中的细节，譬如 `const`、`noexpect`、`explicit`等（这些请自行查询资料了解），有时需要将这些关键词添加到适当函数处使智能指针可以正常运行，并且注意这些**将会被涵盖在OJ上的测试当中（若有细节错误会扣部分分）**，也都有可能成为`Code Review` 中考察的对象。

---

## Bonus

以下挑战完全是可选的，不会被测试或评分：

- 为整个项目添加线程安全相关的内容

- 支持数组并使其与自定义删除器兼容。例如：
  
  ```cpp
  void test10(double& score) {
      // test weak_ptr with arrays
      sharedptr<int[]> sp1{new int[5]{1, 2, 3, 4, 5}};
      weakptr<int[]> wp1(sp1);
  
      // test that after move, the moved-from weak_ptr is expired
      weakptr<int[]> wp2(std::move(wp1));
      // a moved-from weak_ptr should be expired
      if (!wp1.expired()) {
          result = false;
          return;
      }
  
      // test that the moved-to weak_ptr works correctly
      sharedptr<int[]> sp2 = wp2.lock();
      if (!sp2 || sp2[0] != 1 || sp2[1] != 2) {
          result = false;
          return;
      }
  
      sp1.reset();
      wp2.reset();  // need to reset wp2 explicitly
      if (wp2.lock()) {
          result = false;
          return;
      }
  
      score += 0.055;
  }
  ```

- 添加 `owner_before()` 成员函数以实现关联容器中的一致排序。

- 在 `if` 条件中使用自定义智能指针。如果智能指针包含 `nullptr` ，则该条件返回 `false` 否则返回 `true`。
  
  ```cpp
  UniquePtr<double> ptr{new double{1.567}};
    if(ptr)  // => true
             // something
  ptr.reset();
  if(ptr)  // => false
           // some other thing
  ```

使其对 `UniquePtr`, `SharedPtr` and `WeakPtr` 都生效。

---

## 须知

### 截止时间

2026年10月28日18:30前将本项目提交至ACMOJ即可。

### 编译与运行

本项目需要自行构建样例的 `cpp` 文件用以调试，善用 `valgrind`，这会有所帮助。

### 提交方式

请各位自行测试无误后将对应的 `hpp` 文件提交到 ACMOJ 对应渠道即可.

### 评分规则

- `Unique_ptr` $20$ 分；

- `Shared_ptr` $35$ 分；

- `Weak_ptr` $30$ 分；

- Code Review $20$分。

满分 $100$ 分，超过 $100$ 分的也以 $100$ 分计。

## Acknowledgement

特别感谢 `Amirkabir University of Technology 1400-2 —— Advanced Programming Course Project 4 'Smart Pointers'` .

感谢 2022 级汪畋宇学长创建了这个项目。

如有问题请联系本项目的发布者 `duanzeqian`, 他的邮箱地址是: `tonyduan@sjtu.edu.cn`。
