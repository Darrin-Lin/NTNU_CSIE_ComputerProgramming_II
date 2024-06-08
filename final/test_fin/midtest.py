import random as r
mid_part=1
for i in range(0,10):
    with open('./0'+str(mid_part)+'/in/mid'+"0"+str(mid_part)+"_0"+str(i)+'.in', 'w') as f:
        f.write(str(r.randint(1,10)))
        f.write('\n')
