#include "ILA.h"
#include "NebulaUtil.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
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

		if (!isDBConnect)
		{
			writeERFFile("ERF : Reconnecting DB.......");
			// Try to re-connect
			if (mysql_real_connect(db, DBHost.c_str(),DBusername.c_str(),DBpasswd.c_str(), DBname.c_str(),DBPort, NULL, 0))
					{
							writeERFFile("ERF : Reconnected.");

							// check if it's ERFTEMP file
							if(filename.find("ERFTEMP")!=string::npos)
							{
								writeERFFile("ERF : Current log is ERFTEMP file , renaming......");
								string new_filename = getNextFileName(processname + "_" + currentDateTime(false));
								string new_filenamelog = new_filename + ".log";
								string new_filenameerf = new_filename + ".erf";

								string oldlog = filename;
								string olderf = erffile;

								int renresult ;


								renresult = rename(oldlog.c_str(),new_filenamelog.c_str());
								filename = new_filenamelog;

								// write msg in old ERF file
								if (renresult == 0)
									writeERFFile("ERF : Successfully rename " + oldlog + " to " + new_filenamelog );
								else
									writeERFFile("ERF : Fail rename " + oldlog + " to " + new_filenamelog );

								renresult = rename(olderf.c_str(),new_filenameerf.c_str());
								erffile = new_filenameerf;
								// write msg in new ERF file
								if (renresult == 0)
									writeERFFile("ERF : Successfully rename " + olderf + " to " + new_filenameerf );
								else
									writeERFFile("ERF : Fail rename " + olderf + " to " + new_filenameerf );


								writeERFFile("Current filename "+filename);
								writeERFFile("Current ERF file "+erffile);

								// Insert new .log file with initseed and current hash
								insertNewFileToDB(filename);

							}
					}
					else
					{
						writeERFFile("ERF : Reconnection attempt failed.");
					}
		}


		if(updateDB(s_hash)==0)
			{
					writeERFFile("ERF : Error update DB " + s_hash);

					//If previously it connected , but can't connect now ,
					if(isDBConnect)
					{
						writeLogFile("========= ERF OFFLINE ===========");
						isDBConnect=false;
					}

			}else
			{
				//If previously it disconnected from DB, but can connect now
				if(!isDBConnect)
					{
						writeLogFile("========= ERF ONLINE ============");
						isDBConnect=true;
					}
			}

		//write message to file
		writeLogFile(single_line);


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
		  	writeERFFile("ERF : FAIL execute " + oss.str());
			return 0;
	    }

	  writeERFFile("Update hash for file " + filename + " successfully" );
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

		oss << "INSERT INTO ERF VALUES(\"" << fn << "\",\""<<s_hash<<"\",\"" << initseed <<"\",\"" << currentDateTime(true) << "\");";

		if (mysql_query(db,oss.str().c_str()))
		  {
				writeERFFile("ERF : FAIL running " + oss.str());
				writeERFFile(mysql_error(db));
				return;

		  }

		writeERFFile("ERF : " +fn + " is inserted to DB");
}

void ILA::onDateChange()
{
	writeERFFile("onDateChange() called");

	//regenerate initial seed
	s_hash=getInitialSecret();
	initseed = s_hash;

	if(isDBConnect)
	{
		writeERFFile("ERF : DB connection is OK while Date Roll, roll next log..");
		//Update .log and .erf filename
		filename = processname + "_" + currentDateTime(false) + ".log";
		erffile = processname + "_" + currentDateTime(false) + ".erf";

		insertNewFileToDB(filename);
		writeERFFile("initial seed : "+initseed);

	}else
	{
		writeERFFile("ERF : DB connection FAIL while Date Roll, create unique ERFTEMP log..");
		 //Generate unique temp file, we will rename it once DB online
		unsigned int nanotime = getSystemNanotime();
		ostringstream temp;
		temp << nanotime;

		// Insert timestamp to file
		filename = processname + "_ERFTEMP_" + temp.str() + ".log";
		erffile = processname + "_ERFTEMP_" + temp.str() + ".erf";

		//write ERF OFFLINE since the beginning
		writeLogFile("========= ERF OFFLINE ===========");
	}



}

string ILA::getNextFileName(string fn_withdate)
{
	ostringstream oss;
			oss.str("");
			oss << "SELECT filename FROM ERF where filename like '" << fn_withdate << "%' order by filename DESC LIMIT 1;" ;

			if (mysql_query(db,oss.str().c_str()))
			  {
					writeERFFile("ERF : FAIL running " + oss.str());
					writeERFFile(mysql_error(db));

					 // If ILA connected DB but DBname is wrong, ILA can't find next number
					 //Generate unique temp file, we will rename it once DB online
					unsigned int nanotime = getSystemNanotime();
					ostringstream temp;
					temp << nanotime;

					return processname + temp.str() + "_ERFTEMP" ;

			  }else
			  {
				  MYSQL_RES *result = mysql_store_result(db);

				  MYSQL_ROW row = mysql_fetch_row(result);

				  if (row == NULL)
				    {
				       //Nothing return, This is the first file for that day
					  writeERFFile("ERF : No file in DB " + fn_withdate + " is the first one for this day" );
					  return fn_withdate;
				    }

				  string latestfilename = row[0] ;

				  //check if it's _YYYYMMDD or _YYYYMMDD_X
				  size_t n = count(latestfilename.begin(),latestfilename.end(),'_');
				  if (n>1)
				  {
					  // file format is _YYYYMMDD_XXX , find next number
					  int from = latestfilename.rfind("_")+1;
					  int to = latestfilename.rfind(".");
					  string num = latestfilename.substr(from, to - from);
					  writeERFFile("ERF : Latest file in DB is " + latestfilename);
					  writeERFFile("ERF : Latest file in DB number is "+num);
					  int m = atoi(num.c_str())+1;

					  //Patch zero
					  stringstream ss;
					  ss << setfill('0') << setw(4) << m;

					  return fn_withdate + "_" + ss.str() ;


				  }else
				  { 	// file format is _YYYYMMDD
					  return fn_withdate + "_0001";
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

string ILA::getInitialSecret()
{
	/* Decleration and initialization of static constant string type variable */
		static const string charList = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		srand(getSystemNanotime());

		string alphanumeric = "";

		for(int i = 0; i < 65; i++) {
			/* rand() generate random number */
			alphanumeric += charList [rand() % charList.size()];
		}

		return alphanumeric ;
}


unsigned int ILA::getSystemNanotime()
{
	timespec ts;
	clock_gettime(CLOCK_REALTIME,&ts);
	return ts.tv_nsec;

}
