#!/usr/bin/env ruby

size = Random.rand(10...100)
inserts = []
deletes = []
expected = []

for i in 0...size
	a = Random.rand(0...100)	
	inserts.push(a)
end

deletes = inserts.shuffle

expected = inserts - deletes

input = "This file inserts " + size.to_s + " values and then attempts to delete " + size.to_s + " of them\n"
output = ""

for i in 0...size 
	input = input + "i" + inserts[i].to_s + " "
end

for i in 0...size
	input = input + "d" + deletes[i].to_s + " "
end	

for i in 0..expected.length
	output = output + " " + expected[i].to_s
end

File.write("inputs", input)
File.write("outputs", output)
