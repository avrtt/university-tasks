:-dynamic(speaker/2).

save(File) :-
   tell(File), listing, told.

?- consult(lang_database),
   write("What's your name? "),
   read(Name),
   language(Lang),
   write("Do you speak "),
   write(Lang),
   write(" language (y/n)"),
   read(y),
   assert(speaker(Name, Lang)),
   save(lang_database),
   fail.
