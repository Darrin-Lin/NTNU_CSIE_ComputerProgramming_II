with open("./outputs_words.txt", "w") as f:
    with open("./en_US.dic", "r") as d:
        for line in d:
            if line.find("/") == -1:
                if len(line) == 6:
                    if line.find("'") == -1 and line.find("-") == -1:
                        num = False
                        for i in range(0, 10):
                            if line.find(str(i)) != -1:
                                num = True
                                break
                        if num == False:
                            f.write(line)
            else:
                if len(line.split("/")[0]) == 5:
                    if line.find("'") == -1 and line.find("-") == -1:
                        num = False
                        for i in range(0, 10):
                            if line.find(str(i)) != -1:
                                num = True
                                break
                        if num == False:
                            f.write(line.split("/")[0] + "\n")
