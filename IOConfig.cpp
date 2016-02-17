/**
 * @file oi_config.cpp
 * @Synopsis  Generic configuration Class.
 * @author zhenhuabin, zhenhuabin@tencent.com
 * @version 0.0.2
 * @date 2016-02-01
 */
#include "IOConfig.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>

namespace IOConfig 
{

CConfig* CConfig::m_Instance = NULL;
ThreadMutex CConfig::m_cs;

CConfig::CConfig() : m_Delimiter(std::string(1, '=')), m_Comment(std::string(1, '#')) 
{
	// Construct a Config without a file;
}

CConfig::~CConfig() 
{
	// Disstruct a Config
}

CConfig& CConfig::GetInstance() 
{
    if(m_Instance == NULL)
    {
        Lock lock(m_cs);  
        if(m_Instance == NULL)
        {
            m_Instance = new CConfig();
        }
    }
    return *m_Instance;
}

void CConfig::init(std::string filename, std::string delimiter, std::string comment) 
{
	setDelimiter(delimiter);
	setComment(comment);
	std::ifstream in(filename.c_str()); 
	if (!in) 
    {
		printf("Couldn\'t open this file!\n");
		throw FileNotFound(filename); 
	}
	in >> (*this);	// save in memory by operator>>
	in.close();		// auto release 
}

bool CConfig::keyExists(const std::string &key) const 
{
	// Indicate wherher key is found
	mapConstItCin_t p = m_Contents.find(key);
	return (p != m_Contents.end());
}

/* Static */
void CConfig::Trim(std::string &strInOut) 
{
	// Remove leading and trailind whitespace
	static const char whitespace[] = " \n\t\v\r\f";
	strInOut.erase(0, strInOut.find_first_not_of(whitespace));
	strInOut.erase(strInOut.find_last_not_of(whitespace) + 1U);
}

void CConfig::remove(const std::string &key) 
{
	// Remove key and its value
	m_Contents.erase(m_Contents.find(key));
	return;
}

bool CConfig::fileExist(std::string &filename) 
{
	bool exist = false;
	std::ifstream in(filename.c_str());
	if (in) 
    {
		exist = true;
		//in.close();	// auto release
	}
	return exist;
}

void CConfig::readFile(std::string &filname, std::string delimiter, std::string comment) 
{
	m_Delimiter = delimiter;
	m_Comment = comment;
	std::ifstream in(filname.c_str());
	if (!in) throw FileNotFound(filname);
	in >> (*this);
	in.close();			// auto release 
}

std::ostream& operator<<(std::ostream &os, const CConfig  &cf) 
{
	// Save a Config to os
	for (CConfig::mapConstItCin_t p = cf.m_Contents.begin(); p != cf.m_Contents.end(); ++p) 
    {
		os << p->first << " " << cf.m_Delimiter << " " << p->second << std::endl;
	}
	return os;
}

std::istream& operator>>(std::istream &is, CConfig& cf) 
{
	// Load a Config from is
	// Read in keys and values, kepping internal whitespace
	typedef std::string::size_type pos;
	const std::string &delim = cf.m_Delimiter;	// sparator
	const std::string &comm = cf.m_Comment;		// comment
	const pos skip = delim.length();			// length of sparator
	std::string nextline = "";	// might need to read ahead to see where value ends

	while (is || nextline.length() > 0) 
    {
		// Read an entrie line at a time
		std::string line;
		if (nextline.length() > 0) 
        {
			line = nextline;	// we read ahead; use it now
			nextline = "";
		}
        else
        {
			std::getline(is, line);
		}

		// Ignore comments ('#')
		line = line.substr(0, line.find(comm));

		// Parse the line if it contains a delimiter ('=')
		pos delimPos = line.find(delim);
		if (delimPos < std::string::npos) 
        {
			// Extract the key
			std::string key = line.substr(0, delimPos);
			line.replace(0, delimPos+skip, "");

			// See if value continues on the next line
			// Stop at blank line, next line with a key, end of stream,
			// or end of file sentry
			bool terminate = false;
			while (!terminate && is) 
            {
				std::getline(is, nextline);
				terminate = true;

				std::string nlcopy = nextline;
				CConfig::Trim(nlcopy);
				if (nlcopy == "") 
                {
					continue;
				}

				nextline = nextline.substr(0, nextline.find(comm));
				if (nextline.find(delim) != std::string::npos) 
                {
					continue;
				}

				nlcopy = nextline;
				CConfig::Trim(nlcopy);
				if (nlcopy != "") 
                {
					line += "\n";
				}

				line += nextline;
				terminate = false;
			}

			// Store key and value 
			CConfig::Trim(key);
			CConfig::Trim(line);
			cf.m_Contents[key] = line;	// overwrites if key is repeated
		}
	}
	return is;
}

bool CConfig::getValueInt(int &tOutVar, const std::string &strInKey, const int &tInValue) const
{
	// Get the value corresponding to key and store in var 
	// Return true if key is found
	// Otherwise set var to given default
	mapConstItCin_t it = m_Contents.find(strInKey);
	bool bFound = (it != m_Contents.end());
	if (bFound) 
    {
		tOutVar = String2T<int>( it->second );
	}
    else
    {
        std::string strDefault = T2String<int>(tInValue);
		tOutVar = String2T<int>(strDefault);
	}
	return bFound;
}

bool CConfig::getValueStr(std::string &tOutVar, const std::string &strInKey, const std::string &tInValue) const
{
	// Get the value corresponding to key and store in var 
	// Return true if key is found
	// Otherwise set var to given default
	mapConstItCin_t it = m_Contents.find(strInKey);
	bool bFound = (it != m_Contents.end());
	if (bFound) 
    {
		tOutVar = it->second;
	}
    else
    {
		tOutVar = tInValue;
	}
	return bFound;
}

}  // namespace operatorconfig
