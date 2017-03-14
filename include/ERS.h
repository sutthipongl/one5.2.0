#include <cassert>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <dirent.h>

using namespace std;

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>
#include "mysql/mysql.h"
#include <mysql/errmsg.h>


class ERSService
{
public:

	ERSService(){

		 //ERFDB

		DBHost = "localhost";
		DBPort = 3306;
		DBname = "opennebula";
		DBusername = "oneadmin";
		DBpasswd = "oneadmin";

		string confline;
		ifstream ersconfig("/etc/one/ers.conf");

		if (ersconfig.is_open())
		{
			while ( getline (ersconfig,confline) )
				{
					if(confline.length()!=0)
					{
						string key = confline.substr(0,confline.find("=")) ;
						string value = confline.substr(confline.find("=")+1,confline.length());
						cout << "ERS.cnf : " << key << endl;
						cout << "ERS.cnf : " << value << endl;

						if(key.compare("ERSDBHost")==0)
							DBHost = value;
						else if (key.compare("ERSDBPort")==0)
						{
							istringstream   is;
							is.str(value);
							is >> DBPort;
						}
						else if (key.compare("ERSDBName")==0)
							DBname = value;
						else if (key.compare("ERSDBUsername")==0)
							DBusername = value;
						else if (key.compare("ERSDBPassword")==0)
							DBpasswd = value;
					}

				}
			ersconfig.close();
		}

	  else cout << "Unable to open ERS config file, use default";


		 cout << "ERSDB HOST " << DBHost << endl;
		 cout << "ERSDB Port " << DBPort << endl;
		 cout << "ERSDB NAME " << DBname << endl;
		 cout << "ERSDB USER " << DBusername << endl;
		 cout << "ERSDB PASSWD " << DBpasswd << endl;


		cout <<  "ERS Version 1.3 Build 10 Mar 2017" << endl;

		db = mysql_init(NULL);

		if (db == NULL)
		{
			cout << "ERSManager : can't init mysql " << mysql_error(db) << endl;
		}else
		{
			if (mysql_real_connect(db, DBHost.c_str(),DBusername.c_str(),DBpasswd.c_str(), DBname.c_str(),DBPort, NULL, 0)==NULL)
				  {
					  cout << "ERSManager : ERS DB Connection error" << endl;

				  }else
				  {
					  cout << "ERS : DB connected" << endl;
					  isDBCon = true;
				  }
		}
	};

	MYSQL* getDB(){return db;};
	bool isDBConnected(){return isDBCon;};
	bool reconnectDB();
	void registermethod();
	void run();


private:
	string DBHost;
	unsigned int DBPort;
	string DBname;
	string DBusername;
	string DBpasswd;
	xmlrpc_c::serverAbyss* ERSServer;
	xmlrpc_c::registry ERSRegistry;
	MYSQL* db;
	bool isDBCon = false;


};

class ERSLogAcquisitionMethod : public xmlrpc_c::method {
public:
	ERSLogAcquisitionMethod() {
        // signature and help strings are documentation -- the client
        // can query this information with a system.methodSignature and
        // system.methodHelp RPC.
        this->_signature = "i:ii";
            // method's result and two arguments are integers
        this->_help = "ERSLogAcquisitionMethod";
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) ;

	string get_file_contents(const char *filename);


};

class ERSSecretRetrivalMethod : public xmlrpc_c::method {
public:
	ERSSecretRetrivalMethod(ERSService* ersobj): ers(ersobj){
        // signature and help strings are documentation -- the client
        // can query this information with a system.methodSignature and
        // system.methodHelp RPC.
        this->_signature = "i:ii";
            // method's result and two arguments are integers
        this->_help = "ERSSecretRetrivalMethod";
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) ;

private:
    ERSService* ers;


};

class ERSGetFileListMethod : public xmlrpc_c::method {
public:
	ERSGetFileListMethod(){

        this->_signature = "i:ii";
            // method's result and two arguments are integers
        this->_help = "ERSGetFileListMethod";
    }
    void
    execute(xmlrpc_c::paramList const& paramList,
            xmlrpc_c::value *   const  retvalP) ;

};


