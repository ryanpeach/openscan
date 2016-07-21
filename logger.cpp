#include "logger.hpp"

//------------ Intialize the logger by reading from properties file ---------------------
void initializeLogger()
{
	std::string initFileName = "log4cpp.properties";
	log4cpp::PropertyConfigurator::configure(initFileName);
      //  log4cpp::Category& root = log4cpp::Category::getRoot();

}

//-------------Function to test the logger ---------------------
//------This is intended for newcomers to this project to play around with the logger
//------and understand the logging practices followed in this project.

void testlogger()
{
        // Method 1 -  This method is to be used when within a call block many log statements are written to the same log file
	// 	       This will reduce the overhead of calling getInstance for each log statment.
	// The possible string values for the "getInstance" method are 
	// "prog_flow" - To write to Program Flow Log.
	// "trace" - To write to the trace.log file
	// "warn" - To write to the warnings.log file
	// "perf" - To write to the performance log file
	// "usage" - To write to the usage log file.

        log4cpp::Category& prog_flow = log4cpp::Category::getInstance(std::string("prog_flow"));
	prog_flow.debug("Writing debug to progflow log with reference object");
	prog_flow.info("Writing info to progflow log with reference object");
	prog_flow.warn("Writing warning to progflow log with reference object");

	// Method 2- Directly reference the catetory object
	// The overhead of assignment operator in method1 can be avoided by direcly referencing the log4cpp:Category object
	// as shown in the below code
	log4cpp::Category::getInstance(std::string("prog_flow")).debug("Direct Access Method"); 

	// Method 3 - Use the preprocessor directive defined in defines.chh file
	// The method is suggested for better code readability.
	// The preprocessor directives defined are 
	// LOG_PROG_FLOW,LOG_TRACE,LOG_WARNING,LOG_PERF,LOG_USAGE
	// Sample code for the same is provided below.

        LOG_PROG_FLOW.debug("Preprocessor Directive"); 						
        LOG_TRACE.debug("Testing trace");
        LOG_WARNING.warn("Testing Warning");

}


