///////////////////////////////////////////////////////////
//  cashier.h
//  Implementation of the Class cashier
//  Created on:      30-дек-2020 8:39:52
//  Original author: pm92
///////////////////////////////////////////////////////////

#if !defined(EA_78D72BE3_CFD6_43e4_BB10_13BA0997A731__INCLUDED_)
#define EA_78D72BE3_CFD6_43e4_BB10_13BA0997A731__INCLUDED_

#include "client.h"
#include "store.h"
#include "employee.h"

class cashier : public employee
{

public:
	cashier();
	virtual ~cashier();

	int give_book(store::book product, client buyer);
	int print_check(store::book product, client buyer);
	int take_money(store::book product, client buyer);

private:
	int cash_money;

};
#endif // !defined(EA_78D72BE3_CFD6_43e4_BB10_13BA0997A731__INCLUDED_)
