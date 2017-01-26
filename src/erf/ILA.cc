#include "ILA.h"

#include <string>
#include <iostream>
#include <fstream>

void ILA::onNewMessage(string s)
{

	// Do authentication



	//write message to file
	writeFile(s);

	// update DB
	//updateDB();

}
void ILA::writeFile(string s)
{
	 ofstream    file;
	 file.open(filename.c_str(), ios_base::app);

	if (file.fail() == true)
	{
		return;
	}

	file << s;
	file.flush();
	file.close();


}

void ILA::updateDB(){

}
