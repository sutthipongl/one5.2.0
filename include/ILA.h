#ifndef _ILA_H_
#define _ILA_H_

#include <string>

using namespace std;

class ILA
{
public:
	ILA(const string fn):filename(fn){};
	void onNewMessage(string s);

private:

	void writeFile(string s);
	void updateDB();

	string filename;
	string s_hash; // current hash


};


#endif /* _ILA_H_ */
