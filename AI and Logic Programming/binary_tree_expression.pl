%       +
%     /  \
%   3     *     2 * 5 + 3 = 13
%        / \
%      5    2

% tree(+, tree(3, nil, nil), tree(*, tree(5, nil, nil), tree(2, nil, nil))).

ex_tree(Tree, Ex) :- 
    is_tree(Tree),
    eval_ex(Tree, Ex).

is_tree(nil).
is_tree(tree(_, L, R)) :- 
    is_tree(L), 
    is_tree(R).
 
eval_ex(tree(X, nil, nil), X).
eval_ex(tree(+, L, R), X0) :- !,
    eval_ex(R, R1),
	eval_ex(L, L1),
    X0 is L1 + R1.
eval_ex(tree(-, L, R), X0) :- !,
    eval_ex(R, R1),
	eval_ex(L, L1),
    X0 is L1 - R1.
eval_ex(tree(*, L, R), X0) :- !,
    eval_ex(R, R1),
	eval_ex(L, L1),
    X0 is L1 * R1.
eval_ex(tree(/, L, R), X0) :- !,
    eval_ex(R, R1),
	eval_ex(L, L1),
    X0 is L1 / R1.
