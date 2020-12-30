///////////////////////////////////////////////////////////
//  recruiter.h
//  Implementation of the Class recruiter
//  Created on:      30-дек-2020 8:39:52
//  Original author: pm92
///////////////////////////////////////////////////////////

#if !defined(EA_D80DECC6_F2C3_433a_9372_41E3392D4710__INCLUDED_)
#define EA_D80DECC6_F2C3_433a_9372_41E3392D4710__INCLUDED_

#include "employee.h"
#include "human.h"

class recruiter : public employee
{

public:
	recruiter();
	virtual ~recruiter();

	int fire(employee worker);
	int hire(human human);

};
#endif // !defined(EA_D80DECC6_F2C3_433a_9372_41E3392D4710__INCLUDED_)
