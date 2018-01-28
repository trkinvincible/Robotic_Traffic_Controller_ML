

#include "TraceLogger.hpp"

#include <ctime>
#include <iomanip>


std::string getTime()
{
	//time_t * rawtime = new time_t;
	//struct tm * timeinfo;
	//time_t t = time(NULL);
	////time(rawtime);
	//struct tm buf;
	//timeinfo = localtime_s(&t, &buf);

 //              // int tm_sec;         /* seconds */
 //              // int tm_min;         /* minutes */
 //              // int tm_hour;        /* hours */

	std::stringstream sstr;
	//// sstr << asctime(timeinfo);
	//sstr
	//	<< std::setfill('0') << std::setw(2) // <= left pad (size of 2)
	//	<< timeinfo->tm_hour << ":"
	//	<< std::setfill('0') << std::setw(2) // <= left pad (size of 2)
	//	<< timeinfo->tm_min << ":"
	//	<< std::setfill('0') << std::setw(2) // <= left pad (size of 2)
	//	<< timeinfo->tm_sec;

	return sstr.str();
}
