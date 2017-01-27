/* -------------------------------------------------------------------------- */
/* Copyright 2002-2016, OpenNebula Project, OpenNebula Systems                */
/*                                                                            */
/* Licensed under the Apache License, Version 2.0 (the "License"); you may    */
/* not use this file except in compliance with the License. You may obtain    */
/* a copy of the License at                                                   */
/*                                                                            */
/* http://www.apache.org/licenses/LICENSE-2.0                                 */
/*                                                                            */
/* Unless required by applicable law or agreed to in writing, software        */
/* distributed under the License is distributed on an "AS IS" BASIS,          */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   */
/* See the License for the specific language governing permissions and        */
/* limitations under the License.                                             */
/* -------------------------------------------------------------------------- */

#include "Log.h"

#include <string.h>
#include <stdlib.h>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include <sys/types.h>
#include <unistd.h>

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

const char Log::error_names[] ={ 'E', 'W', 'I', 'D', 'D', 'D' };

unsigned int Log::zone_id = 0;

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

FileLog::FileLog(const string&   file_name,
                 const MessageType   level,
                 ios_base::openmode  mode)
        :Log(level), log_file_name(file_name)
{
    ofstream file;

    file.open(log_file_name.c_str(), mode);

    if (file.fail() == true)
    {
        throw runtime_error("Could not open log file");
    }

    if ( file.is_open() == true )
    {
        file.close();
    }
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

FileLog::~FileLog() { }

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

void FileLog::log(
    const char *            module,
    const MessageType       type,
    const char *            message)
{
    char        str[26];
    time_t      the_time;
    ofstream    file;

    if( type <= log_level)
    {
        file.open(log_file_name.c_str(), ios_base::app);

        if (file.fail() == true)
        {
            return;
        }

        the_time = time(NULL);

#ifdef SOLARIS
        ctime_r(&(the_time),str,sizeof(char)*26);
#else
        ctime_r(&(the_time),str);
#endif
        // Get rid of final enter character
        str[24] = '\0';

        file << str << " ";
        file << "[Z"<< zone_id<< "]";
        file << "[" << module << "]";
        file << "[" << error_names[type] << "]: ";
        file << message;
        file << endl;

        file.flush();

        file.close();
    }
}

ERFFileLog::ERFFileLog(const string&   file_name,
                 const MessageType   level,
                 ios_base::openmode  mode)
        :Log(level), log_file_name(file_name)
{
	// Insert timestamp to file
	string fn = file_name.substr(0,file_name.length()-4);
	log_file_name = fn + "_" + currentDateTime() + ".log";


    ofstream file;

    file.open(log_file_name.c_str(), mode);

    if (file.fail() == true)
    {
        throw runtime_error("Could not open log file");
    }

    if ( file.is_open() == true )
    {
        file.close();
    }

    authen = new ILA(log_file_name);

}

const string ERFFileLog::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct);

    return buf;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

ERFFileLog::~ERFFileLog() { }

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


void ERFFileLog::log(
    const char *            module,
    const MessageType       type,
    const char *            message)
{
    char        str[26];
    time_t      the_time;

    if( type <= log_level)
    {

        the_time = time(NULL);

#ifdef SOLARIS
        ctime_r(&(the_time),str,sizeof(char)*26);
#else
        ctime_r(&(the_time),str);
#endif
        // Get rid of final enter character
              str[24] = '\0';

              string temp ;
              stringstream ss;
              ss << zone_id ;

              temp.append(str);
              temp.append("[Z");
              temp.append(ss.str());
              temp.append("][");
              temp.append(module);
              temp.append("][");
              temp+=error_names[type];
              temp.append("][ERF]: ");
              temp.append(message);
              temp.append("\n");

              authen->onNewMessage(temp);
    }
}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

StdLog::StdLog(const MessageType level, int oid,
       const PoolObjectSQL::ObjectType obj_type):Log(level)
{
    ostringstream oss;

    oss << "[" << PoolObjectSQL::type_to_str(obj_type) << " " << oid << "]";
    resource_label = oss.str();
}

/* -------------------------------------------------------------------------- */

void StdLog::log(
    const char *            module,
    const MessageType       type,
    const char *            message)
{
    char        str[26];
    time_t      the_time;
    ofstream    file;

    if( type <= log_level)
    {
        the_time = time(NULL);

#ifdef SOLARIS
        ctime_r(&(the_time), str, sizeof(char)*26);
#else
        ctime_r(&(the_time), str);
#endif
        // Get rid of final enter character
        str[24] = '\0';

        std::clog << str << " "
				  << resource_label
                  << "[Z"<< zone_id<< "]"
                  << "[" << module << "]"
                  << "[" << error_names[type] << "]: "
                  << message << endl;

        std::clog.flush();
    }
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

SysLog::SysLog(const MessageType level,
               const string&     label):Log(level), resource_label("")
{
    static bool initialized = false;

    if (!initialized) //Initialize just once for all SysLog instances
    {
        openlog(label.c_str(), LOG_PID, LOG_DAEMON);

        initialized = true;
    }
};

/* -------------------------------------------------------------------------- */

SysLog::SysLog(const MessageType level, int oid,
    const PoolObjectSQL::ObjectType obj_type):Log(level)
{
    ostringstream oss;

    oss << "[" << PoolObjectSQL::type_to_str(obj_type) << " " << oid << "]";
    resource_label = oss.str();
}

/* -------------------------------------------------------------------------- */

void SysLog::log(
    const char *            module,
    const MessageType       type,
    const char *            message)
{
    if( type <= log_level)
    {
        istringstream smessage(message);
        string        line;

        int slevel = level(type);

        while ( getline(smessage, line) )
        {
            ostringstream oss;

            oss << resource_label
				<< "[Z"<< zone_id<< "]"
                << "[" << module << "]"
                << "[" << error_names[type] << "]: "
                << line;

            syslog(slevel, "%s", oss.str().c_str());
        }
    }
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

