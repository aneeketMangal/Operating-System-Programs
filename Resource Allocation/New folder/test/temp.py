def f(x):
    if(x == 0):
        return 3
    if(abs(x) == 1):
        return 2
    if(abs(x) ==2):
        return 1
    return 0

def g(x):
    if(x == 0):
        return 2
    if(x == -3 or x ==3):
        return -1
    return 0
    
def x(i):
    ans = 0
    for j in range(-2, 3):
        ans+= (f(j)*g(i-j))
        
    return ans
    
for i in range(-10, 16):
    print(i, x(i))