#pragma once
/**
#include <iostream>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h> 
#include <log4cplus/layout.h> 
#include <log4cplus/loggingmacros.h> 
#include <log4cplus/helpers/stringhelper.h> 
 
#define MY_LOG_FILE_PATH "F:/Project/ComShao/PlateRE/PlateER/etc/logconfig.properites"
 
using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;
 
class MyLogger
{
public:
	static MyLogger * getInstance();
	Logger logger;
private:
	MyLogger();
	~MyLogger();
	static MyLogger * my_logger;
};*/