///////////////////////////////////////////////////////////
//  human.h
//  Implementation of the Class human
//  Created on:      30-дек-2020 8:39:52
//  Original author: pm92
///////////////////////////////////////////////////////////

#if !defined(EA_E515F8D0_9F4C_45d2_B846_CECA1E3B0586__INCLUDED_)
#define EA_E515F8D0_9F4C_45d2_B846_CECA1E3B0586__INCLUDED_

class human
{

public:
	human();
	virtual ~human();

	char* get_name();
	void set_name(char* name);

private:
	char name[255];

};
#endif // !defined(EA_E515F8D0_9F4C_45d2_B846_CECA1E3B0586__INCLUDED_)
