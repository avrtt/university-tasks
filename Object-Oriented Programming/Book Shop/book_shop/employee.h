///////////////////////////////////////////////////////////
//  employee.h
//  Implementation of the Class employee
//  Created on:      30-дек-2020 8:39:52
//  Original author: pm92
///////////////////////////////////////////////////////////

#if !defined(EA_5CD145F0_D498_4da5_AB45_B0069DC099B9__INCLUDED_)
#define EA_5CD145F0_D498_4da5_AB45_B0069DC099B9__INCLUDED_

#include "human.h"
#include "store.h"

class employee : public human
{

public:
	employee();
	virtual ~employee();
	unsigned long salary;
	store *m_store;

	usigned long get_salary();
	void set_salary(unsigned long sal);

};
#endif // !defined(EA_5CD145F0_D498_4da5_AB45_B0069DC099B9__INCLUDED_)
