def neighbours(a):
    return sum(a[1:])

print("""
     [E]
  [D][A][B]
     [C]
""")
print("A B C D E A'")

for i in range(0, 2**5):
    mask = []
    temp = i
    for j in range(5):
        mask.append(temp % 2)
        temp //= 2

    mask = mask[::-1]
    neighbours_count = neighbours(mask)
    if mask[0]:
        if neighbours_count >= 2 or neighbours_count == 0:
            print(*mask, 0)
        else:
            print(*mask, 1)
    else:
        if neighbours_count == 2:
            print(*mask, 1)
        else:
            print(*mask, 0)
            
    
        
