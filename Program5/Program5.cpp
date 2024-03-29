/*
* Reflection: I learned a lot from this program. I have been used to doing the c# style of regex, so it was nice to mess around with a different style
* Also, I learned a lot about how the map data structure uses the comparator function to use it's keys. Finally, it was nice 
* to be able to use the range for for going through the containers.
*/

#include <iostream>
#include <istream>
#include <iterator>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include <regex>
#include <algorithm>
#include <cctype>
#include <sstream>

//used for determining when to add a line break into the output
//admittedly the choice of three was arbitrary
const size_t MULTIPLE_OF_MAX_WORD = 3;
const char const * OUTPUT_FILE_NAME = "result.txt";
template<typename T>
void crossReference(std::istream& input, std::map<std::string, std::map<size_t, size_t>, T>& out)
{
	std::string line;
	std::size_t lineNo = 0;
	std::regex matcher("\\b[[:alpha:]'-]+\\b");
	auto end = std::sregex_iterator();	
	//get stuff
	while (std::getline(input, line))
	{
		++lineNo;
		for (std::sregex_iterator i(line.begin(), line.end(), matcher); i != end; ++i)
			++out[(*i).str()][lineNo];
	}
}

int main(int argc, char** argv)
{
	std::ifstream input;
	if (argc > 1) input.open(argv[1]);
	using mapStr = std::basic_string<char>;
	auto compUse = [](std::string  a, std::string  b) 
	{ 
		int compared = _stricmp(a.c_str(), b.c_str());
		if (compared < 0) return true;
		else if (compared == 0) return a < b;
		else return false;
	};
	std::map<std::string, std::map<size_t, size_t>, decltype(compUse)> result(compUse);
	if (!input.is_open()) 
		crossReference(std::cin, result);
	else crossReference(input, result);
	std::ofstream outResult(OUTPUT_FILE_NAME);
	using lineCountType = decltype(result)::value_type;
	auto endElem = std::max_element(result.begin(), result.end(), [](lineCountType &pairA, lineCountType &pairB) { return pairA.first.length() < pairB.first.length(); });
	std::size_t maxLenKeyWord = (endElem == result.end()) ? 0 : endElem->first.length();
	//print results
	for (auto pair : result)
	{
		outResult << pair.first << std::setw(maxLenKeyWord - pair.first.length() + 2) << " : " << (*pair.second.begin()).first << ":" << (*pair.second.begin()).second;
		std::stringstream toWrite;
		for (auto iter = ++pair.second.begin(); iter != pair.second.end(); ++iter)
		{
			toWrite << ", ";
			if (toWrite.str().length() > maxLenKeyWord * MULTIPLE_OF_MAX_WORD)
			{
				outResult << toWrite.str() << std::endl << std::setw(maxLenKeyWord + 2) << " : ";
				toWrite.clear();
				toWrite.str(std::string());
			}
			toWrite << (*iter).first << ":" << (*iter).second;
		}
		if (toWrite.str().length() > 0) outResult << toWrite.str();
		outResult << std::endl;
	}
    return 0;
}