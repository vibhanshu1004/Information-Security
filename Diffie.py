import math
def power(a,b, p):
    if b ==1:
        return a
    else:
        return pow(a,b) % p

def diffie():
    q = int(input("Enter the value of q(prime): "))
    alpha = int(input("Enter the value of alpha(prime): "))
    Xa= int(input("Enter the pvt key of alice: "))
    Xb= int(input("Enter the pvt key of bob: "))
    
    Ya= power(alpha,Xa , q)
    Yb= power(alpha, Xb ,q)
    
    print("The public key of Alice is :",Ya)
    print("The public key of Bob is :",Yb)
    
    ka= power(Yb, Xa ,q)
    kb= power(Ya, Xb ,q)
    print("The secret key of Alice is :",ka)
    print("The secret key of Bob is :",kb)
    
    if ka == kb:
        print("Key exchange successfuland shared secret key is:", ka)
    else:
        print("Error: Keys do not match!")
        
diffie()
  
'''
OUTPUT
Enter the value of q(prime): 7
Enter the value of alpha(prime): 5
Enter the pvt key of alice: 3
Enter the pvt key of bob: 4
The public key of Alice is : 6
The public key of Bob is : 2
The secret key of Alice is : 1
The secret key of Bob is : 1
Key exchange successfuland shared secret key is: 1
'''
    
    
    
   

    
   