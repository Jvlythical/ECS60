#!/usr/bin/env ruby


ar = ARGF.read.split("\n")

puts ar.uniq.length == ar.length
