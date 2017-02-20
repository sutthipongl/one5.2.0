#include <cassert>
#include <stdexcept>
#include <iostream>
#include <sstream>

#include <unistd.h>
#include <dirent.h>

using namespace std;

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>
#include "mysql/mysql.h"
#include <mysql/errmsg.h>

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
	ERSSecretRetrivalMethod(MYSQL* localdb): sdb(localdb){
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
    MYSQL* sdb;

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

class ERSService
{
public:

	ERSService(){
		cout <<  "ERS Version 1.1" << endl;

	db = mysql_init(NULL);

	  if (db == NULL)
	  {
		  cout << "ERSManager : can't init mysql " << mysql_error(db);
		 // exit(1);
	  }

	if (mysql_real_connect(db, "localhost","oneadmin","oneadmin", "opennebula",3306, NULL, 0)==NULL)
	  {
		  cout << "ERSManager : ERS DB Connection error" << endl;
		  cout << "ERSManager : Shutting down service" << endl;
		  exit(0);
	  }

	};

	MYSQL* getDB(){return db;};
	void registermethod();
	void run();


private:

xmlrpc_c::serverAbyss* ERSServer;
xmlrpc_c::registry ERSRegistry;
MYSQL* db;


};
