#!/usr/bin/env ruby

require 'open3'

valid = true

for i in 0...10
	stdout, stderr, status = Open3.capture3("./rand.rb")

	File.write("inputs", stdout)

	stdout = stdout.split("\n")
	a = stdout[0]
	b = stdout[1]

	#puts a + " + " + b

	stdout, stderr, status = Open3.capture3("./LongInt.out < inputs")

	a = stdout[stdout.rindex(">") + 2, stdout.length]
	#puts "Calculated:\n" + a

	stdout, stderr, status = Open3.capture3("./LongInt.rb < inputs")

	b = stdout
	#puts "Expected:\n" + b

	#if a == b then puts "Equal!" 
	#else puts "Not equal!" end

	if a != b then valid = false 
	else puts a.strip + " = " + b.strip end
end

if valid then puts "All good"
else puts "Bad" end
