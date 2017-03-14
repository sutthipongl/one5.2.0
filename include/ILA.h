#ifndef _ILA_H_
#define _ILA_H_

#include <string>
#include <iostream> // std::cout, std::ios
#include <sstream> // std::ostringstream
#include "mysql/mysql.h"
#include <mysql/errmsg.h>
#include "NebulaTemplate.h"

using namespace std;


class ILA
{
public:
	ILA(const string fn):filename(fn){

		onedconf = new OpenNebulaTemplate("/etc/one/", "/var/lib/one/");
		int rc = onedconf->load_configuration();

		    if ( rc != 0 )
		    {
		        throw runtime_error("ERF : Could not load nebula configuration file.");
		    }

		s_hash=getInitialSecret();
		initseed = s_hash;
		isDBConnect = false;
	    processname = fn.substr(0,fn.length()-4);

	    //ERFDB
	    DBHost = "localhost";
	  	DBPort = 3306;
	  	DBname = "opennebula";
		DBusername = "oneadmin";
		DBpasswd = "oneadmin";

		 const VectorAttribute * _erfdb = onedconf->get("ERFDB");

		 if ( _erfdb != 0 )
		 {
			 DBHost = _erfdb->vector_value("SERVER");
			 DBusername = _erfdb->vector_value("USER");
			 DBpasswd = _erfdb->vector_value("PASSWD");
			 DBname = _erfdb->vector_value("DB_NAME");
			 string port =  _erfdb->vector_value("PORT");

			 istringstream   is;
			 is.str(port);
			 is >> DBPort;
		 }

		 cout << "ERFDB HOST " << DBHost << endl;
		 cout << "ERFDB Port " << DBPort << endl;
		 cout << "ERFDB NAME " << DBname << endl;
		 cout << "ERFDB USER " << DBusername << endl;
		 cout << "ERFDB PASSWD " << DBpasswd << endl;



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
		cout <<  "ILA Version 1.4 - 9 Mar 2017" << endl;
		cout <<  "ERF : MySQL client version:" << mysql_get_client_info() << endl;

		db = mysql_init(NULL);

		  if (db == NULL)
		  {
		      cout << "ERF : can't init mysql " << mysql_error(db) << endl;
		     // exit(1);
		  }

		if (mysql_real_connect(db, DBHost.c_str(),DBusername.c_str(),DBpasswd.c_str(), DBname.c_str(),DBPort, NULL, 0)==NULL)
		  {
			  	 cout << "ERF : ILA DB Connection error" << endl;

			  //Generate unique temp file, we will rename it once DB online
			  	unsigned int nanotime = getSystemNanotime();
			  	ostringstream temp;
			  	temp << nanotime;

				// Insert timestamp to file
				filename = processname + temp.str() + "_ERFTEMP" + ".log";
				erffile = processname + temp.str()  + "_ERFTEMP" + ".erf";

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
	OpenNebulaTemplate * onedconf;
	string erffile;
	string s_hash; // current hash
	string initseed; // remember first seed and insert to DB once DB back online
	string DBHost;
	unsigned int DBPort;
	string DBname;
	string DBusername;
	string DBpasswd;
	MYSQL* db;
	bool isDBConnect;
};


#endif /* _ILA_H_ */
