#ifndef _ILA_H_
#define _ILA_H_

#include <string>
#include <iostream> // std::cout, std::ios
#include <sstream> // std::ostringstream
#include "mysql/mysql.h"

using namespace std;


class ILA
{
public:
	ILA(const string fn):filename(fn){

		s_hash="";
		 ostringstream oss;

		// -----------------------------------------------------------
		// MYSQL Database
		// -----------------------------------------------------------

		// CREATE TABLE `opennebula`.`ERF` (
		//  `filename` VARCHAR(100) NOT NULL,
		//  `hash` VARCHAR(65) NULL,
		//  PRIMARY KEY (`filename`));
		// -----------------------------------------------------------

		cout <<  "ERF : MySQL client version:" << mysql_get_client_info() << endl;

		db = mysql_init(NULL);

		  if (db == NULL)
		  {
		      cout << "ERF : can't init mysql " << mysql_error(db);
		     // exit(1);
		  }

		if (mysql_real_connect(db, "localhost","oneadmin","oneadmin", NULL,3306, NULL, 0)==NULL)
		  {
			  cout << "ERF : ILA DB Connection error" << endl;
			  mysql_close(db);
			  //exit(1);
		  }

		oss.str("");
		oss << "USE opennebula;";

		//mysql_query return 0 if success
		// 0 means false in c++

		if (mysql_query(db,oss.str().c_str()))
		  {
				cout << "ERF : FAIL SELECT opennebula DATABASE" << endl;
				 mysql_close(db);
		  }

		// Insert new file with blank hash
		oss.str("");
		oss << "INSERT INTO ERF VALUES(\"" << filename << "\",\"\");";

		if (mysql_query(db,oss.str().c_str()))
		  {
				cout << "ERF : FAIL running " << oss.str() << endl;
				mysql_close(db);
		  }

	};
	void onNewMessage(string s);

private:

	void writeFile(string s);
	void writeDebugFile(string s);
	void updateDB(string curr_hash);

	string filename;
	string s_hash; // current hash
	MYSQL* db;
};


#endif /* _ILA_H_ */
