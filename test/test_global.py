param = 10

def test():
    if param == 10:
        print "equal 10"
    else:
        print "not equal 10"

def main():
    print "main"
    global param
    param = 'helloworld'
    test()

main()
