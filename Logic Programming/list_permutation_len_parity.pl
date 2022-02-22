even_permutation([], []).
even_permutation([X|T], Perm) :-
    even_permutation(T, Perm1),
    insert_odd(X, Perm1, Perm).
even_permutation([X|T], Perm) :-
    odd_permutation(T, Perm1),
    insert_even(X, Perm1, Perm).

odd_permutation([X|T], Perm) :-
    odd_permutation(T, Perm1),
    insert_odd(X, Perm1, Perm).
odd_permutation([X|T], Perm) :-
    even_permutation(T, Perm1),
    insert_even(X, Perm1, Perm).

insert_odd(X, InList, [X|InList]).
insert_odd(X, [Y,Z|InList], [Y,Z|OutList]) :-
    insert_odd(X, InList, OutList).

insert_even(X, [Y|InList], [Y,X|InList]).
insert_even(X, [Y,Z|InList], [Y,Z|OutList]) :-
    insert_even(X, InList, OutList).

even_len(List) :-
    length(List, Length),
    0 =:= Length mod 2.
 
odd_len(List) :-
    not(even_len(List)).
