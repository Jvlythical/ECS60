#!/usr/bin/env ruby

require 'open3'

for i in 0...10
	stdout, stderr, status = Open3.capture3("./rand.rb")

	internal = Random.rand(3...7).to_s
	leaf  =  Random.rand(3...7).to_s

	cmd = "./BTree inputs " + internal  + " " + leaf
	stdout, stderr, status = Open3.capture3(cmd)
	
	if stdout.rindex("Internal")  == nil
		puts internal.to_s + " " + leaf.to_s + " " + "Failed deleting properly"
		stdout, stderr, status = Open3.capture3("mv inputs failed/")
		next
	end

	if stdout.length > 100 
		tmp = stdout[stdout.rindex("Internal"), stdout.length]
		tmp = tmp[tmp.index("\n") + 1, tmp.length]
		tmp = tmp.gsub(/[^0-9]/, " ")
		tmp = tmp.gsub(/\s/, " ")

		a = []
		b = []
		diff = []

		a = tmp.split(" ")

		expected = File.read("outputs")
		File.write("my_outputs.txt", a)
		
		b = expected.split(" ")
		diff = a - b
		
		if diff.length == 0 
			puts cmd + " => passed" + " here"
		elsif stdout[stdout.rindex("Leaf"), stdout.length] == "Leaf: \n"
			puts cmd + " => passed"
		else 
			puts internal.to_s + " " + leaf.to_s + " " + (a - b).to_s
			stdout, stderr, status = Open3.capture3("mv inputs failed/")
		end
	else 
		puts internal.to_s + " " + leaf.to_s + " " + "Seg Fault"
		stdout, stderr, status = Open3.capture3("mv inputs failed/")
	end
end
