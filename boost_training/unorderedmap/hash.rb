# http://ideone.com/zBctE
a = Hash.new
a["hoge"] = 10
a["fuga"] = nil
 
puts a["hoge"]
 
if a["pnyo"] then
    puts "NG"
else
    puts "Expected1"
end
 
if a["fuga"] then
    puts "NG"
else
    puts "Expected2"
end
 
if a.key?("fuga") then
    puts "Expected3"
else
    puts "NG"
end
 
if a.key?("pnyo") then
    puts "NG"
else
    puts "Expected4"
end
