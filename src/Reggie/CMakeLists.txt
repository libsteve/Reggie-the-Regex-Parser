set(reggie_SOURCES 
	automata.c
	automata_eval.c
	automata_operations.c
	nfa_builder.c
	nfa.c
	nfa_copy.c
	nfa_eval.c
	nfa_operations.c
	nfa_useful.c
	pda.c
	pda_eval.c
	regex_parser.c
	regex_tokenizer.c
	regex_tokenizer_nfa.c
	reggie.c)

add_library(reggie ${reggie_SOURCES})
target_link_libraries(reggie collection)
