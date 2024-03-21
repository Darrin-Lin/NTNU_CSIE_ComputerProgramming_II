import random as r
HW=1
HW_PART=1
for i in range(0,9):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        for j in range(1,100):
            typ = r.randint(1,7)
            f.write(str(typ)+'\n')
            for k in range(r.randint(1,800)):
                f.write(chr(r.randint(50,70)))
            f.write('\n')
            if(typ == 1 or typ ==2 or typ == 7):
                f.write(chr(r.randint(50,70)))
            elif(typ == 3 or typ == 4 or typ == 5 or typ == 6):
                for k in range(3,10):
                    f.write(chr(r.randint(50,70)))
            f.write('\n')
                
for i in range(9,10):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        for j in range(-1,-8,-1):
            f.write(str(j)+'\n')