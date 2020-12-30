///////////////////////////////////////////////////////////
//  cleaner.h
//  Implementation of the Class cleaner
//  Created on:      30-дек-2020 8:39:52
//  Original author: pm92
///////////////////////////////////////////////////////////

#if !defined(EA_F3944AAC_36A7_4246_90D4_BD92A00A977D__INCLUDED_)
#define EA_F3944AAC_36A7_4246_90D4_BD92A00A977D__INCLUDED_

#include "employee.h"

class cleaner : public employee
{

public:
	cleaner();
	virtual ~cleaner();

	void clean();

};
#endif // !defined(EA_F3944AAC_36A7_4246_90D4_BD92A00A977D__INCLUDED_)
