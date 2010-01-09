
#include <stdio.h>
#include <stdlib.h>

#include "mjbdebug.h"

void DebugPrintHelper(const char* message, const char* file, int line)
{
	if(MJB_DEBUG_STD_OUT_MESSAGES) {
		DebugPrintStdOutHelper(message, file, line);
	} else if (MJB_DEBUG_STD_ERR_MESSAGES) {
		DebugPrintStdErrHelper(message,file,line);
	}
	
	DebugPrintLogHelper(message,file,line);
}


void DebugPrintStdOutHelper(const char* message, const char* file, int line)
{
	if(!MJB_DEBUG_MESSAGES) {
		return;
	}
	
	if(MJB_DEBUG_STD_OUT_MESSAGES) {
		fprintf(stdout, "Debug: %s:%d: %s\n", file, line, message);
	}
}

void DebugPrintStdErrHelper(const char* message, const char* file, int line)
{
	if(!MJB_DEBUG_MESSAGES) {
		return;
	}
	
	if(MJB_DEBUG_STD_ERR_MESSAGES) {
		fprintf(stderr, "Debug: %s:%d: %s\n", file, line, message);
	}
}

void DebugPrintLogHelper(const char* message, const char* file, int line)
{
	if(!MJB_DEBUG_MESSAGES) {
		return;
	}
	
	if(MJB_DEBUG_STD_LOG_MESSAGES) {
		//NOT_IMPLEMENTED();
	}
}

void DebugVerboseHelper(const char* message, const char* file, int line)
{
	if(!MJB_DEBUG_MESSAGES) {
		return;
	}
	
	if(MJB_DEBUG_STD_OUT_MESSAGES_VERBOSE) {
		DebugPrintStdOutHelper(message, file, line);
	} else if(MJB_DEBUG_STD_ERR_MESSAGES_VERBOSE) {
		DebugPrintStdErrHelper(message,file,line);
	}
	
	if(MJB_DEBUG_LOG_MESSAGES_VERBOSE) {
		DebugPrintLogHelper(message,file,line);
	}
}

void DebugInfoHelper(const char* message, const char* file, int line)
{
	if(!MJB_DEBUG_MESSAGES) {
		return;
	}
	
	if(MJB_DEBUG_STD_OUT_MESSAGES_INFO) {
		DebugPrintStdOutHelper(message, file, line);
	} else if(MJB_DEBUG_STD_ERR_MESSAGES_INFO) {
		DebugPrintStdErrHelper(message,file,line);
	}
	
	if(MJB_DEBUG_LOG_MESSAGES_INFO) {
		DebugPrintLogHelper(message,file,line);
	}
}

void DebugWarnHelper(const char* message, const char* file, int line)
{
	if(!MJB_DEBUG_MESSAGES) {
		return;
	}
	
	if(MJB_DEBUG_STD_OUT_MESSAGES_WARN) {
		DebugPrintStdOutHelper(message, file, line);
	} else if(MJB_DEBUG_STD_ERR_MESSAGES_WARN) {
		DebugPrintStdErrHelper(message,file,line);
	}
	
	if(MJB_DEBUG_LOG_MESSAGES_WARN) {
		DebugPrintLogHelper(message,file,line);
	}
}

void DebugErrorHelper(const char* message, const char* file, int line)
{
	if(!MJB_DEBUG_MESSAGES) {
		return;
	}
	
	if(MJB_DEBUG_STD_OUT_MESSAGES_ERROR) {
		DebugPrintStdOutHelper(message, file, line);
	} else if(MJB_DEBUG_STD_ERR_MESSAGES_ERROR) {
		DebugPrintStdErrHelper(message,file,line);
	}
	
	if(MJB_DEBUG_LOG_MESSAGES_ERROR) {
		DebugPrintLogHelper(message,file,line);
	}
}

