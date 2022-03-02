#include <vector>
#include <string>

#define LEN 100005
class Permutation
{
public:
	void InputArray(std::string str);	
	std::string GetPermutations();
protected:
	void PrintCase();

	std::vector<int> elements;
	char strBuffer[LEN];
};