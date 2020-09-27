#!/usr/bin/env python
import pprint

a = open('qemu/linux-user/syscall.c', 'r').read()
ptr = 0
result = {}
while True:
	spot1 = a.find('\n_syscall', ptr)
	if spot1 == -1: spot1 = float('inf')
	spot2 = a.find('\nsafe_syscall', ptr)
	if spot2 == -1: spot2 = float('inf')
	spot = min(spot1, spot2)
	if spot == float('inf'):
		break
	realspot = a.find('(', spot)
	endspot = a.find(')', spot)
	defn =  a[realspot+1:endspot]
	elems = [b.strip(' \t\n\\') for b in defn.split(',')]
	retty = elems[0]
	name = elems[1]
	types = [(elems[i], elems[i+1]) for i in range(2, len(elems), 2)]
	result[name] = (retty, types)
	ptr = endspot

pprint.pprint(result)
