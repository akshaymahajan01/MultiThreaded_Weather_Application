#include<iostream>
using namespace std;

#ifdef USER_INTERACTION_EXPORTS
#define USER_INTERACTION __declspec(dllexport)
#else
#define USER_INTERACTION __declspec(dllimport)
#endif


class USER_INTERACTION user_interaction
{
public:
	string city;
	string default_city = "solan";
	int  command;

	 string user_input();
	 int get_command();
};