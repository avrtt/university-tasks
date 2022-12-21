///////////////////////////////////////////////////////////
//  supplier.h
//  Implementation of the Class supplier
//  Created on:      30-Dec-2020 8:39:52
//  Original author: pm92
///////////////////////////////////////////////////////////

#if !defined(EA_164800A8_E500_4898_B194_99ECCDB4FED2__INCLUDED_)
#define EA_164800A8_E500_4898_B194_99ECCDB4FED2__INCLUDED_

#include "store.h"
#include "human.h"

class supplier : public human
{

public:
	supplier();
	virtual ~supplier();

	void deliver(store store, book* product, int amount);

};
#endif // !defined(EA_164800A8_E500_4898_B194_99ECCDB4FED2__INCLUDED_)
