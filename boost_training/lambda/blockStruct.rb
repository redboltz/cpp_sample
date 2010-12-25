# http://ideone.com/xrQKz
class My 
    def initialize(val)
        @val = val
    end
    attr_reader :val
end
 
a = [ My.new(5), My.new(3), My.new(1), My.new(2), My.new(4)]
puts a.inject("") { |result, v| result += v.val.to_s + " " }
a.sort! { |lhs, rhs| lhs.val <=> rhs.val }
puts a.inject("") { |result, v| result += v.val.to_s + " " }
a.sort! { |lhs, rhs| rhs.val <=> lhs.val }
puts a.inject("") { |result, v| result += v.val.to_s + " " }
