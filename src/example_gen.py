import exrex
import random
from itertools import islice

def generate_regex():
    regex = ""
    for i in range(random.randint(5, 10)):   # 长度随机
        add_brac = random.randint(0, 1)     # 是否加括号
        add_or = random.randint(0, 1)       # 是否加or
        add_star = random.randint(0, 1)     # 是否加*

        s1 = exrex.getone("[a-z]{1,2}") # 生成随机长度的小写字母
        s2 = exrex.getone("[a-z]{1,2}") 

        if add_brac:
            if add_or:
                regex += "(" + s1 + "|" + s2 + ")"
            else:
                regex += "(" + s1 + ")"
        else:
            if add_or:
                regex += s1 + "|" + s2
            else:
                regex += s1

        if add_star:
            regex += "*"
    return regex

def generate_example(s, f):
    gen = exrex.generate(s)
    strings = list(islice(gen, 100))
    # random.shuffle(strings)
    for s in strings:
        f.write(s + "\n")

with open("./test/regex.txt", "w") as f1:
    for i in range(100):
        regex = generate_regex()
        f1.write(regex + ";\n")

        
        path = "./test/eg/example{}.txt".format(i)
        with open(path, "w") as f2:
            generate_example(regex, f2)
    f1.write('q\n')
