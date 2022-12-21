///////////////////////////////////////////////////////////
//  advisor.h
//  Implementation of the Class advisor
//  Created on:      30-дек-2020 8:39:52
//  Original author: pm92
///////////////////////////////////////////////////////////

#if !defined(EA_CD3F5DFC_C2A2_4f01_BAA9_CE5C2A75D4E9__INCLUDED_)
#define EA_CD3F5DFC_C2A2_4f01_BAA9_CE5C2A75D4E9__INCLUDED_

#include "human.h"
#include "employee.h"

class advisor : public employee
{

public:
	advisor();
	virtual ~advisor();

	void advice(human human);

};
#endif // !defined(EA_CD3F5DFC_C2A2_4f01_BAA9_CE5C2A75D4E9__INCLUDED_)
