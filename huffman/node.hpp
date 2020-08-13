#pragma once
#include<memory>
using std::shared_ptr;
using std::unique_ptr;

struct WeightedChar
{
	WeightedChar(char charc, unsigned int freq, bool isCharb = true) {
		character = charc;
		frequency = freq;
		isChar = isCharb;
	}
	WeightedChar(unsigned int freq) {
		character = 0;
		frequency = freq;
		isChar = false;
	}
	bool operator<(const WeightedChar& other) const
	{
		return other.frequency < this->frequency;
	}
	char character;
	unsigned int frequency;
	bool isChar;
	shared_ptr< WeightedChar> left =nullptr;
	shared_ptr< WeightedChar> right=nullptr;
};
struct WeightedCharInfile {
	WeightedCharInfile(const WeightedChar& mnode) {
		this->character = mnode.character;
		this->frequency = mnode.frequency;
		this->isChar = mnode.isChar;
	}
	///similar like WeightedChar
	char character;
	unsigned int frequency;
	bool isChar;
	/////////////////filed based linklist
	unsigned int left;
	unsigned int right;
};
/*
class Node
{
public:
	Node(char cha, int freqence, Node* leftnode, Node* rightnode)
		:ch(cha), freq(freqence), left(leftnode), right(rightnode)
	{}
	bool isLeaf()
	{
		return left == nullptr && right == nullptr;
	}
	bool operator <(const Node& other) {
		return freq < other.freq;
	}

private:
	char ch;
	int freq;
	Node* left;
	Node* right;
};
*/

typedef  WeightedChar HFnode;
typedef shared_ptr< WeightedChar> nodeptr;