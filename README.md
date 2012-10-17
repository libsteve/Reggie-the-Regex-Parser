Reggie the Regex Parser
=======================

Reggie the Regex Parser (RRP) is a regualr expression parser that will have the ability to identify strings that match the regular expression.

Purpose
-------

To learn about and understand regualr expressions and finite automata.

To write awesome things in the C language.

TO become a badass and super-being among mortals.

To create the foundations for a regex parser for possible future use in attempts at creating compilers.

Design
------

Regualr expressions are written forms of a non-deterministic finite automata (NFA), which represents a machine that matches strings of characters for patterns encoded within the machine.

An NFA is a directed graph with one initial node state (known as "q0") and any number of other states. Any number of states within the NFA can be an accepting state that would signify the acceptance of a string when the string encounters its end. Transitions between states are invoked by encountering specified characters. [Learn more about NFAs from Wikipedia](http://en.wikipedia.org/wiki/Nondeterministic_finite_automaton).

This regex parser will take a regualr expression string and create an NFA modeled by that string.

A user can then take the resulting NFA and pass in a string to see if it passes the NFA.

Use
---

To compile and run tests, run

	make test

To clean the build files, run

	make clean

To clean the build files and binaries, run

	make real-clean