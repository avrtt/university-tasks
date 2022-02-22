:-dynamic(speaker/2).

save(File) :-
   tell(File), listing, told.

choice(1):- 
	write("What's your name? "),
   read(Name),
   language(Lang),
   write("Do you speak "),
   write(Lang),
   write(" language (y/n)"),
   read(y),
   assert(speaker(Name, Lang)).

choice(2):-
	speaker(Name, Lang),
	write(Name),
	write(" speaks "),
	write(Lang), nl.

choice(3):- 
	consult(lang_database),
	write("Loaded."), nl.

choice(4):- 
	save(lang_database),
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
