#include "Log.h"

#include "utils/FileSystemUtil.h"
#include "platform.h"
#include <iostream>
#include <mutex>
#include "Settings.h"
#include <iomanip> 

#if WIN32
#include <Windows.h>
#endif

static std::mutex mLogLock;

LogLevel Log::reportingLevel = LogInfo;
bool Log::dirty = false;
FILE* Log::file = NULL;

LogLevel Log::getReportingLevel()
{
	return reportingLevel;
}

std::string Log::getLogPath()
{
	std::string home = Utils::FileSystem::getHomePath();
	return home + "/.emulationstation/es_log.txt";
}

void Log::setReportingLevel(LogLevel level)
{
	reportingLevel = level;
}

void Log::init()
{
	std::unique_lock<std::mutex> lock(mLogLock);

	if (file != NULL)
		close();

	if (Settings::getInstance()->getString("LogLevel") == "disabled")
	{
		remove(getLogPath().c_str());
		return;
	}

	remove((getLogPath() + ".bak").c_str());

	// rename previous log file
	rename(getLogPath().c_str(), (getLogPath() + ".bak").c_str());

	file = fopen(getLogPath().c_str(), "w");
	dirty = false;
}

std::ostringstream& Log::get(LogLevel level)
{
	time_t t = time(nullptr);
	os << std::put_time(localtime(&t), "%F %T\t");

	switch (level)
	{
	case LogError:
		os << "ERROR\t";
		break;
	case LogWarning:
		os << "WARNING\t";
		break;
	case LogDebug:
		os << "DEBUG\t";
		break;
	default:
		os << "INFO\t";
		break;
	}

	messageLevel = level;

	return os;
}

void Log::flush()
{
	if (!dirty)
		return;

	if (file != nullptr)
		fflush(file);

	dirty = false;
}

void Log::close()
{
	if (file != NULL)
	{
		fflush(file);
		fclose(file);
	}

	dirty = false;
	file = NULL;
}

Log::~Log()
{
	std::unique_lock<std::mutex> lock(mLogLock);

	if (file != NULL)
	{
		os << std::endl;
		fprintf(file, "%s", os.str().c_str());
		dirty = true;
	}

	// If it's an error, also print to console
	// print all messages if using --debug
	if (messageLevel == LogError || reportingLevel >= LogDebug)
	{
#if WIN32
		OutputDebugStringA(os.str().c_str());
#else
		fprintf(stderr, "%s", os.str().c_str());
#endif
	}
}

void Log::setupReportingLevel()
{
	LogLevel lvl = LogInfo;

	if (Settings::getInstance()->getBool("Debug"))
		lvl = LogDebug;
	else
	{
		auto level = Settings::getInstance()->getString("LogLevel");
		if (level == "debug")
			lvl = LogDebug;
		else if (level == "information")
			lvl = LogInfo;
		else if (level == "warning")
			lvl = LogWarning;
		else if (level == "error")
			lvl = LogError;
	}

	setReportingLevel(lvl);
}
