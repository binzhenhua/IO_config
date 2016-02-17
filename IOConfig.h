/**
 *
 * @file oi_config.h
 * @Synopsis  Generic configuration Class.
 * @author zhenhuabin, zhenhuabin@tencent.com
 * @version 0.0.2
 * @date 2016-02-01
 *
 */
#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__

#include <pthread.h>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

#define ConfigHandle IOConfig::CConfig::GetInstance()

namespace IOConfig 
{
// Exception types
// 尽量不要将一个嵌套类声明为public，把嵌套类置于名字空间中是更好的方式。
class FileNotFound 
{
public:
	std::string m_strFileName;
	FileNotFound(const std::string &strFileName = std::string()) : m_strFileName(strFileName) { }
};

class KeyNotFound 
{
public:
	std::string m_strKey;
	KeyNotFound(const std::string &strKey = std::string()) : m_strKey(strKey) { }
};

class ThreadMutex
{

public:
    ThreadMutex()
    {
        pthread_mutex_init(&m_mtx,NULL);
    }

    ~ThreadMutex()
    {
        pthread_mutex_destroy(&m_mtx);
    }

    inline void Lock()
    {
        pthread_mutex_lock(&m_mtx);
    }

    inline void Unlock()
    {
        pthread_mutex_unlock(&m_mtx);
    }

private:
    pthread_mutex_t m_mtx;
};

class Lock
{

public:
    Lock(ThreadMutex &cs) : m_cs(cs)
    {
        m_cs.Lock();
    }
    ~Lock()
    {
        m_cs.Unlock();
    }

private:       
    ThreadMutex &m_cs;
};


class CConfig 
{
	// Method
public:
	static CConfig &GetInstance(); 
	void init(std::string strFilename, std::string strDelimiter = "=", std::string strComment = "#");
	template<typename T> T getValue(const std::string &strInKey) const;	//<! Searchfor key and read value or optional default value, call as getValue<T> 
	template<typename T> T getValue(const std::string &strInKey, const T &tValue) const;
	template<typename T> bool getValueTo(T &tOutVar, const std::string &strInKey) const;
	template<typename T> bool getValueTo(T &tOutVar, const std::string &strInKey, const T &tInValue) const;
	template<typename TOutVar,typename TInValue> bool getValueTo(TOutVar &tOutVar, const std::string &strInKey, const TInValue &tInValue) const;
	bool getValueInt(int &tOutVar, const std::string &strInKey, const int &tInValue) const;
	bool getValueStr(std::string &tOutVar, const std::string &strInKey, const std::string &tInValue) const;
	bool fileExist(std::string &strFileName);
	void readFile(std::string &strFileName, std::string strDelimiter="=", std::string strComment = "#");
	// Check whether key exists in configuration
	bool keyExists(const std::string &strInKey) const;

	// Modify keys and values
	template<class T> void add(const std::string &strInKey, const T& tInValue);
	void remove(const std::string &strInKey);

	// Check or change configuration syntax
	std::string getDelimiter(void) const { return m_Delimiter; }
	std::string getComment(void) const { return m_Comment; }
	std::string setDelimiter(const std::string &strSeparator) 
    {
		std::string strPreDelimiter = m_Delimiter; 
        m_Delimiter = strSeparator; 
        return strPreDelimiter;
	}
	std::string setComment(const std::string &strComment) 
    {
		std::string strPreComment = m_Comment; 
        m_Comment = strComment; 
        return strPreComment;
	}

	// Write or read configuration 
	friend std::ostream& operator<<(std::ostream& os, const CConfig& cf);
	friend std::istream& operator>>(std::istream& is, CConfig& cf);

	// Data
protected:
	std::string m_Delimiter;	//<! separator between key and value 
	std::string m_Comment;		//<! separator between value and comments
	std::map<std::string, std::string> m_Contents;	//!< extracted keys and values 

	typedef std::map<std::string,std::string>::iterator mapi;
	typedef std::map<std::string,std::string>::const_iterator mapConstItCin_t;

	template<typename T> static std::string T2String(const T &t);
	template<typename T> static T String2T(const std::string &str);
	static void Trim(std::string &strInOut);

private:
	CConfig();
	~CConfig();
	//DISALLOW_COPY_AND_ASSIGN(CConfig);
	CConfig(const CConfig&);
	void operator=(const CConfig&);

	static CConfig* m_Instance;
    static ThreadMutex m_cs;

};  // class CConfig

/* Static */
template<typename T>
std::string CConfig::T2String(const T &t) 
{
	// Conver from a T to a string
	// Type T must support << operator
	std::ostringstream ost;
	ost << t;
	return ost.str();
}

/* Static */
template<typename T>
T CConfig::String2T(const std::string &s) 
{
	// Convert from a string to a T
	// Type T must support >> operator
	T t;
	std::istringstream ist(s);
	ist >> t;
	return t;
}

/* Static */
template<>
inline std::string CConfig::String2T<std::string>(const std::string &s) 
{
	// Conver from a T to a string
	// In other words, do nothing
	return s;
}

/* Static */
template<> 
inline bool CConfig::String2T<bool>(const std::string& str) 
{
	// Convert from a string to a bool
	// Interpret "false", "F", "no", "n", "0" as false 
	// Interpret "true", "T", "yes", "y", "1", "-1", or anything else as true
	bool b = true;

	std::string sup = str;
	for (std::string::iterator it = sup.begin(); it != sup.end(); ++it) 
    {
		*it = toupper(*it); // make string all caps
	}
	if (sup == std::string("FALSE") || sup == std::string("F")
			|| sup == std::string("NO") || sup == std::string("N")
			|| sup == std::string("0") || sup == std::string("NONE")) 
    {
		b = false;
	}
	return b;
}

template<typename T>
T CConfig::getValue(const std::string& key) const 
{
	// Read the value corresponding to key
	mapConstItCin_t it = m_Contents.find(key);
	if (it == m_Contents.end()) throw KeyNotFound(key);
	return String2T<T>( it->second);
}

template<typename T>
T CConfig::getValue(const std::string &key, const T &value) const 
{
	// Return the value corresponding to key or given default value
	// if key is not found 
	mapConstItCin_t it = m_Contents.find(key);
	if (it == m_Contents.end()) 
    {
		printf("Not fount\n");
		return value;
	}
	return String2T<T>( it->second);
}

template<typename T>
bool CConfig::getValueTo(T &var, const std::string& key) const 
{
	// Get the value corresponding to key and store in var  
	// Return true if key is found  
	// Otherwise leave var untouched  
	mapConstItCin_t it = m_Contents.find(key);
	bool bFound = (it != m_Contents.end());
	if (bFound) 
    {
		var = String2T<T>( it->second );
	}
	return bFound;
}

template<typename T> 
bool CConfig::getValueTo(T &var, const std::string &key, const T &value) const 
{
	// Get the value corresponding to key and store in var 
	// Return true if key is found
	// Otherwise set var to given default
	mapConstItCin_t it = m_Contents.find(key);
	bool bFound = (it != m_Contents.end());
	if (bFound) 
    {
		var = String2T<T>( it->second );
	}
    else
    {
		var = value;
	}
	return bFound;
}

template<typename TOutVar,typename TInValue> 
bool CConfig::getValueTo(TOutVar &tOutVar, const std::string &strInKey, const TInValue &tInValue) const
{
	// Get the value corresponding to key and store in var 
	// Return true if key is found
	// Otherwise set var to given default
	mapConstItCin_t it = m_Contents.find(strInKey);
	bool bFound = (it != m_Contents.end());
	if (bFound) 
    {
		tOutVar = String2T<TOutVar>( it->second );
	}
    else
    {
        std::string strDefault = T2String<TInValue>(tInValue);
		tOutVar = String2T<TOutVar>(strDefault);
	}
	return bFound;
}


template<typename T>
void CConfig::add(const std::string &strInKey, const T &value) 
{
	// Add a key with given value  
	std::string strValue = T2String(value);
	std::string strKey = strInKey;
	Trim(strKey);
	Trim(strValue);
	m_Contents[strKey] = strValue;
	return;
}

}  // namespace IOConfig
#endif  // OPERATOR_CONFIG_H__

