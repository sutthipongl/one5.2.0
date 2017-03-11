#ifndef _ILA_H_
#define _ILA_H_

#include <string>
#include <iostream> // std::cout, std::ios
#include <sstream> // std::ostringstream
#include "mysql/mysql.h"
#include <mysql/errmsg.h>

using namespace std;


class ILA
{
public:
	ILA(const string fn):filename(fn){

		s_hash=getInitialSecret();
		initseed = s_hash;
		isDBConnect = false;
	    processname = fn.substr(0,fn.length()-4);

	    DBHost = "localhost";
	    DBPort = 3306;

		// -----------------------------------------------------------
		// MYSQL Database
		// -----------------------------------------------------------

//		CREATE TABLE `ERF` (
//		  `filename` varchar(100) NOT NULL,
//		  `hash` varchar(65) DEFAULT NULL,
//	      `secret` varchar(65) NOT NULL,
//	      `lastupdate` datetime DEFAULT NULL,
//		  PRIMARY KEY (`filename`)
//		) ;

		// -----------------------------------------------------------

		ostringstream oss;
		cout <<  "ILA Version 1.3" << endl;
		cout <<  "ERF : MySQL client version:" << mysql_get_client_info() << endl;

		db = mysql_init(NULL);

		  if (db == NULL)
		  {
		      cout << "ERF : can't init mysql " << mysql_error(db) << endl;
		     // exit(1);
		  }

		if (mysql_real_connect(db, DBHost.c_str(),"oneadmin","oneadmin", "opennebula",DBPort, NULL, 0)==NULL)
		  {
			  cout << "ERF : ILA DB Connection error" << endl;

			  //Generate unique temp file, we will rename it once DB online
			  	unsigned int nanotime = getSystemNanotime();
			  	ostringstream temp;
			  	temp << nanotime;

				// Insert timestamp to file
				filename = processname + "_ERFTEMP_" + temp.str() + ".log";
				erffile = processname + "_ERFTEMP_" + temp.str() + ".erf";

				//write ERF OFFLINE since the beginning
				writeLogFile("========= ERF OFFLINE ===========");

		  }else
		  {
			  isDBConnect=true;
			  string temp_filename = getNextFileName(processname + "_" + currentDateTime(false));

			  filename = temp_filename + ".log";
			  erffile = temp_filename + ".erf";

			 // Insert new .log file with blank hash
			 insertNewFileToDB(filename);
		  }

		writeERFFile("initial seed : "+initseed);


	};
	void onNewMessage(string s);
	void onDateChange();

private:

	void writeLogFile(string s);
	void writeERFFile(string s);
	int updateDB(string curr_hash);
	void insertNewFileToDB(string fn);
	const string currentDateTime(bool withtime);
	string getNextFileName(string fn_withdate);
	string getInitialSecret();
	unsigned int getSystemNanotime();

	string processname;
	string filename;
	string erffile;
	string s_hash; // current hash
	string initseed; // remember first seed and insert to DB once DB back online
	string DBHost;
	unsigned int DBPort;
	MYSQL* db;
	bool isDBConnect;
};


#endif /* _ILA_H_ */
