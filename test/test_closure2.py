def outer():
    x = 3
    y = 4
    def inner1():
        print x
        print y
        z= 5
        def inner2():
            print x
            print y
            print z
        return inner2
    return inner1

f = outer()
f1 = f()
f1()
