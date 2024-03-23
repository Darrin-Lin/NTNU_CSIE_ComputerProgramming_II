import random as r
HW=1
HW_PART=4   
for i in range(0,3):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        col = r.randint(1, 10)
        row = r.randint(1, 10)
        door = r.randint(0,3)
        f.write(str(row)+' '+str(col)+'\n')
        for j in range(0,row):
            for k in range(0,col):
                f.write(str(r.randint(1, 100))+' ')
                f.write(str(door)+' '+str(door)+' '+str(door)+' '+str(door)+'\n')
            f.write('\n')
        f.write('\n')
for i in range(3,6):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        col = r.randint(1, 100)
        row = r.randint(1, 100)
        door = r.randint(0,3)
        f.write(str(row)+' '+str(col)+'\n')
        for j in range(0,row):
            for k in range(0,col):
                f.write(str(r.randint(1, 100))+' ')
                f.write(str(door)+' '+str(door)+' '+str(door)+' '+str(door)+'\n')
            f.write('\n')

    