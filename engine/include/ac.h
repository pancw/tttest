
#ifndef __AC_H_
#define __AC_H_

#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <string>
#include <queue>
#include <cstring>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class Node
{
private:
	std::unordered_map<std::string, Node*>* allChild;
	Node* fail = nullptr;
	std::string word;
	bool isWord = false;
public:
	Node* getFailPointer();
	void setFailPointer(Node* f);
	bool hasChild(std::string s);
	Node* getChild(std::string s);
	void addChild(std::string s, Node *n);
	void setWord(std::string s);
	std::string getWord();
	bool checkIsWord();
	std::unordered_map<std::string, Node*>* getAllChild();

	Node();
	~Node();
};

void luaopen_ac_libs(lua_State *L);
#endif
