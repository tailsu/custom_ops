// custom_ops.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "custom_ops.hpp"

template <class T>
void type_discovery()
{
 	int a = 5;
}

/*
	We aim for syntax like this:

	class A; class B;
	int CUSTOM_OP(const A& a, |++, const B& b)
	{
		return a.value() * 2 + b.as_int() * 3;
	}

	then use it like so:
	int main()
	{
		A a; B b;
		int val = a |++ b;
	}
*/

#define typeof BOOST_TYPEOF

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

BOOST_CUSTOM_OP(int, const A&, a, |, +, -, const B&, b)
{
	return a.value() * 2 + b.as_int() * 3;
}

BOOST_CUSTOM_OP(int, const A&, a, |, ~~, &, int&, f)
{
	return f;
}

BOOST_CUSTOM_OP(int, const A&, a, +, , -, int, x)
{
	return x;
}

int _tmain(int argc, _TCHAR* argv[])
{
	A a; B b;
	a.a = 5; b.b = 7;

	int val = a |+- b;
	cout << val << endl;

	//type_discovery<boost::custom_ops::reasonable_type_for_unary_operator_overload<const B&>::type>();

	int A =12;
	int val2 = a |~~& boost::ref(A);
	cout << val2 << endl;

	int nextVal = a +- boost::cref(A);

	vector a, b;

	vector c = a |- *b;
}

