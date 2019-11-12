def build(x):
    def add(a,b=x):
        return a + b
    return add

f = build(100)
res = f(1)
print "res = ", res
