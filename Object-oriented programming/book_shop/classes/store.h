///////////////////////////////////////////////////////////
//  store.h
//  Implementation of the Class store
//  Created on:      30-дек-2020 8:39:52
//  Original author: pm92
///////////////////////////////////////////////////////////

#if !defined(EA_71D1D215_082D_4e9a_9CF2_CE9E8A0B571E__INCLUDED_)
#define EA_71D1D215_082D_4e9a_9CF2_CE9E8A0B571E__INCLUDED_

class store
{

public:
	store();
	virtual ~store();
	unsigned int amount_books;
	unsigned int amount_of_employee;
	store::book *m_book;

	void add_book(book b);
	int* get_adress();
	book get_book(char* name);
	void set_adress(int* adress);
	void set_amount_books(int size);

private:
	struct book
	{

	public:
		char name[255];
		unsigned long price;
		int rating;

	};
	unsigned int address[5];

};
#endif // !defined(EA_71D1D215_082D_4e9a_9CF2_CE9E8A0B571E__INCLUDED_)
