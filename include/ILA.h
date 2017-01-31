#ifndef _ILA_H_
#define _ILA_H_

#include <string>
//#include "SqlDB.h"
//#include "MySqlDB.h"

using namespace std;

class ILA
{
public:
	ILA(const string fn):filename(fn){

		s_hash="";

		// -----------------------------------------------------------
		// MYSQL Database
		// -----------------------------------------------------------

//		CREATE TABLE `opennebula`.`ERF` (
//		  `filename` VARCHAR(100) NOT NULL,
//		  `hash` VARCHAR(45) NULL,
//		  PRIMARY KEY (`filename`));
//
	/*	try
		{
			ILADB = new MySqlDB("localhost", 3306, "oneadmin", "oneadmin", "opennebula");

			//INSERT NEW RECORD in DB

			ostringstream oss;
			oss << "INSERT INTO ERF VALUES (" << filename << "," << s_hash << ");";

			ILADB->exec(oss);


		}
		catch (exception&)
		{
			throw;
		}*/

	};
	void onNewMessage(string s);

private:

	void writeFile(string s);
	void writeDebugFile(string s);
	void updateDB(string curr_hash);

	string filename;
	string s_hash; // current hash
	//MySqlDB * ILADB;

};


#endif /* _ILA_H_ */
