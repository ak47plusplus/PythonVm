def test():
    fd = ''
    try:
        print "start"
        fd = open('/dev/null','rw', encoding='utf-8')
        print "end"
        return 123
    except BaseException as e:
        print "error"
    finally:
        fd.close()

test()
