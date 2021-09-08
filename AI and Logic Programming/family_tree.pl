:- discontiguous(sister/2).
:- discontiguous(brother/2).
:- discontiguous(parent/2).
:- discontiguous(male/1).
:- discontiguous(female/1).

% 1ST GENERATION
male(jack).
female(jenny).

parent(jack, mike).
parent(jack, chloe).
parent(jenny, mike).
parent(jenny, chloe).

% 2ND GENERATION
brother(mike, chloe).
sister(chloe, mike).

parent(mike, bob).
parent(mike, mildred).
parent(anne, bob).
parent(anne, mildred).
parent(chloe, jim).
parent(chloe, sophie).
parent(harry, jim).
parent(harry, sophie).

female(anne).
male(harry).

% 3RD GENERATION
brother(bob, mildred).
brother(jim, sophie).
sister(mildred, bob).
sister(sophie, jim).

parent(bob, james).
parent(bob, lily).
parent(rosie, james).
parent(rosie, lily).
parent(mildred, jared).
parent(mildred, mia).
parent(tom, jared).
parent(tom, mia).
parent(jim, rick).
parent(jim, emma).
parent(liza, rick).
parent(liza, emma).
parent(sophie, josh).
parent(sophie, helen).
parent(oliver, josh).
parent(oliver, helen).

female(rosie).
male(tom).
female(liza).
male(oliver).

% 4TH GENERATION
brother(james, lily).
brother(jared, mia).
brother(rick, emma).
brother(josh, helen).
sister(lily, james).
sister(mia, jared).
sister(emma, rick).
sister(helen, josh).


male(X) :- brother(X, _).
female(X) :- sister(X, _).

% RULES
child(X, Y) :- parent(Y, X).
son(X, Y) :- parent(Y, X) , male(X).
daughter(X, Y) :- parent(Y, X) , female(X).
father(X, Y) :- parent(X, Y) , male(X).
mother(X, Y) :- parent(X, Y) , female(X).
grandparent(X, Y) :- parent(X, Z) , parent(Z, Y).
grandfather(X, Y) :- grandparent(X, Y) , male(X).
grandmother(X, Y) :- grandparent(X, Y) , female(X).
grandchild(X, Y) :- grandparent(Y, X).
grandson(X, Y) :- grandchild(X, Y) , male(X).
granddaughter(X, Y) :- grandchild(X, Y) , female(X).
greatgrandparent(X, Y) :- grandparent(X, Z) , parent(Z, Y).
greatgrandfather(X, Y) :- greatgrandparent(X, Y) , male(X).
greatgrandmother(X, Y) :- greatgrandparent(X, Y) , female(X).
greatgrandchild(X, Y) :- greatgrandparent(Y, X).
greatgrandson(X, Y) :- greatgrandchild(Y, X), male().
greatgranddaughter(X, Y) :- greatgrandchild(Y, X), female().
uncle(X, Y) :- parent(Z, Y) , brother(X, Z).
aunt(X, Y) :- parent(Z, Y) , sister(X, Z).
cousin(X, Y) :- uncle(Z, X) , father(Z, Y).
nephew(X, Y) :- (uncle(Y, X) ; aunt(Y, X)) , male(X).
niece(X, Y) :- (uncle(Y, X) ; aunt(Y, X)) , female(X).
sibling(X, Y) :- brother(X, Y) ; sister(X, Y).
ancestor(X, Y) :- child(X, Y) ; grandchild(X, Y) ; greatgrandchild(X, Y). 
descendant(X, Y) :- ancestor(Y, X).
