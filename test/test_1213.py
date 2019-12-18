def test(x):
    y = 3
    def say():
        print x
        print y
    return say

f = test(1)
f()
