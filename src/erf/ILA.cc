#include "ILA.h"
#include "NebulaUtil.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;



void ILA::onNewMessage(string s)
{

	// break raw message to lines
	stringstream ss_raw(s);

	while (ss_raw.good())
	{
		string single_line;
		getline(ss_raw,single_line,'\n');

		// Do authentication
		s_hash = one_util::sha256_digest(single_line+s_hash);

		// DEBUG purpose
		writeERFFile("authening : "+single_line);
		writeERFFile(s_hash);

		//write message to file
		writeLogFile(single_line);

		// update DB
		if(updateDB(s_hash)==0)
			cout << "ERF : Error update DB " << s_hash << endl;
	}



}
void ILA::writeLogFile(string s)
{
	 ofstream    file;
	 file.open(filename.c_str(), ios_base::app);

	if (file.fail() == true)
	{
		return;
	}

	file << s << endl;
	file.flush();
	file.close();

}

int ILA::updateDB(string curr_hash){

	  ostringstream oss;
	  oss << "UPDATE ERF SET hash=\"" << curr_hash << "\",lastupdate=\"" << currentDateTime(true) << "\" WHERE filename=\""<<filename<<"\"";

	  if (mysql_query(db,oss.str().c_str()))
	    {
	  		cout << "ERF : FAIL execute " << oss.str() << endl;


			const char *    err_msg = mysql_error(db);
			int             err_num = mysql_errno(db);

			if( err_num == CR_SERVER_GONE_ERROR || err_num == CR_SERVER_LOST )
			{
				cout << "MySQL connection error " << err_num << " : " << err_msg << endl;

				// Try to re-connect
				if (mysql_real_connect(db, "localhost","oneadmin","oneadmin", "opennebula",3306, NULL, 0))
				{
					cout << "ERF : Reconnected." << endl;
				}
				else
				{
					cout << "ERF : Reconnection attempt failed." << endl;
				}
			}
			else
			{

				cout << "ERF : error " << err_num << " : " << err_msg << endl;
			}

			return 0;
	    }

	  return 1;

}


//DEBUG purpose
void ILA::writeERFFile(string s)
{
	 ofstream    file;

	 file.open(erffile.c_str(), ios_base::app);

	if (file.fail() == true)
	{
		return;
	}

	file << s << endl;
	file.flush();
	file.close();

}

void ILA::insertNewFileToDB(string fn)
{
		ostringstream oss;
		oss.str("");
		oss << "INSERT INTO ERF VALUES(\"" << fn << "\",\"\",\"" << currentDateTime(true) << "\");";

		if (mysql_query(db,oss.str().c_str()))
		  {
				cout << "ERF : FAIL running " << oss.str() << endl;
				cout << mysql_error(db) << endl;

		  }
}

void ILA::onFileNameChange(string newfn)
{
	cout << "onFilenameChange("<<newfn<<") called" << endl;

	//Update .log and .erf filename
	filename  = newfn;
	erffile = filename.substr(0,filename.length()-4) + ".erf";

	insertNewFileToDB(filename);


}

string ILA::getnextfilename(string fn_withdate)
{
	ostringstream oss;
			oss.str("");
			oss << "SELECT filename FROM ERF where filename like '" << fn_withdate << "%' order by filename DESC LIMIT 1;" ;

			if (mysql_query(db,oss.str().c_str()))
			  {
					cout << "ERF : FAIL running " << oss.str() << endl;
					cout << mysql_error(db) << endl;

			  }else
			  {
				  MYSQL_RES *result = mysql_store_result(db);

				  if (result == NULL)
				    {
				        //This is the first file for that day
					  return fn_withdate;
				    }

				  MYSQL_ROW row = mysql_fetch_row(result);

				  string latestfilename = row[0] ;

				  //check if it's _YYYYMMDD or _YYYYMMDD_X
				  size_t n = count(latestfilename.begin(),latestfilename.end(),'_');
				  if (n>1)
				  {
					  // file format is _YYYYMMDD_X , file next number

					  string num = latestfilename.substr(latestfilename.rfind("_")+1);
					  int m = atoi(num.c_str())+1;

					  stringstream ss;
					  ss << m;

					  return fn_withdate + "_" + ss.str() ;


				  }else
				  { // file format is _YYYYMMDD

					  return fn_withdate + "_1";
				  }
			  }

	return fn_withdate;
}

const string ILA::currentDateTime(bool withtime) {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    if (withtime)
    	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    else
    	 strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);

    return buf;
}


