(Almost) Custom operators
=================================

Introduction
------------

Long story cut short, this library allows for syntax sugar like this:

```
class A; class B;
...
int val = A /~+!- B;
```

where the `/~+!-` part is our custom operator. These custom operators consist
of any overloadable binary operator plus an arbitrary string of unary operators.

The above operator could be defined like this:
```
BOOST_CUSTOM_OP(int, const A&, a, /, ~+!, -, const B&, b)
{
	return a.value() * 3 + b.as_int() * 2;
}
```
This defines an operator that returns an int and takes two const references named `a` and `b`.
The operator string ( `/~+!-` in our example) is defines as the binary op `/`, the intermediate
string `~+!` and the last operator `-`. This splitting is required due to implementation issues.
When used, the library defines a unary operator that takes the second type as an argument.
This implies all kinds of restrictions on the second type; for example it
can't be a fundamental type (`int`, `double`, `some_class*`, etc.)(see notes)
and also can't have a unary operator like the rightmost operator in the operator string.

Synopsis
--------

```
BOOST_CUSTOM_OP(rettype, param1type, param1name, binop, ops, lastop, param2type, param2name)
{
	// user implementation
}
```

- `rettype` - the return type of the operator
- `param1type` - the type of the left-hand parameter
- `param1name` - the name of the left-hand parameter
- `binop` - the first character of the operator string, most C++ binary operators
- `ops` - the intermediate operators, any string of valid C++ prefix unary operators
- `lastop` - the last operator in the string, any valid C++ prefix unary operator
- `param2type` - the type of the right-hand parameter
- `param2name` - the name of the right-hand parameter

Notes
-----

- The library depends on Boost.Typeof
- if you want the second type parameter to be a fundamental type, you must
wrap it with `boost::ref()` or `boost::cref()` at the call site. The internally
used reference wrapper type isn't `boost::reference_wrapper<T>`, so, the above
limitations on unary operators aren't exposed.
- prefix `++` and `--` are supported as well. Mind that when using them you'll have to
treat them like single operators, i.e. to define the operator `|++*--`, you'll write:
```
BOOST_CUSTOM_OP(..., |, ++*, --, ...)
```
- operator comma (`,`) can be used like this:
```
#define COMMA ,
BOOST_CUSTOM_OP(..., COMMA, +*, -, ...)
```

	The library defines the more verbose `BOOST_CUSTOM_OP_COMMA` for this purpose
- supported prefix unary operators are `+` `-` `&` `*` `++` `--` `!` `~`

A full example
--------------

```
#include "custom_ops.hpp"

struct A
{
	int value() const { return a; }
	int a;
};

struct B
{
	int as_int() const { return b; }
	int b;
};

BOOST_CUSTOM_OP(int, const A&, a, /, ~+, -, const B&, b)
{
	return a.value() * 2 + b.as_int() * 3;
}

int main()
{
	A a; B b;
	a.a = 5; b.b = 7;

	int val = a /~+- b;
	cout << val << endl;
}
```