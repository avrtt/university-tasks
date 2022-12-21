///////////////////////////////////////////////////////////
//  client.h
//  Implementation of the Class client
//  Created on:      30-дек-2020 8:39:52
//  Original author: pm92
///////////////////////////////////////////////////////////

#if !defined(EA_BB2E4376_ED55_4ec9_9A38_B56C2E4DC13C__INCLUDED_)
#define EA_BB2E4376_ED55_4ec9_9A38_B56C2E4DC13C__INCLUDED_

#include "advisor.h"
#include "store.h"
#include "cashier.h"
#include "human.h"

class client : public human
{

public:
	client();
	virtual ~client();

	void ask_advice(advisor advisor);
	void buy(book product, cachier c);
	void review(store::book product);

};
#endif // !defined(EA_BB2E4376_ED55_4ec9_9A38_B56C2E4DC13C__INCLUDED_)
