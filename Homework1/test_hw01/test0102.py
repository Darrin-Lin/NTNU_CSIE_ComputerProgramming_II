import random as r
HW=1
HW_PART=2
operand = ['+','-','*']
for i in range(0,3):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        f.write(str(r.randint(1,1024)) + '_' + str(r.randint(10,16)))
        for j in range(1,10):
            f.write(' '+r.choice(operand)+' ')
            f.write(str(r.randint(1,1024)) + '_' + str(r.randint(10,16)))
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(3,6):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        num = r.randint(1,1024)
        base = [2,8,10,16]
        useing_base=r.choice(base)
        if useing_base == 2:
            num = bin(num)
            num = num[2:]
        elif useing_base == 8:
            num = oct(num)
            num = num[2:]
        elif useing_base == 10:
            num = str(num)
        elif useing_base == 16:
            num = hex(num)
            num = num[2:]
            num = num.upper()
        f.write(num + '_' + str(useing_base))
        for j in range(1,10):
            f.write(' '+r.choice(operand)+' ')
            num = r.randint(1,1024)
            base = [2,8,10,16]
            useing_base=r.choice(base)
            if useing_base == 2:
                num = bin(num)
                num = num[2:]
            elif useing_base == 8:
                num = oct(num)
                num = num[2:]
            elif useing_base == 10:
                num = str(num)
            elif useing_base == 16:
                num = hex(num)
                num = num[2:]
                num = num.upper()
            f.write(num + '_' + str(useing_base))
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(6,7):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        f.write('1_2')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(7,8):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        f.write('+2334_8 + 245_10')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(8,9):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        f.write('1_2+ 5_8')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(9,10):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_0"+str(i)+'.in', 'w') as f:
        f.write('1_2 / 5_8')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(10,11):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_"+str(i)+'.in', 'w') as f:
        f.write('1__2 + 123_4')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(11,12):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_"+str(i)+'.in', 'w') as f:
        f.write('0_2 + 123_4')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(12,13):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_"+str(i)+'.in', 'w') as f:
        f.write('1_2 + 123_3')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(13,14):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_"+str(i)+'.in', 'w') as f:
        f.write('1_2 + _4')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(14,15):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_"+str(i)+'.in', 'w') as f:
        f.write('1_2 + 123_')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(15,16):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_"+str(i)+'.in', 'w') as f:
        f.write('1_2 + 123_4 +')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(16,17):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_"+str(i)+'.in', 'w') as f:
        f.write('1_2 + 123_4 + 1aa_16')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(17,18):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_"+str(i)+'.in', 'w') as f:
        f.write('1_2 + 123_4 + 1_16')
        f.write('\n')
        f.write('1')
for i in range(18,19):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_"+str(i)+'.in', 'w') as f:
        f.write('1_2 + 123_4 +1_17')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')
for i in range(19,20):
    with open('./0'+str(HW_PART)+'/in/hw0'+str(HW)+'0'+str(HW_PART)+"_"+str(i)+'.in', 'w') as f:
        f.write('1_2 + 123_4  + 1_12')
        f.write('\n')
        f.write(str(r.randint(2,16)))
        f.write('\n')