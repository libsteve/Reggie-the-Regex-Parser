if(TEST)
	add_executable(list-test list-test.c)
	# Extra linking for the project.
	target_link_libraries(list-test reggie)
	# This is so you can do 'make test' to see all your tests run, instead of
	# manually running the executable runUnitTests to see those specific tests.
	add_test(NAME lists COMMAND list-test)

	add_executable(map-test map-test.c)
	target_link_libraries(map-test reggie)
	add_test(NAME maps COMMAND map-test)

	add_executable(nfa_copy-test nfa_copy-test.c)
	target_link_libraries(nfa_copy-test reggie)
	add_test(NAME nfa_copying COMMAND nfa_copy-test)

	# add_executable(nfa_eval-test nfa_eval-test.c)
	# target_link_libraries(nfa_eval-test reggie)
	# add_test(NAME nfa_eval COMMAND nfa_eval-test)

	add_executable(nfa_operations-test nfa_operations-test.c)
	target_link_libraries(nfa_operations-test reggie)
	add_test(NAME nfa_operations COMMAND nfa_operations-test)

	# add_executable(nfa_parsing_eval-test nfa_parsing_eval-test.c)
	# target_link_libraries(nfa_parsing_eval-test reggie)
	# add_test(NAME nfa_parsing_eval COMMAND nfa_parsing_eval-test)

	add_executable(nfa-test nfa-test.c)
	target_link_libraries(nfa-test reggie)
	add_test(NAME nfa COMMAND nfa-test)

	add_executable(reggie-test reggie-test.c)
	target_link_libraries(reggie-test reggie)
	add_test(NAME reggie COMMAND reggie-test)

	add_executable(strings-test strings-test.c)
	target_link_libraries(strings-test reggie)
	add_test(NAME strings COMMAND strings-test)
endif()
