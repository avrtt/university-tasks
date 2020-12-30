///////////////////////////////////////////////////////////
//  administrator.h
//  Implementation of the Class administrator
//  Created on:      30-дек-2020 8:39:51
//  Original author: pm92
///////////////////////////////////////////////////////////

#if !defined(EA_83DF3F64_0620_406b_BD49_7FC167BD3C50__INCLUDED_)
#define EA_83DF3F64_0620_406b_BD49_7FC167BD3C50__INCLUDED_

#include "employee.h"

class administrator : public employee
{

public:
	administrator();
	virtual ~administrator();

	int management(employee employee);
	void write_plan();

};
#endif // !defined(EA_83DF3F64_0620_406b_BD49_7FC167BD3C50__INCLUDED_)
