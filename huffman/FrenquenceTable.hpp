#pragma once
#include<queue>
#include"node.hpp"
#include<string>
class FrequencyTable
{
public:
	FrequencyTable()
	{
	}
	std::vector<unsigned int>caculateFrequencyTable(std::string inputstr)
	{
		std::vector<unsigned int> asciiTable(257, 0);

		for (char s:inputstr)
		{
			asciiTable[static_cast<unsigned int>(s)]++;
		}
		return asciiTable;
	}
	//std::vector<Node> frenqT;

};