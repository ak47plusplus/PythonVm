def func(rad, *args):
    q = 'sb'
    x = 2
    def say():
        print x
    x = 99
    return say

f = func(100, 2)
f()
