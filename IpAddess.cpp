#include <iostream>
#include <fstream>
#include <set>
#include <vector>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::vector;

class IpAddressValidation
{
	int m_totalIpv4AddressCount = 0;
	int m_totalIpv6AddressCount = 0;
	int m_invalidIpAddressCount = 0;

	string inputFileName;

	std::set<string> m_setIpv4;
	std::set<string> m_setIpv6;
	std::set<string> m_duplicateIpv4;
	std::set<string> m_duplicateIpv6;

	bool isNumber(const string &str);
	vector<string> split(const string &str, char delim);
	bool checkIPv4(string& inputIpAddress);
	bool HexValueValidation(string s);
	bool checkIPv6(string& inputIpAddress);

public:
	IpAddressValidation(string& filename) :inputFileName(filename) {}
	void checkIPAddress();
	void getAddressCounts() {
		cout << "Total ipv4 address count: " << m_totalIpv4AddressCount << endl;
		cout << "Total ipv6 address count: " << m_totalIpv6AddressCount << endl;
		cout << "Unique ipv4 address count: " << m_setIpv4.size() << endl;
		cout << "Unique ipv6 address count: " << m_setIpv6.size() << endl;
		cout << "Total invalid address count: " << m_invalidIpAddressCount << endl;
	}
};
//////////////////////////////////////////////////////////////////////
// isNumber() - Function to check if the input string is a valid number 
// input - string
//////////////////////////////////////////////////////////////////////
bool IpAddressValidation::isNumber(const string &str)
{
	return !str.empty() &&
		(str.find_first_not_of("[0123456789]") == std::string::npos);
}

/////////////////////////////////////////////////////////////////////
// split() - Function to split string `str` using a given delimiter
// input - string, char
////////////////////////////////////////////////////////////////////
vector<string> IpAddressValidation::split(const string &str, char delim)
{
	auto i = 0;
	vector<string> list;

	auto pos = str.find(delim);

	while (pos != string::npos)
	{
		list.push_back(str.substr(i, pos - i));
		i = ++pos;
		pos = str.find(delim, pos);
	}

	list.push_back(str.substr(i, str.length()));

	return list;
}
/////////////////////////////////////////////////////////////////////////////////////
// checkIPv4() - Function to check if the input address is a valid ipv4 address or not
// input - string
////////////////////////////////////////////////////////////////////////////////////
bool IpAddressValidation::checkIPv4(string& inputIpAddress) {

	vector<string> splitDigits = split(inputIpAddress, '.');

	if (splitDigits.size() > 0) {
		for (auto &i : splitDigits)
		{
			if (isNumber(i))
			{
				if (atoi((i.c_str())) <= 255)
					continue;
				else
					return false;
			}
			else
				return false;
		}
	}
	else
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// checkHex() - Function to check if the string represents a hexadecimal number
// input - string
///////////////////////////////////////////////////////////////////////////////
bool IpAddressValidation::HexValueValidation(string s)
{
	int n = s.length();

	// Iterate over string
	for (int i = 0; i < n; i++) {
		char ch = s[i];

		// Check if the character
		// is invalid
		if ((ch < '0' || ch > '9')
			&& (ch < 'A' || ch > 'F')
			&& (ch < 'a' || ch > 'f')) {
			return false;
		}
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////
// checkIPv6() - Function to check if the given string is valid ipv6 address or not
// input - string
///////////////////////////////////////////////////////////////////////////////////
bool IpAddressValidation::checkIPv6(string& inputIpAddress)
{
    vector<string> parsedVector = split(inputIpAddress, ':');
    
    //To count the number of values splitting with ':'
    if (parsedVector.size() != 8)
		return false;
    
    int count = 0;

    //To count number of times ':' occurred
    for (int i = 0; i < inputIpAddress.size();i++) {
		if (inputIpAddress[i] == ':')
			count++;
	}

    //To check if count is not equals to 7 then return
	if (count != 7)
		return false;

	//To check if all the splited values are in hexadecimal format
	for (int i = 0;
		i < parsedVector.size(); i++) {

		int len = parsedVector[i].size();

		if (!HexValueValidation(parsedVector[i])
			|| len > 4 || len < 1) {
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//checkIPAddress() - Function to open the input file, parse and validate all the ip addresses
//input - void
/////////////////////////////////////////////////////////////////////////////////////////////
void IpAddressValidation::checkIPAddress()
{
	std::string line;

	ifstream input_file(inputFileName);
	if (!input_file.is_open()) {
		cerr << "Could not open the file - '"
			<< inputFileName << "'" << endl;
		return;
	}

	while (getline(input_file, line)) {

		if (checkIPv4(line))
		{
			++m_totalIpv4AddressCount;
			int size1 = m_setIpv4.size();
			m_setIpv4.insert(line);
			int size2 = m_setIpv4.size();

			if (size1 == size2)
				m_duplicateIpv4.insert(line);
		}
		else if (checkIPv6(line))
		{
			++m_totalIpv6AddressCount;
			int size1 = m_setIpv6.size();
			m_setIpv6.insert(line);
			int size2 = m_setIpv6.size();

			if (size1 == size2)
				m_duplicateIpv6.insert(line);
		}
		else {
			++m_invalidIpAddressCount;
		}
	}
	input_file.close();
}

int main()
{
	string filename("ipAddresses.txt");

	IpAddressValidation ipValid(filename);
	
    ipValid.checkIPAddress();
	ipValid.getAddressCounts();

	return 0;
}