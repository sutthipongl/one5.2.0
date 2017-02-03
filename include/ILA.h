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

		s_hash="";

		// -----------------------------------------------------------
		// MYSQL Database
		// -----------------------------------------------------------

//		CREATE TABLE `ERF` (
//		  `filename` varchar(100) NOT NULL,
//		  `hash` varchar(65) DEFAULT NULL,
//		  `lastupdate` datetime DEFAULT NULL,
//		  PRIMARY KEY (`filename`)
//		) ;

		// -----------------------------------------------------------

		ostringstream oss;
		cout <<  "ERF : MySQL client version:" << mysql_get_client_info() << endl;

		db = mysql_init(NULL);

		  if (db == NULL)
		  {
		      cout << "ERF : can't init mysql " << mysql_error(db);
		     // exit(1);
		  }

		if (mysql_real_connect(db, "localhost","oneadmin","oneadmin", "opennebula",3306, NULL, 0)==NULL)
		  {
			  cout << "ERF : ILA DB Connection error" << endl;
			  mysql_close(db);
		  }


		// Insert timestamp to file
		string temp = fn.substr(0,fn.length()-4);
		filename = temp + "_" + currentDateTime(false) + ".log";
		erffile = temp + "_" + currentDateTime(false) + ".erf";


		// Insert new .log file with blank hash
		insertNewFileToDB(filename);

	};
	void onNewMessage(string s);
	void onFileNameChange(string newfn);

private:

	void writeLogFile(string s);
	void writeERFFile(string s);
	int updateDB(string curr_hash);
	void insertNewFileToDB(string fn);
	const string currentDateTime(bool withtime);

	string filename;
	string erffile;
	string s_hash; // current hash
	MYSQL* db;
};


#endif /* _ILA_H_ */
