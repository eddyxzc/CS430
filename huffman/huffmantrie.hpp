#pragma once
#include"node.hpp"
#include<queue>
#include<iostream>
#include<fstream>
#include<sstream>
#include"FrenquenceTable.hpp"
using std::priority_queue;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using namespace std;

/*
union Charbit
{
	struct
	{
		char bit1 : 1;
		char bit2 : 1;
		char bit3 : 1;
		char bit4 : 1;
		char bit5 : 1;
		char bit6 : 1;
		char bit7 : 1;
		char bit8 : 1;
		
	};
	char char8bits;
};*/
class HFCodeTree
{

public:
	HFCodeTree() {}
	void buildCodeTable() {
		cout <<endl<< "Code Table:" << endl;
		asciicodedtable = vector<string>(256);
		buildCodeTable(asciicodedtable, root, "");
		for (unsigned int i=0;i<256;i++)
		{
			if (asciicodedtable[i]!="")
			{
				cout << static_cast<char>(i) << "---->" << asciicodedtable[i] << std::endl;
			}
		}
		std::cout << endl;
	}

	/*
	 Compress input text string and write the tree and coded text to return string
	*/
	string Compress(string inputChars) {
		FrequencyTable ft;
		auto asciiFreq = ft.caculateFrequencyTable(inputChars);

		vector<WeightedChar> CharFreqTable;
		for (unsigned int i = 0; i < asciiFreq.size(); i++)
		{
			if (asciiFreq[i] > 0)
			{
				CharFreqTable.emplace_back(WeightedChar(static_cast<char>(i), asciiFreq[i]));
			}
		}

		priority_queue<WeightedChar> pqWChars;
		for (auto wchar : CharFreqTable)
		{
			pqWChars.emplace(wchar);
		}
		auto pqWchars2(pqWChars);
		

		std::cout << endl << "Character Frenquency Table:" << endl;
		while (!pqWChars.empty())
		{
			auto item = pqWChars.top();
			cout << item.character << " " << item.frequency << endl;
			pqWChars.pop();
		}
		std::cout << endl;
		buildtree(pqWchars2);
		dumptree();//print tree in level order
		buildCodeTable();
		string codedtext= textCoded(inputChars);


		return codedtext;
	}

	string Decompress(string codedtext) {

	}
	string textCoded(string srctext) 
	{
		string codedtxt = "";
		for (auto s:srctext)
		{
			codedtxt += asciicodedtable[static_cast<unsigned int>(s)];
		}
		return codedtxt;
	}
	void buildtree(priority_queue<HFnode>& pqWChars) {
		while(pqWChars.size()>1)
		{
		nodeptr left(new HFnode(pqWChars.top()));
		pqWChars.pop();
		nodeptr right(new HFnode(pqWChars.top()));
		pqWChars.pop();
		HFnode parent =HFnode(left->frequency + right->frequency);
		parent.left = left;
		parent.right = right;
		pqWChars.push(parent);
		}
		root.reset(new HFnode(pqWChars.top()));
		pqWChars.pop();
	}
	
	string decode(string codetext) {
		string expandedtext = "";
		const char* inchars = codetext.c_str();
		for (size_t i=0;i<codetext.length();)
		{
			auto node = root;
			while (!node->isChar)
			{
				if (inchars[i] == '0') {
					node = node->left;
				}
				else {//"s==1"//
					node = node->right;
				}
				i++;
			}
			expandedtext += node->character;
		}
		return expandedtext;
	}
	void dumptree() {// 
		std::cout << "output tree in level order" << endl;
		std::queue<nodeptr> que;
		que.push(root);
		int len = 1;
		nodeptr cur;
		while (!que.empty())
		{


			for (int i = 0; i < len; i++)
			{
				cur = que.front();
				if (cur->isChar)
				{
					cout << cur->character << ":" << cur->frequency << " ";

				}
				else {
					cout << cur->frequency << " ";
				}
				que.pop();
				if (cur->left)
				{
					que.push(cur->left);
				}

				if (cur->right)
				{
					que.push(cur->right);
				}

			}
			len = que.size();
			cout << std::endl;
		}

	}
	
	void write_tree(nodeptr node,string& output) {
		if (node->isChar)
		{//write 0, leaf node
			output += node->character;
			
		}
		else {
			// write 1, nonleaf node
			output += static_cast<char>(0);
			write_tree(node->left, output);
			write_tree(node->right, output);
		}
		
	}
	void WriteComprssedFile(ofstream& out, string codedtext)
	{
		string huffmantreetxt = "";
		write_tree(root, huffmantreetxt);
		//string binarytree = String2bytes(huffmantreetxt);
		string binaryCodedText = String2bytes(codedtext);
		//out << static_cast<unsigned int>(huffmantreetxt.length());
		//out << static_cast<unsigned int> (binaryCodedText.length()) << static_cast<unsigned int>(codedtext.length());
		unsigned int treelength = huffmantreetxt.length();
		unsigned int binarytextlength = binaryCodedText.length();
		unsigned int codedtextlength = codedtext.length();

		out.write((char*)&treelength, sizeof(unsigned int));
		out.write((char*)&binarytextlength, sizeof(unsigned int));
		out.write((char*)&codedtextlength, sizeof(unsigned int));

		string info = (huffmantreetxt + binaryCodedText);
		out.write(info.c_str(), info.length());
		cout << endl << "The compressed text is " << binarytextlength << " bytes." << endl;
	}
	nodeptr read_tree(string& stringin,unsigned int& offset) {
		if (stringin[offset] != 0x00) {
                // leaf node
			nodeptr node(new HFnode(stringin[offset],0));
			offset++;
			return node;
		}
		else {
			//nonleaf node
			nodeptr node(new HFnode(0));
			offset++;
			node->left=read_tree(stringin, offset);
			node->right = read_tree(stringin, offset);
			return node;
		}
	}
	
	string ReadCompressedFile(ifstream& in) {
		unsigned int nTreebytes,nbytestext, nbitstext;
		in.read((char*)&nTreebytes, sizeof(unsigned int));
		in.read((char*)&nbytestext, sizeof(unsigned int));
		in.read((char*)&nbitstext, sizeof(unsigned int));
		string huffmantree, binaryCodedText;
		huffmantree.resize(nTreebytes);
		in.read((char*)huffmantree.c_str(), nTreebytes);
		binaryCodedText.resize(nbytestext);
		in.read((char*)binaryCodedText.c_str(), nbytestext);
		unsigned int offset = 0;
		root=read_tree(huffmantree, offset);
		dumptree();
		return bytes2string(binaryCodedText, nbitstext);
	}

	//compress strings like"10010001000" to binary bytes
	static string String2bytes(string& bitstream) {
		int n = bitstream.size();
		int nchar = n / 8;
		if (n % 8 > 0)nchar++;

		string bitchars;
		bitchars.resize(nchar);
		int ichar = 0;
		//int ibits = 0;
		char bit = 0;
		for (size_t ibit = 0; ibit < n; ibit++)
		{
			if (ibit > 0 && ibit % 8 == 0)
			{
				ichar++;
			}
			if (bitstream[ibit] == '1')
			{
				bit = 1;
			}
			else {
				bit = 0;
			}
			bitchars[ichar] = bitchars[ichar] << 1 | bit;
		}
		return bitchars;
	}

	// translate bytestream to string
	static string bytes2string(string bitcharstream,int n) {
		//size_t n= bitcharstream.size();
		size_t ichar = 0;
		size_t bitoffset = 8;
		string bitchars;
		bitchars.resize(n);
		for (size_t ibit = 0; ibit < n; ibit++)
		{
			if (ibit > 0 && ibit % 8 == 0)
			{
				ichar++;
				bitoffset = 8;//read from left to right
				if ((n - ibit) < 8)
					bitoffset = n - ibit;
			}
			bitoffset--;
			bitchars[ibit] = static_cast<char>(bitcharstream.c_str()[ichar] >> bitoffset) & 0x01;
			if (bitchars[ibit] == 0x01)bitchars[ibit] = '1';
			if (bitchars[ibit] == 0x00)bitchars[ibit] = '0';
		}
		return bitchars;
	}
	nodeptr root;// root node of the tree
	private:
	vector<string> asciicodedtable;
	//int size;
	void buildCodeTable(vector<string>& codetable, nodeptr root, string res) {
		if (root->isChar)
		{
			codetable[static_cast<unsigned int>(root->character)] = res;
			return;
		}
		buildCodeTable(codetable, root->left, res + '0');
		buildCodeTable(codetable, root->right, res + '1');
	}
};