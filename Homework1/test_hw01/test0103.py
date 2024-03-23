import random as r
HW=1
HW_PART=3
for i in range(0,4):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        ysize = r.randint(1, 10)
        xsize = r.randint(1, 10)
        f.write(str(ysize)+' '+str(xsize)+'\n')
        for j in range(0, ysize):
            f.write(str(r.randint(1, 3))+' ')
        f.write('\n')
        for j in range(0, ysize):
            f.write(str(r.randint(-100, 100))+' ')
        f.write('\n\n')
        for j in range(0, xsize):
            f.write(str(r.randint(1, 5))+' ')
        f.write('\n')
        for j in range(0, xsize):
            f.write(str(r.randint(-10, 10))+' ')
        f.write('\n')
for i in range(4,6):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        ysize = r.randint(1, 10)
        xsize = r.randint(1, 10)
        f.write(str(ysize)+' '+str(xsize)+'\n')
        for j in range(0, ysize):
            f.write(str(r.randint(1, 10))+' ')
        f.write('\n')
        for j in range(0, ysize):
            f.write(str(r.randint(-1000, 1000))+' ')
        f.write('\n\n')
        for j in range(0, xsize):
            f.write(str(r.randint(1, 5))+' ')
        f.write('\n')
        for j in range(0, xsize):
            f.write(str(r.randint(-3, 3))+' ')
        f.write('\n')
for i in range(6,8):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        ysize = r.randint(1, 1000)
        xsize = r.randint(1, 1000)
        f.write(str(ysize)+' '+str(xsize)+'\n')
        for j in range(0, ysize):
            f.write(str(r.randint(1, 3))+' ')
        f.write('\n')
        for j in range(0, ysize):
            f.write(str(r.randint(-1000, 1000))+' ')
        f.write('\n\n')
        for j in range(0, xsize):
            f.write(str(r.randint(1, 10))+' ')
        f.write('\n')
        for j in range(0, xsize):
            f.write(str(r.randint(-100, 100))+' ')
        f.write('\n')
for i in range(8,9):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        ysize = r.randint(1, 10)
        xsize = 1
        f.write(str(ysize)+' '+str(xsize)+'\n')
        for j in range(0, ysize):
            f.write(str(r.randint(1, 1000000000))+' ')
        f.write('\n')
        for j in range(0, ysize):
            f.write(str(r.randint(-1000, 1000))+' ')
        f.write('\n\n')
        for j in range(0, xsize):
            f.write('1 ')
        f.write('\n')
        for j in range(0, xsize):
            f.write(str(r.randint(-1, 1))+' ')
        f.write('\n')
for i in range(9,10):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        ysize = r.randint(1, 10)
        xsize = r.randint(1, 10)
        f.write(str(ysize)+' '+str(xsize)+'\n')
        for j in range(0, ysize):
            f.write(str(r.randint(1, 2))+' ')
        f.write('\n')
        for j in range(0, ysize):
            f.write(str(r.randint(-1000, 1000))+' ')
        f.write('\n\n')
        for j in range(0, xsize):
            f.write(str(r.randint(1, 100000))+' ')
        f.write('\n')
        for j in range(0, xsize):
            f.write('1 ')
        f.write('\n')
