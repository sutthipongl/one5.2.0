#include <cassert>
#include <stdexcept>
#include <iostream>
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

	string DBHost;
	int DBPort;

	ERSService(){

		DBHost = "192.168.1.4";
		DBPort = 3306;

		cout <<  "ERS Version 1.2 Build 4 Mar 2017" << endl;

		db = mysql_init(NULL);

		if (db == NULL)
		{
			cout << "ERSManager : can't init mysql " << mysql_error(db) << endl;
		}else
		{
			if (mysql_real_connect(db, DBHost.c_str(),"oneadmin","oneadmin", "opennebula",DBPort, NULL, 0)==NULL)
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


