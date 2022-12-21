///////////////////////////////////////////////////////////
//  manager.h
//  Implementation of the Class manager
//  Created on:      30-дек-2020 8:39:52
//  Original author: pm92
///////////////////////////////////////////////////////////

#if !defined(EA_BFC1DCFF_9001_469c_928D_64AC3E87F390__INCLUDED_)
#define EA_BFC1DCFF_9001_469c_928D_64AC3E87F390__INCLUDED_

#include "employee.h"
#include "store.h"
#include "supplier.h"

class manager : public employee
{

public:
	manager();
	virtual ~manager();

	void ask_supplier(employee worker);
	void check_money(unsigned long money);
	void deliver(store store, book* product, int amount);
	void pay_salary(employee pay);
	void pay_taxes();
	void store_money();

private:
	int shop_money;

};
#endif // !defined(EA_BFC1DCFF_9001_469c_928D_64AC3E87F390__INCLUDED_)
