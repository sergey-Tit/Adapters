
## Библиотека адаптеров для упрощенной работы с алгоритмами и контейнерами.

Зачастую стоит задача применения нескольких алгоритмов одновременно, например:

```cpp
std::vector<int> v = {1,2,3,4,5,6};
std::vector<int> result;

std::copy_if(v.begin(), v.end(), std::back_inserter(result), [](int i){return i % 2;});
std::transform(result.begin(), result.end(), result.begin(), [](int i){return i * i;});

for(int i : result)
    std::cout << i << " ";
```

Эту задачу можно решить более коротко и красиво, используя адаптеры:

```cpp
std::vector<int> v = {1,2,3,4,5,6};

auto removed = v | filter([](int i){return i % 2;});
auto result = removed | transform([](int i){return i * i;});

for(int i : result)
    std::cout << i << " ";
```

Или еще более коротко, использовав [конвейер, наподобие того как это принято в unix-системах](https://en.wikipedia.org/wiki/Pipeline_(Unix))

```cpp
std::vector<int> v = {1,2,3,4,5,6};

for(int i : v | filter([](int i){return i % 2;}) | transform([](int i){return i * i;}))
     std::cout << i << " ";
```

Еще одним значимым отличием такого подхода от изначального является то, что вычисления являются ленивыми, а создаваемые объекты не владеют массивом данных для решения данной задача. Подобный подход в частности применяется в классах [std::string_view](https://en.cppreference.com/w/cpp/string/basic_string_view) и [std::span](https://en.cppreference.com/w/cpp/container/span)

## Список адаптеров

Адаптеры применяются к контейнерам, и выдвигают собственные требования к ним, которые проверяются с помощью метода static_assert

* transform - изменяет значения элементов наподобие того как это делает алгоритм transform
* filter    - фильтрация по определенному признаку, признак передается в качестве аргумента
* take      - берет только N первых элементов
* drop      - пропускает N первых элементов
* reverse   - реверсия
* keys      - возвращает ключи для ассоциативных контейнеров
* values    - возвращает значения для ассоциативных контейнеров

1. Адаптеры, для их использования в `range-based for`, удовлетворяют ряду [требований](https://en.cppreference.com/w/cpp/language/range-for).

2. Данная библиотека представляет из себя упрощенную версию библиотеки std::ranges.
