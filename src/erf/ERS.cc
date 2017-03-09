#include "ERS.h"
#include "iostream"
#include "fstream"


using namespace std;

// Get file
void ERSLogAcquisitionMethod::execute(xmlrpc_c::paramList const& paramList,
        xmlrpc_c::value *   const  retvalP){

	cout << "ERS : ERSLogAcquisitionMethod called" << endl;

	 string const fn(paramList.getString(0));
	 string fullfilepath  = "/var/log/one/"+fn;

    *retvalP = xmlrpc_c::value_string(get_file_contents(fullfilepath.c_str()));

}

string ERSLogAcquisitionMethod::get_file_contents(const char *filename)
{
  ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }

  return "";
}

// Get hash for a file
void ERSSecretRetrivalMethod::execute(xmlrpc_c::paramList const& paramList,
        xmlrpc_c::value *   const  retvalP){

	cout << "ERS : ERSSecretRetrivalMethod called" << endl;

    string const fn(paramList.getString(0));

    paramList.verifyEnd(1);

    ostringstream oss;
   	oss << "SELECT hash,secret from ERF WHERE filename=\"/var/log/one/"<<fn<<"\"";

   	if(!ers->isDBConnected())
   		ers->reconnectDB();


   	if(ers->isDBConnected())
   	{
   		if (mysql_query(ers->getDB(),oss.str().c_str()))
   			  {
   					cout << "ERSSecretRetrivalMethod : FAIL running " << oss.str() << endl;
   					cout << mysql_error(ers->getDB()) << endl;

   					//can't connect DB, return empty result
   					*retvalP = xmlrpc_c::value_string(" | ");

   			  }else
   			  {
   				  MYSQL_RES *result = mysql_store_result(ers->getDB());

   				  MYSQL_ROW row = mysql_fetch_row(result);

   				  if (row == NULL)
   					{
   					   //Nothing return, This is the first file for that day
   					  cout << "ERSSecretRetrivalMethod : Can't find secret for " << fn << endl;
   					}

   				  string hash = row[0] ;
   				  string secret = row[1];

   				  ostringstream returnstr;
   				  returnstr << hash << "|" << secret;

   				  cout << "ERSSecretRetrivalMethod return : " << hash << "|" << secret << endl;

   				  *retvalP = xmlrpc_c::value_string(returnstr.str());

   			  }
   	}else
   	{
   		*retvalP = xmlrpc_c::value_string(" | ");
   	}


}

// get *.log in /var/log/one
void ERSGetFileListMethod::execute(xmlrpc_c::paramList const& paramList,
        xmlrpc_c::value *   const  retvalP){

	cout << "ERS : ERSGetFileListMethod called" << endl;
    string result="";

	 DIR *dir;
	 struct dirent *ent;
	 if ((dir = opendir ("/var/log/one/")) != NULL) {

	   while ((ent = readdir (dir)) != NULL) {

	     result.append(ent->d_name);
	     result.append("\n");
	   }
	   closedir (dir);
	 } else {
	   cout << "ERS can't open /var/log/one directory" << endl;
	 }

    *retvalP = xmlrpc_c::value_string(result);

}

void ERSService::registermethod(){

	// Create ERS method object
	xmlrpc_c::methodPtr ERSLogAcquisitionMethodP(new ERSLogAcquisitionMethod());
	xmlrpc_c::methodPtr ERSSecretRetrivalMethodP(new ERSSecretRetrivalMethod(this));
	xmlrpc_c::methodPtr ERSGetFileListMethodP(new ERSGetFileListMethod());

	//Register methods
	ERSRegistry.addMethod("ERS.logacquire", ERSLogAcquisitionMethodP);
	ERSRegistry.addMethod("ERS.retrievesecret", ERSSecretRetrivalMethodP);
	ERSRegistry.addMethod("ERS.listfile", ERSGetFileListMethodP);
}

bool ERSService::reconnectDB()
{
	if (mysql_real_connect(db, DBHost.c_str(),"oneadmin","oneadmin", "opennebula",DBPort, NULL, 0)==NULL)
		  {
			  cout << "ERSManager : ERS DB Connection error" << endl;
			  isDBCon = false;
			  return false;
		  }else
		  {
			  cout << "ERS : DB connected" << endl;
			  isDBCon = true;
			  return true;
		  }
}
void ERSService::run(){

	try{

			xmlrpc_c::serverAbyss::constrOpt opt = xmlrpc_c::serverAbyss::constrOpt();

			opt.registryP(&ERSRegistry);
			opt.keepaliveTimeout(10);
			opt.keepaliveMaxConn(10);
			opt.timeout(10);
			//opt.socketFd(8080);
			opt.portNumber(8080);

			ERSServer = new xmlrpc_c::serverAbyss(opt);

		    cout << "ERF : ERS server start...." << endl;
			ERSServer->run(); //never return
			assert(false);

		} catch (exception const& e) {

			cerr << "ERS RPC Server start failed " << e.what() << endl;

		}
}


int main(int argc, char **argv)
{
			ERSService ers ;
			ers.registermethod();
			ers.run();

}
