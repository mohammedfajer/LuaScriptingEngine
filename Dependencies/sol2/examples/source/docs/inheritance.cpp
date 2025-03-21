#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

struct A {
	int a = 10;
	virtual int call() {
		return 0;
	}
	virtual ~A() {
	}
};
struct B : A {
	int b = 11;
	virtual int call() override {
		return 20;
	}
};

int main(int, char*[]) {

	sol::state lua;

	lua.new_usertype<A>("A", "call", &A::call);

	lua.new_usertype<B>("B",
	     "call",
	     &B::call,
	     sol::base_classes,
	     sol::bases<A>());

	return 0;
}
