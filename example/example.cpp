#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <getopt.h>
#include "IOConfig.h"

using namespace std;

int main(int argc, char** argv) 
{

    string conf = "./conf.cfg";

    if(argc < 2)
    {
        cout << "Use default congfig:" << conf << endl;
    }
    else
    {
        conf = argv[1];
        cout << "Use congfig:" << conf << endl;
    }

    char *shortOpts = "c:";
    struct option longOpts[] =
    {    
        { "config", optional_argument, NULL, 'c' }
    };   

    int c;
    while ((c = getopt_long(argc, argv, shortOpts, longOpts, NULL)) != -1)
    {    
        switch (c)
        {    
        case 'c': 
            conf = optarg;
            break;
        default:
            break;
        }    
    }

	std::string temp;
	int port;
	std::string ipAddress;
	std::string username;
	std::string password;
	bool isCool = false;
	std::string TITLE_3V3;
    std::string strTitle;
    std::string strSubTitle;
    int iLimitNum;
	const char Config[] = "config.cfg";

	// /Users/sakishum1118/MyCCpp/try_catch
	// 读配置文件，失败返回一个 异常类对象
	try 
    {
		// Code that could throw an exception
		ConfigHandle.init(Config);
	} 
	catch (IOConfig::FileNotFound &filenf) 
    {
		std::cerr << "**Warring: Attemped to open no exist file <" << filenf.m_strFileName << ">." << std::endl;
		return EXIT_FAILURE;
	}

	// 新增
	ConfigHandle.add("saki", "shum");

	// 获取
	temp = ConfigHandle.getValue("saki", temp);		// 读取字符串
	port = ConfigHandle.getValue("port", 0);		// 读取整型
	ipAddress = ConfigHandle.getValue("ipAddress", ipAddress);
	username = ConfigHandle.getValue("username", username);
	password = ConfigHandle.getValue("password", password);
    bool bIsKeyExist = ConfigHandle.getValueTo(strTitle,"title");
	std::cout << "bIsKeyExist: " << bIsKeyExist << " title :" << strTitle << std::endl;
    std::string defValue("I am DefaultTitle");
    bIsKeyExist = ConfigHandle.getValueTo(strTitle,"title",defValue);
	std::cout << "bIsKeyExist: " << bIsKeyExist << " default title :" << strTitle << std::endl;
    bIsKeyExist = ConfigHandle.getValueTo(strTitle,"title","HaHa I change the default title");
	std::cout << "bIsKeyExist: " << bIsKeyExist << " default title :" << strTitle << std::endl;
    bIsKeyExist = ConfigHandle.getValueTo(strTitle,"title",0);
	std::cout << "bIsKeyExist: " << bIsKeyExist << " default title :" << strTitle << std::endl;
    bIsKeyExist = ConfigHandle.getValueStr(strTitle,"title","I change the title default value too");
	std::cout << "bIsKeyExist: " << bIsKeyExist << " subtitle :" << strTitle << std::endl;


    bIsKeyExist = ConfigHandle.getValueTo(strSubTitle,"subtitle","test I am subtitle default value");
	std::cout << "bIsKeyExist: " << bIsKeyExist << " subtitle :" << strSubTitle << std::endl;

    bIsKeyExist = ConfigHandle.getValueInt(iLimitNum,"limits",3);
	std::cout << "bIsKeyExist: " << bIsKeyExist << " limits:" << iLimitNum << std::endl;

	// 读取一个值，失败返回一个异常类对象
	try 
    {
		isCool = ConfigHandle.getValue<bool>("isCool");		// 读取布尔值
	}
    catch (IOConfig::KeyNotFound &keynf)
    {
		std::cerr << "**Warrning: Attemped to vist no exist key <" << keynf.m_strKey << ">." << std::endl;
		return EXIT_FAILURE;
	}
	TITLE_3V3 = ConfigHandle.getValue("3V3_TITLE", TITLE_3V3);

	std::cout << "saki      :" << temp << std::endl;
	std::cout << "port      :" << port << std::endl;
	std::cout << "ipAddress :" << ipAddress << std::endl;
	std::cout << "username  :" << username << std::endl;
	std::cout << "password  :" << password << std::endl;
    if (isCool) 
    {
		printf("isCool    :Found isCool TRUE\n"); 
	}
    else
    {
		printf("isCool    :Found isCool FALSE\n");
	}
	std::cout << "TITLE_3V3 :" << TITLE_3V3 << std::endl;
	return EXIT_SUCCESS;
}
