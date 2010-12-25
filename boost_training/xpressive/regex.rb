str = "foobarbazbarfoo"

# 単純なマッチ
if str =~ /bar/
  p $& # => "bar"
end

# 後方参照を伴うマッチ
if str =~ /(foo|bar|baz)(.*)\1/
  p $2 # => "barbazbar"
end

# 先読みを伴うマッチ
if str =~ /bar(?=foo)/
  p $& # => "bar"
end

# 最短一致のマッチ
if str =~ /.*?bar/
  p $& # => "foobar"
end

# 単純な置換
p str.gsub(/foo/, "XXX") # => "XXXbarbazbarXXX"

# ブロックを伴う置換
p str.gsub(/foo|bar|baz/) { |s| s.upcase } # => "FOOBARBAZBARFOO"

# 分割
p str.split(/baz/) # => ["foobar", "barfoo"]

# 切り出し
p str.slice(/baz/) # => "baz"

