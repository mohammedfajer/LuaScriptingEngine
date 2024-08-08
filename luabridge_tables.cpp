//#include <iostream>
//#include <lua.hpp>
//#include <LuaBridge/LuaBridge.h>
//
//struct Person {
//	std::string name;
//	int age;
//};
//
//void printPerson(const Person &person) {
//	std::cout << "Person - Name: " << person.name << ", Age: " << person.age << std::endl;
//}
//
//int main() {
//	lua_State *L = luaL_newstate();
//	luaL_openlibs(L);
//
//	// Expose a C++ structure to Lua
//	luabridge::getGlobalNamespace(L)
//		.beginClass<Person>("Person")
//		.addConstructor<void(*)(void)>()
//		.addData("name", &Person::name)
//		.addData("age", &Person::age)
//		.endClass()
//		.addFunction("printPerson", printPerson);
//
//	// Load and execute Lua script
//	luaL_dofile(L, "test_tables.lua");
//
//	lua_close(L);
//	return 0;
//}
