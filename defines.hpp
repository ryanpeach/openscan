/**
 * defines.hpp
 *
 *  Created on: Dec 08, 2015
 *      Author: Sundaram
 * This file has the necessary dependant files to be included for the logger to work. Include it in the source file where logging is to be used.
 * To change the logging levels for any of the files you need to change the threshold settings in the log4cpp.properties file. 
 * The logging levels can be changed without recompiling the code.
 */
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#define LOG_PROG_FLOW log4cpp::Category::getInstance(std::string("prog_flow"))
#define LOG_TRACE log4cpp::Category::getInstance(std::string("trace"))
#define LOG_WARNING log4cpp::Category::getInstance(std::string("warn"))
#define LOG_PERF log4cpp::Category::getInstance(std::string("perf"))
#define LOG_USAGE log4cpp::Category::getInstance(std::string("usage"))
