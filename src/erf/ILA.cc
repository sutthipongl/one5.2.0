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
		writeDebugFile("authening : "+single_line);
		writeDebugFile(s_hash);

		//write message to file
		writeFile(single_line);

		// update DB
		updateDB(s_hash);
	}



}
void ILA::writeFile(string s)
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

void ILA::updateDB(string curr_hash){

//	  ostringstream oss;
//	  oss << "SELECT last_oid FROM pool_control WHERE filename='"<<filename<<"'";
//	  ILADB->exec(oss);
}


//DEBUG purpose
void ILA::writeDebugFile(string s)
{

	 ofstream    file;
	 file.open("/var/log/one/debug.log", ios_base::app);

	if (file.fail() == true)
	{
		return;
	}

	file << s << endl;
	file.flush();
	file.close();


}

