:-dynamic(staff/3).

save(File) :-
	tell(File), listing, told.

choice(1):- 
	write("Full name: "),
	read(Name),
	write("Status: "),
	read(Status),
	write("Email: "),
   	read(Email),
	assert(staff(Name, Status, Email)).

choice(2):-
	staff(Name, Status, Email),
	write(Name), nl,
	write(Status), nl,
	write(Email), nl,
	write("\n"), nl.

choice(3):- 
	consult(department_database),
	write("Loaded."), nl.

choice(4):- 
	save(department_database),
	write("Saved."), nl.

choice(0):- !.

menu :-
	repeat,
	write("------------------------------\n"),
	write("1 - add\n"),
	write("2 - show\n"),
	write("3 - load\n"),
	write("4 - save\n"),
	write("0 - exit\n"),
	read(Choice),
	choice(Choice),
	Choice=0,
	!.

?- menu.
