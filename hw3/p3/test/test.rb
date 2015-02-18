#!/usr/bin/env ruby
require 'open3'

parent = "inputs"
input = ["in1", "in2", "in3", "in4", "in5"]

for i in 0...input.length
	mine = ""

	stdout, stderr, status = Open3.capture3("./a.out < " + parent + "/" + input[i])
	stdout = stdout.split("\n")

	puts stdout[0]
	stdout = stdout[1, stdout.length]
	
	
	file = File.read(parent + "/" + input[i])
	nums = []

	content = File.read(file.strip())
	content = content.split("\n")
	content = content[1].split(" ")
	
	min = 0
	max = 0

	for n in 0...content.length
		if content[n][0] == "r" 
			min = content[n].gsub("r", "")

		elsif content[n][0] == "i"	
			nums.push(content[n].gsub("i", ""))
		else
			max = content[n]
		end
	end

	#puts nums
	#puts nums
	#puts input[i]
	#puts input[i] + " ------------ "
	#puts nums - stdout
	is_valid = true
	
	results = nums - stdout

	for k in 0...results.length
		results[k] = results[k].to_i
		if results[k] < min.to_i or results[k] > max.to_i then next 
		else 
			puts results[k]
			is_valid = false 
		end
	end

	if is_valid then puts "SAME"
	else puts "BAD" end
end
