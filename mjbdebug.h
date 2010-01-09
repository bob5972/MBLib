#ifndef MJBDEBUG_H_201001091239
#define MJBDEBUG_H_201001091239

//should be pulled form a config header
#define MJB_ENABLE_DEBUG

//backtraces and logfile not implemented

#ifndef MJB_DISABLE_DEBUG
	#ifndef MJB_ENABLE_DEBUG
		#warning No debug behavior specified.  Defaulting to enabled.
	#endif
	
	#define MJB_ENABLE_DEBUG
#else
	#ifdef MJB_ENABLE_DEBUG
		#warning Conflicting debug behavior specified.  Defaulting to enabled.
		#undef MJB_DISABLE_DEBUG
	#else
		#define MJB_ENABLE_DEBUG
	#endif
#endif
	
	
#ifdef MJB_ENABLE_DEBUG
	#define MJB_DEBUG

	#ifndef MJB_DISABLE_ASSERTIONS
		#define MJB_ENABLE_ASSERTIONS
	#endif
#endif

#ifdef MJB_DISABLE_DEBUG
	#ifdef MJB_DEBUG
		#error Bad debug settings specified.
	#endif
#endif

#ifndef MJB_DEBUG_LOG_FILE
	#define MJB_DEBUG_LOG_FILE "debug.log"
#endif

//Default "enabled" options
#ifdef MJB_DEBUG
	//General debug options
	#ifndef MJB_DEBUG_MESSAGES
		#define MJB_DEBUG_MESSAGES 1
	#endif
	
	#ifndef MJB_DEBUG_CRASH_ON_ERROR
		#define MJB_DEBUG_CRASH_ON_ERROR 1
	#endif
	
	//backtrace options
	#ifndef MJB_DEBUG_STACK_TRACE_ON_VERBOSE
		#define MJB_DEBUG_STACK_TRACE_ON_VERBOSE 0
	#endif
	
	#ifndef MJB_DEBUG_STACK_TRACE_ON_INFO
		#define MJB_DEBUG_STACK_TRACE_ON_INFO 0
	#endif
	
	#ifndef MJB_DEBUG_STACK_TRACE_ON_WARN
		#define MJB_DEBUG_STACK_TRACE_ON_WARN 1
	#endif
	
	#ifndef MJB_DEBUG_STACK_TRACE_ON_ERROR
		#define MJB_DEBUG_STACK_TRACE_ON_ERROR 1
	#endif	
	
	// std out printing
	#ifndef MJB_DEBUG_STD_OUT_MESSAGES
		#define MJB_DEBUG_STD_OUT_MESSAGES 0
	#endif
	
	#ifndef MJB_DEBUG_STD_OUT_MESSAGES_VERBOSE
		#define MJB_DEBUG_STD_OUT_MESSAGES_VERBOSE 0
	#endif
	
	#ifndef MJB_DEBUG_STD_OUT_MESSAGES_INFO
		#define MJB_DEBUG_STD_OUT_MESSAGES_INFO 0
	#endif
	
	#ifndef MJB_DEBUG_STD_OUT_MESSAGES_WARN
		#define MJB_DEBUG_STD_OUT_MESSAGES_WARN 0
	#endif
	
	#ifndef MJB_DEBUG_STD_OUT_MESSAGES_ERROR
		#define MJB_DEBUG_STD_OUT_MESSAGES_ERROR 0
	#endif
	
	// std err printing 	
	#ifndef MJB_DEBUG_STD_ERR_MESSAGES
		#define MJB_DEBUG_STD_ERR_MESSAGES 1
	#endif
	
	#ifndef MJB_DEBUG_STD_ERR_MESSAGES_VERBOSE
		#define MJB_DEBUG_STD_ERR_MESSAGES_VERBOSE 0
	#endif
	
	#ifndef MJB_DEBUG_STD_ERR_MESSAGES_INFO
		#define MJB_DEBUG_STD_ERR_MESSAGES_INFO 1
	#endif
	
	#ifndef MJB_DEBUG_STD_ERR_MESSAGES_WARN
		#define MJB_DEBUG_STD_ERR_MESSAGES_WARN 1
	#endif
	
	#ifndef MJB_DEBUG_STD_ERR_MESSAGES_ERROR
		#define MJB_DEBUG_STD_ERR_MESSAGES_ERROR 1
	#endif
	
	// log file printing	
	#ifndef MJB_DEBUG_LOG_MESSAGES
		#define MJB_DEBUG_LOG_MESSAGES 1
	#endif
	
	#ifndef MJB_DEBUG_LOG_MESSAGES_VERBOSE
		#define MJB_DEBUG_LOG_MESSAGES_VERBOSE 0
	#endif
	
	#ifndef MJB_DEBUG_LOG_MESSAGES_INFO
		#define MJB_DEBUG_LOG_MESSAGES_INFO 1
	#endif
	
	#ifndef MJB_DEBUG_LOG_MESSAGES_WARN
		#define MJB_DEBUG_LOG_MESSAGES_WARN 1
	#endif
	
	#ifndef MJB_DEBUG_LOG_MESSAGES_ERROR
		#define MJB_DEBUG_LOG_MESSAGES_ERROR 1
	#endif
#else //MJB_DEBUG not defined

		//General debug options
	#ifndef MJB_DEBUG_MESSAGES
		#define MJB_DEBUG_MESSAGES 0
	#endif
	
	#ifndef MJB_DEBUG_CRASH_ON_ERROR
		#define MJB_DEBUG_CRASH_ON_ERROR 0
	#endif
	
	//backtrace options
	#ifndef MJB_DEBUG_STACK_TRACE_ON_VERBOSE
		#define MJB_DEBUG_STACK_TRACE_ON_VERBOSE 0
	#endif
	
	#ifndef MJB_DEBUG_STACK_TRACE_ON_INFO
		#define MJB_DEBUG_STACK_TRACE_ON_INFO 0
	#endif
	
	#ifndef MJB_DEBUG_STACK_TRACE_ON_WARN
		#define MJB_DEBUG_STACK_TRACE_ON_WARN 1
	#endif
	
	#ifndef MJB_DEBUG_STACK_TRACE_ON_ERROR
		#define MJB_DEBUG_STACK_TRACE_ON_ERROR 1
	#endif	
	
	// std out printing
	#ifndef MJB_DEBUG_STD_OUT_MESSAGES
		#define MJB_DEBUG_STD_OUT_MESSAGES 0
	#endif
	
	#ifndef MJB_DEBUG_STD_OUT_MESSAGES_VERBOSE
		#define MJB_DEBUG_STD_OUT_MESSAGES_VERBOSE 0
	#endif
	
	#ifndef MJB_DEBUG_STD_OUT_MESSAGES_INFO
		#define MJB_DEBUG_STD_OUT_MESSAGES_INFO 0
	#endif
	
	#ifndef MJB_DEBUG_STD_OUT_MESSAGES_WARN
		#define MJB_DEBUG_STD_OUT_MESSAGES_WARN 0
	#endif
	
	#ifndef MJB_DEBUG_STD_OUT_MESSAGES_ERROR
		#define MJB_DEBUG_STD_OUT_MESSAGES_ERROR 0
	#endif
	
	// std err printing 	
	#ifndef MJB_DEBUG_STD_ERR_MESSAGES
		#define MJB_DEBUG_STD_ERR_MESSAGES 0
	#endif
	
	#ifndef MJB_DEBUG_STD_ERR_MESSAGES_VERBOSE
		#define MJB_DEBUG_STD_ERR_MESSAGES_VERBOSE 0
	#endif
	
	#ifndef MJB_DEBUG_STD_ERR_MESSAGES_INFO
		#define MJB_DEBUG_STD_ERR_MESSAGES_INFO 0
	#endif
	
	#ifndef MJB_DEBUG_STD_ERR_MESSAGES_WARN
		#define MJB_DEBUG_STD_ERR_MESSAGES_WARN 0
	#endif
	
	#ifndef MJB_DEBUG_STD_ERR_MESSAGES_ERROR
		#define MJB_DEBUG_STD_ERR_MESSAGES_ERROR 0
	#endif
	
	// log file printing	
	#ifndef MJB_DEBUG_LOG_MESSAGES
		#define MJB_DEBUG_LOG_MESSAGES 0
	#endif
	
	#ifndef MJB_DEBUG_LOG_MESSAGES_VERBOSE
		#define MJB_DEBUG_LOG_MESSAGES_VERBOSE 0
	#endif
	
	#ifndef MJB_DEBUG_LOG_MESSAGES_INFO
		#define MJB_DEBUG_LOG_MESSAGES_INFO 0
	#endif
	
	#ifndef MJB_DEBUG_LOG_MESSAGES_WARN
		#define MJB_DEBUG_LOG_MESSAGES_WARN 0
	#endif
	
	#ifndef MJB_DEBUG_LOG_MESSAGES_ERROR
		#define MJB_DEBUG_LOG_MESSAGES_ERROR 0
	#endif
#endif //MJB_DEBUG

//You could disable these if you don't want the debug functions called (for performance), but for now its more work than it's worth.

#define DebugPrint(msg) { DebugPrintHelper( (msg), __FILE__, __LINE__); }
#define DebugPrintStdOut(msg) { DebugPrintStdOutHelper( (msg), __FILE__, __LINE__); }
#define DebugPrintStdErr(msg) { DebugPrintStdErrHelper( (msg), __FILE__, __LINE__); }
#define DebugPrintLog(msg) { DebugPrintLogHelper( (msg), __FILE__, __LINE__); }
#define DebugPrintVerbose(msg) { DebugPrintVerboseHelper( (msg), __FILE__, __LINE__); }
#define DebugPrintInfo(msg) { DebugPrintInfoHelper( (msg), __FILE__, __LINE__); }
#define DebugPrintWarn(msg) { DebugPrintWarnHelper( (msg), __FILE__, __LINE__); }
#define DebugPrintError(msg) { DebugPrintErrorHelper( (msg), __FILE__, __LINE__); }
	
void DebugPrintHelper(const char* message, const char* file, int line);
void DebugPrintStdOutHelper(const char* message, const char* file, int line);
void DebugPrintStdErrHelper(const char* message, const char* file, int line);
void DebugPrintLogHelper(const char* message, const char* file, int line);

void DebugVerboseHelper(const char* message, const char* file, int line);
void DebugInfoHelper(const char* message, const char* file, int line);
void DebugWarnHelper(const char* message, const char* file, int line);
void DebugErrorHelper(const char* message, const char* file, int line);


#endif //MJBDEBUG_H_201001091239
