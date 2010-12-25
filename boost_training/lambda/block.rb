# http://ideone.com/7C3b6
a = [5, 3, 1, 2, 4]
puts a.inject("") { |result, v| result += v.to_s + " " }
a.sort! { |lhs, rhs| lhs <=> rhs }
puts a.inject("") { |result, v| result += v.to_s + " " }
a.sort! { |lhs, rhs| rhs <=> lhs }
puts a.inject("") { |result, v| result += v.to_s + " " }
