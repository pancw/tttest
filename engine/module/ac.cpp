#include "ac.h"

Node* Node::getFailPointer() {
	return this->fail;
}

void Node::setFailPointer(Node* f) {
	this->fail = f;
}

bool Node::hasChild(std::string s) {
	if (this->allChild->find(s)!= this->allChild->end())
	{
		return true;
	}
	return false;
}

Node* Node::getChild(std::string s) {
	if (this->hasChild(s))
	{
		return (*(this->allChild))[s];
	}
	return nullptr;
}

void Node::addChild(std::string s, Node *n) {
	(*(this->allChild))[s] = n;
}

void Node::setWord(std::string s)
{
	this->isWord = true;
	this->word = s;
}

std::string Node::getWord() {
	return this->word;
}

bool Node::checkIsWord() {
	return this->isWord;
}

std::unordered_map<std::string, Node*>* Node::getAllChild() {
	return this->allChild;
}

Node::Node() {
	this->allChild = new std::unordered_map<std::string, Node*>();
}

Node::~Node() {
	// TODO
}

// ----------------------------------------------------------------class Node

int chsize(char c)
{
	if (c > 240)
		return 4;
	else if(c > 225)
		return 3;
	else if(c > 192)
		return 2;
	else
		return 1;
}

Node* root = nullptr;

static void add(const char* str) {
	int len = strlen(str);
	if (len == 0) return;

	Node* now = root;
	int idx = 0;
	while (idx < len)
	{
		int utfLen = chsize(str[idx]);
		//printf("utfLen=%d,char=%c\n", utfLen, str[idx]);

		char utfStr[100] = {}; // TODO
		strncpy(utfStr, str + idx, utfLen);
		std::string s(utfStr);
		//std::cout << s << std::endl;

		idx += utfLen;
		if (!now->hasChild(s))
		{
			now->addChild(s, new Node());
		}
		now = now->getChild(s);
	}
	std::string word(str);
	now->setWord(word);
}

static void build_fail_pointer()
{
	// bfs
	std::queue<Node *> q;
	q.push(root);

	while (!q.empty())
	{
		Node* now = q.front();
		q.pop();

		std::unordered_map<std::string, Node*>* allChild = now->getAllChild();
		std::unordered_map<std::string, Node*>::iterator it = allChild->begin();
		for (; it != allChild->end(); it++)
		{
			std::string s = it->first;
			Node* fail = now->getFailPointer();
			while (fail && (!fail->hasChild(s)))
			{
				fail = fail->getFailPointer();
			}

			Node* n = it->second;
			if (fail)
			{
				n->setFailPointer(fail->getChild(s));
			}
			else
			{
				n->setFailPointer(root);
			}
			q.push(n);
		}
	}
	root->setFailPointer(root);
}

static void try_match(const char* input)
{
	Node* now = root;
	int idx = 0;
	int len = strlen(input);

	while (idx < len)
	{
		int utfLen = chsize(input[idx]);
		char utfStr[100] = {}; // TODO
		strncpy(utfStr, input + idx, utfLen);
		std::string s(utfStr);
		idx += utfLen;

		while (!(now->getChild(s)) && now != root)
		{
			now = now->getFailPointer();
		}

		if (now->hasChild(s))
		{
			now = now->getChild(s);
		}

		//if (now->checkIsWord())
			// match now->getWord();
		//else
		Node *f = now;
		while (f != root)
		{
			if (f->checkIsWord())
			{
				std::cout << "matched:" << f->getWord() << std::endl;
			}
			f = f->getFailPointer();
		}
	}
}

/*
char tt[][1000] = {
	"abcrr",
	"bc",
	"d",
	"ºº×Ó",
	"²ÝÄàÂí",
};

void build_tree()
{
	int i = 0;
	for (; i < 5; i++)
	{
		add(tt[i]);
	}
}

int init()
{
	printf("begin:\n");
	build_tree();
	printf("build tree done.\n");
	build_fail_pointer();
	printf("build fail done.\n");

	char input[1000];
	while (std::cin >> input)
	{
		printf("input=%s\n", input);
		try_match(input);
	}

	return 0;
}
*/

static int createRoot(lua_State* L)
{
		
	root = new Node();
	return 0;	
}

static int addNode(lua_State* L)
{
	const char* str = lua_tostring(L, 1);
	if (!str)
	{
		return 0;
	}
	add(str);
	return 0;
}

static int buildFailPointer(lua_State* L)
{
	build_fail_pointer();
	return 0;
}

static int tryMatch(lua_State* L)
{
	const char* str = lua_tostring(L, 1);
	if (!str)
	{
		return 0;
	}
	try_match(str);
	return 0;
}

const luaL_reg aclib[] =
{
	{"createRoot", createRoot},
	{"addNode", addNode},
	{"buildFailPointer", buildFailPointer},
	{"tryMatch", tryMatch},
	{NULL, NULL},
};

void luaopen_ac_libs(lua_State* L)
{
	luaL_register(L, "lac", aclib);	
}
