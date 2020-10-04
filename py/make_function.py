def makefunction(x):
    def add(a,b=x):
        return a+ b

    return add

add1 = makefunction(5)
print add1(10)